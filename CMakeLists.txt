cmake_minimum_required(VERSION 3.10)
project(easygtk VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Warnings como erros (sem -pedantic que conflita com headers GTK4)
add_compile_options(-Wall -Wextra -Werror)

# Find GTK4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

# Library sources
set(EASYGTK_SOURCES
    src/core/app.c
    src/core/widget.c
    src/core/memory.c
    src/core/signal.c
    src/core/property.c
    src/core/timer.c
    src/core/css.c
    src/core/model.c
    src/core/viewmodel.c
    src/core/binding.c
    src/core/events.c
    src/core/clipboard.c
    src/core/dnd.c
    src/core/log.c
    src/core/cast.c
    src/core/refcount.c
    src/core/pool.c
    src/core/container.c
    src/core/validation.c
    src/internal/widget_helpers.c
    src/internal/event_helpers.c
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
    src/widgets/spin_button.c
    src/widgets/scale.c
    src/widgets/text_view.c
    src/widgets/stack.c
    src/widgets/notebook.c
    src/widgets/paned.c
    src/widgets/frame.c
    src/widgets/expander.c
    src/widgets/image.c
    src/widgets/picture.c
    src/widgets/separator.c
    src/widgets/spinner.c
    src/widgets/level_bar.c
    src/widgets/header_bar.c
    src/widgets/popover_menu.c
    src/widgets/menu_button.c
    src/widgets/list_view.c
    src/widgets/column_view.c
    src/widgets/file_chooser.c
    src/widgets/color_chooser.c
    src/widgets/about_dialog.c
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
)

target_link_libraries(easygtk PUBLIC PkgConfig::GTK4)

# Static library
add_library(easygtk_static STATIC ${EASYGTK_SOURCES})

target_include_directories(easygtk_static
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(easygtk_static PUBLIC PkgConfig::GTK4)

# Examples
option(BUILD_EXAMPLES "Build examples" ON)

if(BUILD_EXAMPLES)
    add_executable(hello_world examples/hello_world.c)
    target_link_libraries(hello_world PRIVATE easygtk)

    add_executable(form_example examples/form_example.c)
    target_link_libraries(form_example PRIVATE easygtk)

    add_executable(counter_example examples/counter_example.c)
    target_link_libraries(counter_example PRIVATE easygtk)

    add_executable(mvvm_example examples/mvvm_example.c)
    target_link_libraries(mvvm_example PRIVATE easygtk)

    add_executable(widgets_example examples/widgets_example.c)
    target_link_libraries(widgets_example PRIVATE easygtk)

    add_executable(dialogs_example examples/dialogs_example.c)
    target_link_libraries(dialogs_example PRIVATE easygtk)

    add_executable(events_example examples/events_example.c)
    target_link_libraries(events_example PRIVATE easygtk)

    add_executable(dnd_example examples/dnd_example.c)
    target_link_libraries(dnd_example PRIVATE easygtk)

    add_executable(input_widgets_example examples/input_widgets_example.c)
    target_link_libraries(input_widgets_example PRIVATE easygtk)

    add_executable(containers_example examples/containers_example.c)
    target_link_libraries(containers_example PRIVATE easygtk)

    add_executable(new_widgets_example examples/new_widgets_example.c)
    target_link_libraries(new_widgets_example PRIVATE easygtk)

    add_executable(spa_example examples/spa_example.c)
    target_link_libraries(spa_example PRIVATE easygtk)

    add_executable(menu_example examples/menu_example.c)
    target_link_libraries(menu_example PRIVATE easygtk)

    add_executable(menubar_example examples/menubar_example.c)
    target_link_libraries(menubar_example PRIVATE easygtk)

    add_executable(listview_example examples/listview_example.c)
    target_link_libraries(listview_example PRIVATE easygtk)

    add_executable(dashboard_example examples/dashboard_example.c)
    target_link_libraries(dashboard_example PRIVATE easygtk)

    add_executable(validation_example examples/validation_example.c)
    target_link_libraries(validation_example PRIVATE easygtk)

    add_executable(container_api_example examples/container_api_example.c)
    target_link_libraries(container_api_example PRIVATE easygtk)
endif()

# Install
install(TARGETS easygtk easygtk_static
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/easygtk DESTINATION include)