cmake_minimum_required(VERSION 3.10)
project(easygtk VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Warnings como erros (sem -pedantic que conflita com headers GTK4)
add_compile_options(-Wall -Wextra -Werror)

# Find GTK4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Library sources
set(EASYGTK_SOURCES
    src/core/app.c
    src/core/widget.c
    src/core/memory.c
    src/core/signal.c
    src/core/property.c
    src/core/timer.c
    src/core/css.c
    src/core/viewmodel.c
    src/widgets/window.c
    src/widgets/button.c
    src/widgets/label.c
    src/widgets/entry.c
    src/widgets/box.c
    src/widgets/grid.c
    src/widgets/check_button.c
    src/widgets/switch.c
    src/widgets/progress_bar.c
    src/widgets/combo_box.c
    src/widgets/scrolled_window.c
    src/widgets/dialog.c
)

# Create shared library
add_library(easygtk SHARED ${EASYGTK_SOURCES})

target_include_directories(easygtk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${GTK4_INCLUDE_DIRS}
)

target_link_libraries(easygtk PRIVATE ${GTK4_LIBRARIES})
target_compile_options(easygtk PRIVATE ${GTK4_CFLAGS_OTHER})

# Static library
add_library(easygtk_static STATIC ${EASYGTK_SOURCES})

target_include_directories(easygtk_static
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${GTK4_INCLUDE_DIRS}
)

target_link_libraries(easygtk_static PRIVATE ${GTK4_LIBRARIES})
target_compile_options(easygtk_static PRIVATE ${GTK4_CFLAGS_OTHER})

# Examples
option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_EXAMPLES)
    add_executable(hello_world examples/hello_world.c)
    target_link_libraries(hello_world PRIVATE easygtk ${GTK4_LIBRARIES})
    target_include_directories(hello_world PRIVATE ${GTK4_INCLUDE_DIRS})

    add_executable(form_example examples/form_example.c)
    target_link_libraries(form_example PRIVATE easygtk ${GTK4_LIBRARIES})
    target_include_directories(form_example PRIVATE ${GTK4_INCLUDE_DIRS})

    add_executable(counter_example examples/counter_example.c)
    target_link_libraries(counter_example PRIVATE easygtk ${GTK4_LIBRARIES})
    target_include_directories(counter_example PRIVATE ${GTK4_INCLUDE_DIRS})

    add_executable(mvvm_example examples/mvvm_example.c)
    target_link_libraries(mvvm_example PRIVATE easygtk ${GTK4_LIBRARIES})
    target_include_directories(mvvm_example PRIVATE ${GTK4_INCLUDE_DIRS})

    add_executable(widgets_example examples/widgets_example.c)
    target_link_libraries(widgets_example PRIVATE easygtk ${GTK4_LIBRARIES})
    target_include_directories(widgets_example PRIVATE ${GTK4_INCLUDE_DIRS})

    add_executable(dialogs_example examples/dialogs_example.c)
    target_link_libraries(dialogs_example PRIVATE easygtk ${GTK4_LIBRARIES})
    target_include_directories(dialogs_example PRIVATE ${GTK4_INCLUDE_DIRS})
endif()

# Install
install(TARGETS easygtk easygtk_static
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/easygtk DESTINATION include)
