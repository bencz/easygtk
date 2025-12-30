cmake_minimum_required(VERSION 3.10)
project(spa_dashboard VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Warnings
add_compile_options(-Wall -Wextra -Werror)

# Find dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Try to find SQLite3 using pkg-config first, fallback to manual search
pkg_check_modules(SQLITE3 sqlite3)
if(NOT SQLITE3_FOUND)
    # Manual search
    find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
    find_library(SQLITE3_LIBRARY sqlite3)
    if(SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY)
        set(SQLITE3_FOUND TRUE)
        set(SQLITE3_INCLUDE_DIRS ${SQLITE3_INCLUDE_DIR})
        set(SQLITE3_LIBRARIES ${SQLITE3_LIBRARY})
    else()
        message(FATAL_ERROR "SQLite3 not found. Please install sqlite3-devel or libsqlite3-dev")
    endif()
endif()

# EasyGTK
set(EASYGTK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
include_directories(${EASYGTK_DIR}/include)
link_directories(${EASYGTK_DIR}/build)

# Sources
set(SOURCES
    src/main.c
    src/database.c
    src/app_state.c
    src/page_login.c
    src/page_dashboard.c
    src/page_users.c
    src/page_products.c
    src/page_settings.c
)

# Executable
add_executable(spa_dashboard ${SOURCES})

target_include_directories(spa_dashboard PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GTK4_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
)

target_link_libraries(spa_dashboard PRIVATE
    easygtk
    ${GTK4_LIBRARIES}
    ${SQLITE3_LIBRARIES}
)

target_compile_options(spa_dashboard PRIVATE ${GTK4_CFLAGS_OTHER})

# Copy assets to build directory
add_custom_command(TARGET spa_dashboard POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory"
)

# Create database directory
add_custom_command(TARGET spa_dashboard POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_BINARY_DIR}/database
    COMMENT "Creating database directory"
)
