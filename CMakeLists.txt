cmake_minimum_required(VERSION 3.16)
project(Snaarp VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Add your source files
set(SOURCES
    main.cpp
    CreateAndPinSnaarpFolder.cpp
    mainwindow.cpp
    usbcontroller.cpp  # Added USB controller source
)

set(HEADERS
    CreateAndPinSnaarpFolder.h
    mainwindow.h
    usbcontroller.h    # Added USB controller header
)

# Add UI files if you have any
set(UI_FILES
    mainwindow.ui
)

# Create the executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Windows-specific configurations
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        shell32
        ole32
        uuid
        shlwapi
        advapi32
        setupapi    # Added for USB control
        cfgmgr32   # Added for USB control
    )

    # MinGW-specific flags for Windows subsystem
    if(MINGW)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
            LINK_FLAGS "-mwindows"
        )
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
            LINK_FLAGS "/SUBSYSTEM:WINDOWS"
        )
    endif()
endif()

# Enable automoc for Qt
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

# Add assets copying if needed
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    )
endif()
