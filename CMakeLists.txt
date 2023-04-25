cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME TH-AB-INFO_IV)

project(${PROJECT_NAME})

#################### OUTPUT DIRECTORY ####################

### set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

#################### SOURCE FILES ####################

### set source files
set(SOURCE_FILES 
    src/main.cpp                            ### main file

    src/py_extention/ExtentionManager.cpp   ### python extention manager

    src/py_extention/PyRunner.cpp           ### python runner
    src/py_extention/PyRunnerFunctions.cpp

    src/py_extention/websocket/SocketClient.cpp  ### socket client
    src/py_extention/websocket/MultiSocket.cpp  ### socket server

    src/vulkan_base/vulkan_device.cpp       ### vulkan
)

####################  EXECUTABLE  ####################

### add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


#################### LIBRARIES ####################

### include imgui
set(IMGUI_FILES libs/imgui/imgui.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp libs/imgui/backends/imgui_impl_sdl2.cpp libs/imgui/backends/imgui_impl_vulkan.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE libs/imgui libs/imgui/backends)

### include SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

### include Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})

### include Python
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
target_link_libraries(${PROJECT_NAME} ${Python3_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Python3_INCLUDE_DIRS})

### include socket (dll)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} ws2_32)
else()
    target_link_libraries(${PROJECT_NAME} socket)
endif()

#################### COMPILE OPTIONS ####################

set(CMAKE_EXE_LINKER_FLAGS "-static")

#################### POST BUILD ####################

### copy Python dll
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Python3_RUNTIME_LIBRARY_DIRS}/python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}.dll ${PROJECT_SOURCE_DIR}/bin)
endif()

### copy Python scripts
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/src/py_extention/scripts/ ${PROJECT_SOURCE_DIR}/bin/scripts)


