cmake_minimum_required(VERSION 3.5)

# Definitions
set(PROJECT_NAME simulation_lifecycle)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

# Include paths
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
set(CJSON_INCLUDE ${CJSON_INCLUDE} ${PROJECT_SOURCE_DIR}/third_party/cJSON)
set (UNITY_INCLUDE ${UNITY_INCLUDE} ${PROJECT_SOURCE_DIR}/third_party/Unity/src)

# Assign the include directories
include_directories(${INCLUDE_DIRS} ${CJSON_INCLUDE} ${UNITY_INCLUDE})

# Collect source files
AUX_SOURCE_DIRECTORY(src/simulation_lifecycle SOURCE_FILES)
AUX_SOURCE_DIRECTORY(src/simulation_lifecycle/utils UTILS_SOURCES)


set (CJSON_SRC ${CJSON_SRC} ${PROJECT_SOURCE_DIR}/third_party/cJSON/cJSON.c)
set (UNITY_SRC ${UNITY_SRC} ${PROJECT_SOURCE_DIR}/third_party/Unity/src/unity.c)

# Add executable from src/main.c file
add_executable(simulation_lifecycle src/main.c ${SOURCE_FILES} ${UTILS_SOURCES} ${CJSON_SRC})

# Unit tests
FILE(GLOB TestSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/test_*.c)
foreach (testSrc ${TestSources})
    get_filename_component(testName ${testSrc} NAME_WE)
    add_executable(${testName} ${testSrc}  ${SOURCE_FILES} ${UTILS_SOURCES} ${CJSON_SRC} ${UNITY_SRC})
endforeach (testSrc)
