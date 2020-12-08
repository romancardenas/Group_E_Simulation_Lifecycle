file(GLOB MAIN ./main.c)
file(GLOB SOURCES ./simulation_lifecycle/*.c ./simulation_lifecycle/*/*.c)

include_directories(${simulation_lifecycle_SOURCE_DIR}/include)
include_directories(${simulation_lifecycle_SOURCE_DIR}/third_party/cJSON)

# Add executable from src/main.c file
add_executable(sim_lifecycle ${MAIN} ${SOURCES})
target_link_libraries(sim_lifecycle cjson)

add_library(sim_lifecycle_lib ${SOURCES})
target_link_libraries(sim_lifecycle_lib cjson)
