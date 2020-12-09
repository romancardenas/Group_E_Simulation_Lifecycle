include_directories(${simulation_lifecycle_SOURCE_DIR}/include)
include_directories(${simulation_lifecycle_SOURCE_DIR}/third_party/cJSON)
include_directories(${simulation_lifecycle_SOURCE_DIR}/third_party/Unity/src)

# Unit tests
FILE(GLOB TestSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./test_*.c)
foreach (testSrc ${TestSources})
    get_filename_component(testName ${testSrc} NAME_WE)
    add_executable(${testName} ${testSrc})
    target_link_libraries(${testName} cjson unity sim_lifecycle_lib)
    add_test(NAME ${testName} COMMAND ${testName} WORKING_DIRECTORY ${simulation_lifecycle_SOURCE_DIR}/bin)
endforeach (testSrc)
