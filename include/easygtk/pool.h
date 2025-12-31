/**
 * EasyGTK - Memory Pool
 * 
 * Pool de alocação para widgets pequenos.
 * Reduz fragmentação e melhora performance.
 */

#ifndef EASYGTK_POOL_H
#define EASYGTK_POOL_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo pool de memória.
 * 
 * @param block_size Tamanho de cada bloco (deve ser >= sizeof(void*))
 * @param initial_blocks Número inicial de blocos pré-alocados
 * @return Ponteiro para o pool ou NULL
 */
EgMemoryPool *eg_pool_new(size_t block_size, size_t initial_blocks);

/**
 * Libera o pool e toda memória associada.
 * 
 * @param pool Ponteiro para o pool
 */
void eg_pool_free(EgMemoryPool *pool);

/**
 * Aloca um bloco do pool.
 * 
 * @param pool Ponteiro para o pool
 * @return Ponteiro para o bloco ou NULL se falhar
 */
void *eg_pool_alloc(EgMemoryPool *pool);

/**
 * Devolve um bloco ao pool.
 * 
 * @param pool Ponteiro para o pool
 * @param ptr Ponteiro para o bloco a devolver
 */
void eg_pool_release(EgMemoryPool *pool, void *ptr);

/**
 * Obtém estatísticas do pool.
 * 
 * @param pool Ponteiro para o pool
 * @param total_blocks Retorna total de blocos alocados (pode ser NULL)
 * @param free_blocks Retorna blocos livres (pode ser NULL)
 * @param block_size Retorna tamanho do bloco (pode ser NULL)
 */
void eg_pool_stats(EgMemoryPool *pool, size_t *total_blocks, 
                   size_t *free_blocks, size_t *block_size);

/**
 * Reseta o pool, liberando todos os blocos.
 * Não libera a memória, apenas marca como disponível.
 * 
 * @param pool Ponteiro para o pool
 */
void eg_pool_reset(EgMemoryPool *pool);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_POOL_H */
