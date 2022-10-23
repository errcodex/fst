cmake_minimum_required(VERSION 3.10)

project(fst LANGUAGES CXX)

aux_source_directory("src" _SRC)
aux_source_directory("src/Device" _SRC_Device)
aux_source_directory("src/MediaData" _SRC_MediaData)
aux_source_directory("src/Sink" _SRC_Sink)
aux_source_directory("src/Source" _SRC_Source)
aux_source_directory("src/Stream" _SRC_Stream)
add_executable(${PROJECT_NAME} ${_SRC} ${_SRC_Device} ${_SRC_MediaData} ${_SRC_Sink} ${_SRC_Source} ${_SRC_Stream})

add_subdirectory("external/libutils")

target_include_directories(${PROJECT_NAME} PRIVATE "external/include")
target_include_directories(${PROJECT_NAME} PRIVATE "src")
target_link_directories(${PROJECT_NAME} PRIVATE "external/lib")
target_link_libraries(${PROJECT_NAME} PRIVATE  "librtmpd.lib" libutils::libutils_all)

find_package(FFMPEG REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})