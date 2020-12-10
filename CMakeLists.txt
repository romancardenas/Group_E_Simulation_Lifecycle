CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# Definitions
SET(PROJECT_NAME simulation_lifecycle)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
PROJECT(${PROJECT_NAME})
SET(CMAKE_C_STANDARD 11)

# Create third-party libraries
ADD_LIBRARY(cjson ${PROJECT_SOURCE_DIR}/third_party/cJSON/cJSON.c)
ADD_LIBRARY(unity ${PROJECT_SOURCE_DIR}/third_party/Unity/src/unity.c)

# Simulation Lifecycle library and main routine
ADD_SUBDIRECTORY(src)

# Unit tests
include(CTest)
ENABLE_TESTING()
ADD_SUBDIRECTORY(test)
