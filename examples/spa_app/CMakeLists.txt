cmake_minimum_required(VERSION 3.10)
project(spa_dashboard VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# Find dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

# Try to find SQLite3 using pkg-config first, fallback to manual search
pkg_check_modules(SQLITE3 IMPORTED_TARGET sqlite3)
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

# Check for library with platform-specific extension
if(WIN32)
    set(EASYGTK_SHARED_LIB_NAME "easygtk.dll")
    set(EASYGTK_IMPORT_LIB_NAME "easygtk.lib")
    # Also check for MinGW naming convention
    set(EASYGTK_SHARED_LIB_NAME_ALT "libeasygtk.dll")
    set(EASYGTK_IMPORT_LIB_NAME_ALT "libeasygtk.dll.a")
elseif(APPLE)
    set(EASYGTK_SHARED_LIB_NAME "libeasygtk.dylib")
else()
    set(EASYGTK_SHARED_LIB_NAME "libeasygtk.so")
endif()

# Function to find EasyGTK library in a directory
function(find_easygtk_in_dir DIR RESULT_VAR)
    set(${RESULT_VAR} "" PARENT_SCOPE)
    
    if(WIN32)
        # Check MSVC naming
        if(EXISTS "${DIR}/${EASYGTK_SHARED_LIB_NAME}")
            set(${RESULT_VAR} "${DIR}/${EASYGTK_SHARED_LIB_NAME}" PARENT_SCOPE)
            return()
        endif()
        # Check MinGW naming
        if(EXISTS "${DIR}/${EASYGTK_SHARED_LIB_NAME_ALT}")
            set(${RESULT_VAR} "${DIR}/${EASYGTK_SHARED_LIB_NAME_ALT}" PARENT_SCOPE)
            return()
        endif()
    else()
        if(EXISTS "${DIR}/${EASYGTK_SHARED_LIB_NAME}")
            set(${RESULT_VAR} "${DIR}/${EASYGTK_SHARED_LIB_NAME}" PARENT_SCOPE)
            return()
        endif()
    endif()
endfunction()

# Try to find EasyGTK in parent project
set(EASYGTK_FOUND FALSE)
if(EXISTS "${EASYGTK_PARENT_DIR}/include/easygtk/easygtk.h")
    find_easygtk_in_dir("${EASYGTK_PARENT_DIR}/build" EASYGTK_LIBRARY)
    
    # Also check common build subdirectories on Windows
    if(NOT EASYGTK_LIBRARY AND WIN32)
        find_easygtk_in_dir("${EASYGTK_PARENT_DIR}/build/Release" EASYGTK_LIBRARY)
        if(NOT EASYGTK_LIBRARY)
            find_easygtk_in_dir("${EASYGTK_PARENT_DIR}/build/Debug" EASYGTK_LIBRARY)
        endif()
    endif()
    
    if(EASYGTK_LIBRARY)
        set(EASYGTK_FOUND TRUE)
        set(EASYGTK_INCLUDE_DIRS "${EASYGTK_PARENT_DIR}/include")
        message(STATUS "EasyGTK found in parent project: ${EASYGTK_LIBRARY}")
        
        # Create imported target for consistency
        add_library(EasyGTK::easygtk SHARED IMPORTED)
        set_target_properties(EasyGTK::easygtk PROPERTIES
            IMPORTED_LOCATION "${EASYGTK_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${EASYGTK_INCLUDE_DIRS}"
        )
        
        # On Windows with MSVC, we need the import library
        if(WIN32 AND MSVC)
            get_filename_component(EASYGTK_LIB_DIR "${EASYGTK_LIBRARY}" DIRECTORY)
            if(EXISTS "${EASYGTK_LIB_DIR}/easygtk.lib")
                set_target_properties(EasyGTK::easygtk PROPERTIES
                    IMPORTED_IMPLIB "${EASYGTK_LIB_DIR}/easygtk.lib"
                )
            endif()
        endif()
    endif()
endif()

if(NOT EASYGTK_FOUND)
    # Try pkg-config
    pkg_check_modules(EASYGTK IMPORTED_TARGET easygtk)

    if(NOT EASYGTK_FOUND)
        # Manual search in system paths
        find_path(EASYGTK_INCLUDE_DIR easygtk/easygtk.h)
        find_library(EASYGTK_LIBRARY easygtk)

        if(EASYGTK_INCLUDE_DIR AND EASYGTK_LIBRARY)
            set(EASYGTK_FOUND TRUE)
            message(STATUS "EasyGTK found in system: ${EASYGTK_LIBRARY}")
            
            # Create imported target
            add_library(EasyGTK::easygtk SHARED IMPORTED)
            set_target_properties(EasyGTK::easygtk PROPERTIES
                IMPORTED_LOCATION "${EASYGTK_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${EASYGTK_INCLUDE_DIR}"
            )
        else()
            message(FATAL_ERROR "EasyGTK not found. Build the main project first or install easygtk.")
        endif()
    else()
        message(STATUS "EasyGTK found via pkg-config")
        # Create alias for pkg-config target
        add_library(EasyGTK::easygtk ALIAS PkgConfig::EASYGTK)
    endif()
endif()

# Sources
set(SOURCES
    src/main.c
    src/database.c
    src/app_state.c
    src/users_viewmodel.c
    src/page_login.c
    src/page_dashboard.c
    src/page_users.c
    src/page_products.c
    src/page_settings.c
)

# Executable
if(WIN32)
    # WIN32 flag creates a GUI application without console window
    # Remove it if you want to see console output for debugging
    add_executable(spa_dashboard ${SOURCES})
else()
    add_executable(spa_dashboard ${SOURCES})
endif()

target_include_directories(spa_dashboard PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link using imported targets (handles include dirs and library paths automatically)
target_link_libraries(spa_dashboard PRIVATE
    EasyGTK::easygtk
    PkgConfig::GTK4
)

# Link SQLite3 (use imported target if available, otherwise manual)
if(TARGET PkgConfig::SQLITE3)
    target_link_libraries(spa_dashboard PRIVATE PkgConfig::SQLITE3)
else()
    target_include_directories(spa_dashboard PRIVATE ${SQLITE3_INCLUDE_DIRS})
    target_link_libraries(spa_dashboard PRIVATE ${SQLITE3_LIBRARIES})
endif()

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

# On Windows, copy DLLs to output directory for easier execution
if(WIN32)
    # Copy EasyGTK DLL
    if(EASYGTK_LIBRARY)
        add_custom_command(TARGET spa_dashboard POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${EASYGTK_LIBRARY}"
            $<TARGET_FILE_DIR:spa_dashboard>
            COMMENT "Copying EasyGTK DLL"
        )
    endif()
endif()