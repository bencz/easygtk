/**
 * EasyGTK - Memory Pool
 */

#include <stdlib.h>
#include <string.h>
#include "internal/internal.h"
#include <easygtk/pool.h>

/* Estrutura de bloco livre (lista encadeada) */
typedef struct FreeBlock {
    struct FreeBlock *next;
} FreeBlock;

/* Estrutura do pool */
struct EgMemoryPool {
    size_t block_size;      /* Tamanho de cada bloco */
    size_t total_blocks;    /* Total de blocos alocados */
    size_t free_count;      /* Blocos livres */
    FreeBlock *free_list;   /* Lista de blocos livres */
    void **chunks;          /* Array de chunks alocados */
    size_t chunk_count;     /* Número de chunks */
    size_t chunk_capacity;  /* Capacidade do array de chunks */
};

/* Aloca um novo chunk de blocos */
static bool pool_grow(EgMemoryPool *pool, size_t num_blocks) {
    if (pool == NULL || num_blocks == 0) return false;
    
    /* Expandir array de chunks se necessário */
    if (pool->chunk_count >= pool->chunk_capacity) {
        size_t new_capacity = pool->chunk_capacity == 0 ? 4 : pool->chunk_capacity * 2;
        void **new_chunks = realloc(pool->chunks, new_capacity * sizeof(void *));
        if (new_chunks == NULL) return false;
        pool->chunks = new_chunks;
        pool->chunk_capacity = new_capacity;
    }
    
    /* Alocar novo chunk */
    size_t chunk_size = pool->block_size * num_blocks;
    void *chunk = malloc(chunk_size);
    if (chunk == NULL) return false;
    
    memset(chunk, 0, chunk_size);
    pool->chunks[pool->chunk_count++] = chunk;
    
    /* Adicionar blocos à lista livre */
    char *ptr = (char *)chunk;
    for (size_t i = 0; i < num_blocks; i++) {
        FreeBlock *block = (FreeBlock *)ptr;
        block->next = pool->free_list;
        pool->free_list = block;
        ptr += pool->block_size;
    }
    
    pool->total_blocks += num_blocks;
    pool->free_count += num_blocks;
    
    return true;
}

EgMemoryPool *eg_pool_new(size_t block_size, size_t initial_blocks) {
    /* Garantir tamanho mínimo para a lista encadeada */
    if (block_size < sizeof(FreeBlock)) {
        block_size = sizeof(FreeBlock);
    }
    
    EgMemoryPool *pool = malloc(sizeof(EgMemoryPool));
    if (pool == NULL) return NULL;
    
    pool->block_size = block_size;
    pool->total_blocks = 0;
    pool->free_count = 0;
    pool->free_list = NULL;
    pool->chunks = NULL;
    pool->chunk_count = 0;
    pool->chunk_capacity = 0;
    
    if (initial_blocks > 0) {
        if (!pool_grow(pool, initial_blocks)) {
            free(pool);
            return NULL;
        }
    }
    
    return pool;
}

void eg_pool_free(EgMemoryPool *pool) {
    if (pool == NULL) return;
    
    /* Liberar todos os chunks */
    for (size_t i = 0; i < pool->chunk_count; i++) {
        free(pool->chunks[i]);
    }
    free(pool->chunks);
    free(pool);
}

void *eg_pool_alloc(EgMemoryPool *pool) {
    if (pool == NULL) return NULL;
    
    /* Se não há blocos livres, alocar mais */
    if (pool->free_list == NULL) {
        size_t grow_size = pool->total_blocks > 0 ? pool->total_blocks : 16;
        if (!pool_grow(pool, grow_size)) {
            return NULL;
        }
    }
    
    /* Remover da lista livre */
    FreeBlock *block = pool->free_list;
    pool->free_list = block->next;
    pool->free_count--;
    
    /* Limpar o bloco */
    memset(block, 0, pool->block_size);
    
    return block;
}

void eg_pool_release(EgMemoryPool *pool, void *ptr) {
    if (pool == NULL || ptr == NULL) return;
    
    /* Adicionar de volta à lista livre */
    FreeBlock *block = (FreeBlock *)ptr;
    block->next = pool->free_list;
    pool->free_list = block;
    pool->free_count++;
}

void eg_pool_stats(EgMemoryPool *pool, size_t *total_blocks, 
                   size_t *free_blocks, size_t *block_size) {
    if (pool == NULL) {
        if (total_blocks) *total_blocks = 0;
        if (free_blocks) *free_blocks = 0;
        if (block_size) *block_size = 0;
        return;
    }
    
    if (total_blocks) *total_blocks = pool->total_blocks;
    if (free_blocks) *free_blocks = pool->free_count;
    if (block_size) *block_size = pool->block_size;
}

void eg_pool_reset(EgMemoryPool *pool) {
    if (pool == NULL) return;
    
    /* Reconstruir lista livre com todos os blocos */
    pool->free_list = NULL;
    pool->free_count = 0;
    
    for (size_t i = 0; i < pool->chunk_count; i++) {
        char *ptr = (char *)pool->chunks[i];
        size_t blocks_in_chunk = (i == 0) ? 
            (pool->total_blocks / pool->chunk_count) : 
            (pool->total_blocks / pool->chunk_count);
        
        for (size_t j = 0; j < blocks_in_chunk; j++) {
            FreeBlock *block = (FreeBlock *)ptr;
            block->next = pool->free_list;
            pool->free_list = block;
            pool->free_count++;
            ptr += pool->block_size;
        }
    }
}
