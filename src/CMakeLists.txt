set(${PROJECT_NAME}_SOURCES
    median_filter_thread.cpp
    zoom_graphics_view.cpp
    odd_spin_box.cpp
    mainwindow.cpp
    main.cpp
)

include_directories(${ITK_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

qt5_use_modules(${PROJECT_NAME} LINK_PUBLIC Widgets)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${ITK_LIBRARIES})