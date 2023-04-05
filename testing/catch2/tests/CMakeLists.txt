enable_testing()
find_package(Catch2 3 REQUIRED)
add_executable(tests)

add_subdirectory(adapters)
add_subdirectory(core)

target_include_directories(tests 
  PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/src
  )

target_link_libraries(tests
  PRIVATE
    Catch2::Catch2WithMain
  )


include(CTest)
include(Catch)
catch_discover_tests(tests)
