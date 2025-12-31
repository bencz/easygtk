# SPA Dashboard - Advanced Single Page Application

A complete and professional **Single Page Application** built with **EasyGTK**, demonstrating all the advanced features of the library.

## 🎯 Features

### Professional Architecture
- **Complete MVVM**: ViewModel, Model, Observable Properties
- **SQLite Database**: Data persistence with authentication
- **Declarative Data Binding**: Automatic reactive UI
- **Computed Properties**: Real-time calculated statistics
- **Command Pattern**: Encapsulated navigation and actions

### Modern Visuals
- **Dark Theme**: Modern Catppuccin-style interface
- **Sidebar Navigation**: Fixed sidebar menu with icons
- **Responsive Layout**: Adaptable layout with panes
- **Custom CSS**: Gradients, shadows and transitions
- **Cards & Stats**: Elegant visual components

### Functionalities

#### 🔐 Authentication System
- Login with validation
- Session management
- Secure logout
- Default credentials: `admin` / `admin123`

#### 📊 Dashboard
- Total registered users
- Total products in stock
- Total inventory value
- Low stock products
- Automatic update via computed properties

#### 👥 User Management
- List all users
- Status display (active/inactive)
- Data from SQLite
- Scroll interface for long lists

#### 📦 Product Management
- Product listing with details
- Price, stock and category
- Search and filtering (prepared)
- Complete CRUD (structure ready)

#### ⚙️ Settings
- Logged in user display
- Logout option
- User profile (extensible)

## 📁 Project Structure

```
spa_app/
├── assets/
│   └── style.css              # Complete modern CSS
├── database/
│   └── app.db                 # SQLite (auto-generated)
├── include/
│   ├── database.h             # Database API
│   ├── app_state.h            # Global state management
│   └── pages.h                # Page declarations
├── src/
│   ├── main.c                 # Main application
│   ├── database.c             # SQLite implementation
│   ├── app_state.c            # Application state
│   ├── page_login.c           # Login page
│   ├── page_dashboard.c       # Dashboard with stats
│   ├── page_users.c           # User management
│   ├── page_products.c        # Product management
│   └── page_settings.c        # Settings
├── CMakeLists.txt             # Build configuration
└── README.md                  # This file
```

## 🚀 Compilation

### Prerequisites

- GTK4
- SQLite3
- CMake 3.10+
- EasyGTK compiled (in parent folder)

### Build

```bash
# A partir do diretório spa_app/
mkdir build && cd build
cmake ..
make -j4
```

### Run

```bash
# From the spa_app/build/ directory
./spa_dashboard
```

**Note**: Always run from the build directory so that relative paths work correctly.

## 🔑 Default Credentials

### Administrator
- **Username**: `admin`
- **Password**: `admin123`

### Demo Users
- **Username**: `john` | **Password**: `password`
- **Username**: `jane` | **Password**: `password`

## 🎨 Color Themes

### Main Palette (Catppuccin Mocha)
- **Background**: `#1e1e2e` (dark)
- **Surface**: `#313244` (medium)
- **Accent**: `#89b4fa` (blue)
- **Text**: `#cdd6f4` (light)
- **Subtext**: `#7f849c` (muted)
- **Success**: `#a6e3a1` (green)
- **Danger**: `#f38ba8` (red)

### Main CSS Classes
- `.sidebar` - Sidebar navigation
- `.main-content` - Main content area
- `.stat-card` - Statistics cards
- `.nav-button` - Navigation buttons
- `.btn-primary` - Primary button
- `.card` - Generic card

## 🏗️ Architecture

### Global State (AppState)
```c
typedef struct {
    EgViewModel *view_model;      // MVVM ViewModel
    EgModel *current_user_model;  // Logged user model
    User *logged_user;            // User data
    AppPage current_page;         // Current page
    EgStack *main_stack;          // Login/app stack
    EgStack *content_stack;       // Content stack
    bool is_authenticated;        // Authentication status
} AppState;
```

### Navigation
```c
app_state_navigate_to(PAGE_DASHBOARD);
app_state_navigate_to(PAGE_USERS);
app_state_navigate_to(PAGE_PRODUCTS);
app_state_navigate_to(PAGE_SETTINGS);
```

### Data Binding
```c
// Label updates automatically when property changes
eg_bind(eg_label_as_widget(label), view_model, "total_users");

// Entry with two-way binding
eg_bind(eg_entry_as_widget(entry), view_model, "username");

// Button executes command automatically
eg_bind_cmd(eg_button_as_widget(button), view_model, "logout");
```

## 📊 Database

### Table: users
```sql
- id (INTEGER PRIMARY KEY)
- username (TEXT UNIQUE)
- email (TEXT)
- password_hash (TEXT)
- is_active (BOOLEAN)
- created_at (TIMESTAMP)
```

### Table: products
```sql
- id (INTEGER PRIMARY KEY)
- name (TEXT)
- description (TEXT)
- price (REAL)
- stock (INTEGER)
- category (TEXT)
- created_at (TIMESTAMP)
```

## 🔧 Possible Extensions

### Implemented Features
- [x] Complete CRUD for users (create, edit, delete)
- [x] Complete CRUD for products
- [x] Real-time search system
- [x] User password change
- [x] Light/dark theme toggle

### Features to Add
- [ ] List pagination
- [ ] Advanced form validations
- [ ] Product image upload
- [ ] Reports and charts
- [ ] Notification system
- [ ] Action history (audit log)
- [ ] Multi-tenancy (multiple organizations)

### Technical Improvements
- [ ] Real password encryption (bcrypt/argon2)
- [ ] JWT tokens for sessions
- [ ] Database migrations
- [ ] Unit tests
- [ ] Internationalization (i18n)
- [ ] Automatic DB backup

## 📝 License

MIT License - Part of the EasyGTK project

## 🙏 Credits

- **EasyGTK**: GTK4 wrapper library
- **GTK4**: Graphical interface toolkit
- **SQLite**: Database engine
- **Catppuccin**: Color palette
