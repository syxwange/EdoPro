cmake_minimum_required(VERSION 3.10.0)



aux_source_directory(. SRC)

add_executable(EduPro ${SRC})

#让debug或release模式生成的bin文件在一个目录中
set_target_properties(EduPro PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin
)

