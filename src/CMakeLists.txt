file(GLOB MAIN ./main.c)
file(GLOB SOURCES ./simulation_lifecycle/*.c ./simulation_lifecycle/*/*.c)

include_directories(${simulation_lifecycle_SOURCE_DIR}/include)
include_directories(${simulation_lifecycle_SOURCE_DIR}/third_party/cJSON)

# Add library with all the simulation lifecycle functions
add_library(sim_lifecycle_lib ${SOURCES})
target_link_libraries(sim_lifecycle_lib m cjson)      # library needs the math and cJSON libraries

# Add executable from src/main.c file
add_executable(sim_lifecycle ${MAIN})
target_link_libraries(sim_lifecycle cjson sim_lifecycle_lib)  # executable needs the cJSON and simulation lifecycle libraries
