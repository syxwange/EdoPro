cmake_minimum_required(VERSION 3.26.0)
project(msTTS VERSION 0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(../include/speechSdk)


file(GLOB SRCS  *.cpp *.h )

add_executable(${PROJECT_NAME}
    ${SRCS}
)

target_link_directories(${PROJECT_NAME} PRIVATE ../lib64)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Microsoft.CognitiveServices.Speech.core.lib
    )

#让debug或release模式生成的bin文件在一个目录中
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin
)

