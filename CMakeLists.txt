cmake_minimum_required(VERSION 3.5)

# Definitions
set(PROJECT_NAME simulation_lifecycle)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11") 

link_directories(${PROJECT_SOURCE_DIR}/third_party/cJSON/lib)

# Include paths
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/third_party/cJSON/include)

# Assign the include directories
include_directories(${INCLUDE_DIRS})

# Collect source files
AUX_SOURCE_DIRECTORY(src/simulation_lifecycle SOURCE_FILES)
AUX_SOURCE_DIRECTORY(src/simulation_lifecycle/utils UTILS_SOURCES)


# Add executable from src/main.c file
# add_executable(simulation_lifecycle src/main.c ${SOURCE_FILES} ${PROJECT_SOURCE_DIR}/third_party/cJSON/cJSON.c)
add_executable(simulation_lifecycle src/main.c ${SOURCE_FILES} ${UTILS_SOURCES} ${PROJECT_SOURCE_DIR}/third_party/cJSON/cJSON.c)
# target_link_libraries(simulation_lifecycle libcjson.dll.a)
