fetch_package("github:holepunchto/libjs")

list(APPEND tests
  create-typed-function-return-uint8array
  create-typed-function-return-void
  create-typed-function-return-void-arg-uint8array
)

foreach(test IN LISTS tests)
  add_executable(${test} ${test}.cc)

  set_target_properties(
    ${test}
    PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 23
  )

  target_link_libraries(
    ${test}
    PRIVATE
      js_shared
      jstl
  )

  add_test(
    NAME ${test}
    COMMAND ${test}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )

  set_tests_properties(
    ${test}
    PROPERTIES
    TIMEOUT 30
  )
endforeach()
