
cmake_minimum_required(VERSION 3.24.0)
project(eduPro VERSION 0.1 LANGUAGES CXX)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 COMPONENTS Widgets Network REQUIRED) # Qt COMPONENTS

include_directories(D:/javaAndApp/vcpkg/installed/x64-windows/include)

file(GLOB SRCS  *.cpp *.h)
file(GLOB_RECURSE WNDSRCS wnd/*.cpp wnd/*.h)
file(GLOB CORES  core/*.cpp core/*.h)
file(GLOB OAIS  openai/*.cpp openai/*.h)
add_executable(${PROJECT_NAME}
    # WIN32   #加上则不带命令行调试窗口
    ${SRCS}
    ${CORES}
    ${WNDSRCS}
    ${OAIS}
)

#让debug或release模式生成的bin文件在一个目录中
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin
)

target_link_directories(${PROJECT_NAME} PRIVATE D:/javaAndApp/vcpkg/installed/x64-windows/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets  Qt6::Network
    tesseract53.lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()