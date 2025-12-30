# EasyGTK SPA Example

Exemplo completo de uma **Single Page Application** usando EasyGTK com padrão MVVM.

## Características

### Arquitetura MVVM Completa
- **ViewModel Global**: Estado compartilhado entre todas as páginas
- **Data Binding Declarativo**: Ligação automática entre UI e dados
- **Computed Properties**: Propriedades calculadas automaticamente
- **Commands**: Ações reutilizáveis com validação can_execute

### Funcionalidades Implementadas

#### 1. Home Page
- Exibe estatísticas calculadas automaticamente:
  - Total de produtos em estoque (computed property)
  - Valor total do inventário (computed property)
- Navegação para outras páginas
- Atualização reativa quando dados mudam

#### 2. Products Page
- Lista de produtos com informações
- Botão **Refresh** que atualiza quantidades
- Demonstra atualização reativa das computed properties
- ScrolledWindow para listas longas

#### 3. Profile Page
- Formulário com data binding two-way:
  - Username (Entry)
  - Email (Entry)
  - Notificações (CheckButton)
- Botão **Save** com validação:
  - Desabilitado quando username vazio
  - Atualização automática do estado
- Command binding para ações

## Conceitos Demonstrados

### 1. Computed Properties
```c
/* Total de produtos calculado automaticamente */
static void compute_total_products(EgProperty *computed, void *user_data) {
    int total = 0;
    for (int i = 0; i < product_count; i++) {
        total += products[i].quantity;
    }
    eg_property_set_int(computed, total);
}
```

### 2. Data Binding Declarativo
```c
/* Two-way binding: Entry <-> Property */
eg_bind_entry_text(username_entry, app_vm, "username");

/* One-way binding: Label <- Property */
eg_bind_label_text(stat1_value, app_vm, "total_products");

/* Command binding: Button -> Command */
eg_bind_button_command(btn_save, app_vm, "save_profile");
```

### 3. Commands com Validação
```c
/* Command com can_execute dinâmico */
static bool cmd_save_profile_can_execute(EgCommand *cmd, void *param, void *user_data) {
    const char *username = eg_view_model_get_string(app_vm, "username");
    return username != NULL && username[0] != '\0';
}

/* Observa mudanças para atualizar can_execute */
eg_property_on_changed(username_prop, on_username_changed_for_save, save_cmd);
```

### 4. Navegação entre Páginas
```c
/* Stack para páginas */
EgStack *page_stack = eg_stack_new();
eg_stack_add_named(page_stack, create_home_page(), "home");
eg_stack_add_named(page_stack, create_products_page(), "products");
eg_stack_add_named(page_stack, create_profile_page(), "profile");

/* Navegação via command */
static void cmd_navigate_products(EgCommand *cmd, void *param, void *user_data) {
    eg_stack_set_visible_child_name(page_stack, "products");
}
```

## Como Executar

```bash
# Compilar
cmake -B build -S .
cmake --build build -j4

# Executar
./build/spa_example
```

## Benefícios do Padrão MVVM

1. **Separação de Responsabilidades**: UI separada da lógica
2. **Testabilidade**: ViewModel pode ser testado sem UI
3. **Reatividade**: Mudanças se propagam automaticamente
4. **Reutilização**: Commands e bindings são declarativos
5. **Manutenibilidade**: Código organizado e desacoplado

## Próximos Passos

Possíveis melhorias:
- Adicionar persistência de dados
- Implementar navegação com histórico
- Adicionar validações mais complexas
- Implementar ListView para listas dinâmicas
- Adicionar animações de transição
