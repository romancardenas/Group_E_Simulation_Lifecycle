cmake_minimum_required(VERSION 3.5)

# Definitions
set(PROJECT_NAME simulation_lifecycle)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

# Include libraries
find_package(GDAL REQUIRED)  # For Geospatial analysis
include_directories(${GDAL_INCLUDE_DIRS})

# Include paths
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include/simulation_lifecycle)

# Assign the include directories
include_directories(${INCLUDE_DIRS})

# Find library sources
file(GLOB_RECURSE LIB_SRC_FILES include/*/*.c)

# Build static/shared library
add_library(${PROJECT_NAME}_LIB STATIC ${LIB_SRC_FILES})

# Install library
install(TARGETS ${PROJECT_NAME}_LIB DESTINATION lib)

# Install library headers
file(GLOB_RECURSE LIB_INC_FILES include/*/*.h)
install(FILES ${LIB_INC_FILES} DESTINATION include)

set(SOURCE_FILES src/main.c)
add_executable(simulation_lifecycle ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_LIB)
target_link_libraries(${PROJECT_NAME} ${GDAL_LIBRARY})
