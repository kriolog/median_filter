set(${PROJECT_NAME}_SOURCES
    median_filter_thread.h
    median_filter_thread.cpp
    zoom_graphics_view.cpp
    mainwindow.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

qt5_use_modules(${PROJECT_NAME} LINK_PUBLIC Widgets)
# target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ...)