# SPA Dashboard - Advanced Single Page Application

Uma aplicação **Single Page Application** completa e profissional construída com **EasyGTK**, demonstrando todos os recursos avançados da biblioteca.

## 🎯 Características

### Arquitetura Profissional
- **MVVM Completo**: ViewModel, Model, Properties observáveis
- **SQLite Database**: Persistência de dados com autenticação
- **Data Binding Declarativo**: UI reativa automática
- **Computed Properties**: Estatísticas calculadas em tempo real
- **Command Pattern**: Navegação e ações encapsuladas

### Visual Moderno
- **Dark Theme**: Interface moderna estilo Catppuccin
- **Sidebar Navigation**: Menu lateral fixo com ícones
- **Responsive Layout**: Layout adaptável com panes
- **CSS Customizado**: Gradientes, sombras e transições
- **Cards & Stats**: Componentes visuais elegantes

### Funcionalidades

#### 🔐 Sistema de Autenticação
- Login com validação
- Gerenciamento de sessão
- Logout seguro
- Credenciais padrão: `admin` / `admin123`

#### 📊 Dashboard
- Total de usuários cadastrados
- Total de produtos em estoque
- Valor total do inventário
- Produtos com estoque baixo
- Atualização automática via computed properties

#### 👥 Gerenciamento de Usuários
- Listagem de todos os usuários
- Exibição de status (ativo/inativo)
- Dados vindos do SQLite
- Interface scroll para listas longas

#### 📦 Gerenciamento de Produtos
- Listagem de produtos com detalhes
- Preço, estoque e categoria
- Busca e filtragem (preparado)
- CRUD completo (estrutura pronta)

#### ⚙️ Configurações
- Exibição de usuário logado
- Opção de logout
- Perfil do usuário (extensível)

## 📁 Estrutura do Projeto

```
spa_app/
├── assets/
│   └── style.css              # CSS moderno completo
├── database/
│   └── app.db                 # SQLite (gerado automaticamente)
├── include/
│   ├── database.h             # API do banco de dados
│   ├── app_state.h            # Gerenciamento de estado global
│   └── pages.h                # Declarações das páginas
├── src/
│   ├── main.c                 # Aplicação principal
│   ├── database.c             # Implementação SQLite
│   ├── app_state.c            # Estado da aplicação
│   ├── page_login.c           # Página de login
│   ├── page_dashboard.c       # Dashboard com stats
│   ├── page_users.c           # Gerenciamento de usuários
│   ├── page_products.c        # Gerenciamento de produtos
│   └── page_settings.c        # Configurações
├── CMakeLists.txt             # Build configuration
└── README.md                  # Este arquivo
```

## 🚀 Compilação

### Pré-requisitos

- GTK4
- SQLite3
- CMake 3.10+
- EasyGTK compilado (na pasta pai)

### Build

```bash
# A partir do diretório spa_app/
mkdir build && cd build
cmake ..
make -j4
```

### Executar

```bash
# A partir do diretório spa_app/build/
./spa_dashboard
```

**Nota**: Execute sempre do diretório build para que os caminhos relativos funcionem.

## 🔑 Credenciais Padrão

### Administrador
- **Username**: `admin`
- **Password**: `admin123`

### Usuários Demo
- **Username**: `john` | **Password**: `password`
- **Username**: `jane` | **Password**: `password`

## 🎨 Temas de Cores

### Paleta Principal (Catppuccin Mocha)
- **Background**: `#1e1e2e` (dark)
- **Surface**: `#313244` (medium)
- **Accent**: `#89b4fa` (blue)
- **Text**: `#cdd6f4` (light)
- **Subtext**: `#7f849c` (muted)
- **Success**: `#a6e3a1` (green)
- **Danger**: `#f38ba8` (red)

### Classes CSS Principais
- `.sidebar` - Navegação lateral
- `.main-content` - Área principal
- `.stat-card` - Cards de estatísticas
- `.nav-button` - Botões de navegação
- `.btn-primary` - Botão primário
- `.card` - Card genérico

## 🏗️ Arquitetura

### Estado Global (AppState)
```c
typedef struct {
    EgViewModel *view_model;      // ViewModel MVVM
    EgModel *current_user_model;  // Model do usuário logado
    User *logged_user;            // Dados do usuário
    AppPage current_page;         // Página atual
    EgStack *main_stack;          // Stack login/app
    EgStack *content_stack;       // Stack de conteúdo
    bool is_authenticated;        // Status de autenticação
} AppState;
```

### Navegação
```c
app_state_navigate_to(PAGE_DASHBOARD);
app_state_navigate_to(PAGE_USERS);
app_state_navigate_to(PAGE_PRODUCTS);
app_state_navigate_to(PAGE_SETTINGS);
```

### Data Binding
```c
// Label se atualiza automaticamente quando property muda
eg_bind(eg_label_as_widget(label), view_model, "total_users");

// Entry com two-way binding
eg_bind(eg_entry_as_widget(entry), view_model, "username");

// Button executa command automaticamente
eg_bind_cmd(eg_button_as_widget(button), view_model, "logout");
```

## 📊 Banco de Dados

### Tabela: users
```sql
- id (INTEGER PRIMARY KEY)
- username (TEXT UNIQUE)
- email (TEXT)
- password_hash (TEXT)
- is_active (BOOLEAN)
- created_at (TIMESTAMP)
```

### Tabela: products
```sql
- id (INTEGER PRIMARY KEY)
- name (TEXT)
- description (TEXT)
- price (REAL)
- stock (INTEGER)
- category (TEXT)
- created_at (TIMESTAMP)
```

## 🔧 Extensões Possíveis

### Features Implementadas
- [x] CRUD completo para usuários (criar, editar, deletar)
- [x] CRUD completo para produtos
- [x] Sistema de busca em tempo real
- [x] Alteração de senha de usuários
- [x] Tema claro/escuro toggle

### Features a Adicionar
- [ ] Paginação de listas
- [ ] Validações de formulário avançadas
- [ ] Upload de imagens de produtos
- [ ] Relatórios e gráficos
- [ ] Sistema de notificações
- [ ] Histórico de ações (audit log)
- [ ] Multi-tenancy (múltiplas organizações)

### Melhorias Técnicas
- [ ] Criptografia real de senhas (bcrypt/argon2)
- [ ] Tokens JWT para sessões
- [ ] Migrations do banco de dados
- [ ] Testes unitários
- [ ] Internacionalização (i18n)
- [ ] Backup automático do DB

## 📝 Licença

MIT License - Parte do projeto EasyGTK

## 🙏 Créditos

- **EasyGTK**: Biblioteca wrapper para GTK4
- **GTK4**: Toolkit de interface gráfica
- **SQLite**: Database engine
- **Catppuccin**: Paleta de cores
