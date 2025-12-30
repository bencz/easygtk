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

# EasyGTK - Try to find in parent project first, fallback to system
set(EASYGTK_PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
if(EXISTS "${EASYGTK_PARENT_DIR}/include/easygtk/easygtk.h" AND EXISTS "${EASYGTK_PARENT_DIR}/build/libeasygtk.so")
    # Found in parent project (development mode)
    set(EASYGTK_FOUND TRUE)
    set(EASYGTK_INCLUDE_DIRS "${EASYGTK_PARENT_DIR}/include")
    set(EASYGTK_LIBRARY_DIRS "${EASYGTK_PARENT_DIR}/build")
    set(EASYGTK_LIBRARIES "easygtk")
    message(STATUS "EasyGTK found in parent project: ${EASYGTK_PARENT_DIR}")
else()
    # Try pkg-config
    pkg_check_modules(EASYGTK easygtk)
    if(NOT EASYGTK_FOUND)
        # Manual search in system paths
        find_path(EASYGTK_INCLUDE_DIR easygtk/easygtk.h)
        find_library(EASYGTK_LIBRARY easygtk)
        if(EASYGTK_INCLUDE_DIR AND EASYGTK_LIBRARY)
            set(EASYGTK_FOUND TRUE)
            set(EASYGTK_INCLUDE_DIRS ${EASYGTK_INCLUDE_DIR})
            set(EASYGTK_LIBRARIES ${EASYGTK_LIBRARY})
            message(STATUS "EasyGTK found in system: ${EASYGTK_LIBRARY}")
        else()
            message(FATAL_ERROR "EasyGTK not found. Build the main project first or install easygtk.")
        endif()
    else()
        message(STATUS "EasyGTK found via pkg-config")
    endif()
endif()

include_directories(${EASYGTK_INCLUDE_DIRS})
if(EASYGTK_LIBRARY_DIRS)
    link_directories(${EASYGTK_LIBRARY_DIRS})
endif()

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
    ${EASYGTK_LIBRARIES}
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
