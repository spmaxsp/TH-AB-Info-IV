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

    #src/py_extention/PyRunner.cpp           ### python runner
    #src/py_extention/PyRunnerFunctions.cpp
    src/py_extention/PyShellExec.cpp

    src/py_extention/websocket/SocketClient.cpp  ### socket client

    src/py_extention/modules/ModuleTemplate/PyModule.cpp

    src/py_extention/modules/ModuleTemplate/proto/PyModuleProt.pb.cc  ### protobuf
    #src/py_extention/modules/Shimmersensor/proto/ShimmersensorProt.pb.cc

    src/vulkan_base/vulkan_device.cpp       ### vulkan
    src/vulkan_base/vulkan_swapchain.cpp
    src/vulkan_base/vulkan_renderpass.cpp
    src/vulkan_base/vulkan_cmdbuffer.cpp
    src/vulkan_base/vulkan_pipeline.cpp
    src/vulkan_base/vulkan_utils.cpp
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

### include socket (dll)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} ws2_32)
else()
    target_link_libraries(${PROJECT_NAME} socket)
endif()

### include protobuf
find_package(Protobuf REQUIRED)
target_link_libraries(${PROJECT_NAME} ${Protobuf_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Protobuf_INCLUDE_DIRS})

### include opencv2
find_package(OpenCV REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})

### Vulkan Memory Allocator (header only)
target_include_directories(${PROJECT_NAME} PRIVATE libs/vma)

### Logger (header only)
target_include_directories(${PROJECT_NAME} PRIVATE libs/bslogger)

#################### COMPILE OPTIONS ####################

set(CMAKE_EXE_LINKER_FLAGS "-static")

#################### SHADERS ####################

if (UNIX)
add_custom_target(build_shaders ALL
    COMMAND "${PROJECT_SOURCE_DIR}/shaders/compile.sh"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/shaders"
)
endif(UNIX)

if (WIN32)
add_custom_target(build_shaders ALL
    COMMAND "${PROJECT_SOURCE_DIR}/shaders/compile.bat"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/shaders"
)
endif(WIN32)

add_dependencies(${PROJECT_NAME} build_shaders)

#################### COPY SHADERS ####################

### Copy shaders
file(GLOB_RECURSE SHADER_FILES ${PROJECT_SOURCE_DIR}/shaders/*.spv)
### Display files
message(STATUS "Shader files: ${SHADER_FILES}")

### Copy files
add_custom_target(copy_shaders ALL
    COMMENT "Copying shaders to shaders directory"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER_FILES} ${PROJECT_SOURCE_DIR}/bin/shaders
    DEPENDS build_shaders)

### Add dependencies
add_dependencies(${PROJECT_NAME} copy_shaders)

#################### POST BUILD ####################

### copy Python scripts
file(GLOB_RECURSE PY_FILES ${PROJECT_SOURCE_DIR}/src/py_extention/modules/*.py)
### display files
message(STATUS "Python files: ${PY_FILES}")
### copy files
add_custom_target(copy_py ALL
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PY_FILES} ${PROJECT_SOURCE_DIR}/bin/scripts
                    COMMENT "Copying Python files to bin directory" )

add_dependencies(${PROJECT_NAME} copy_py)

