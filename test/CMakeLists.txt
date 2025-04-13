fetch_package("github:holepunchto/libjs")

list(APPEND tests
  create-function-receiver
  create-function-return-bool
  create-function-return-double
  create-function-return-int32
  create-function-return-int64
  create-function-return-string
  create-function-return-uint8array
  create-function-return-uint32
  create-function-return-void
  create-function-return-void-arg-bool
  create-function-return-void-arg-double
  create-function-return-void-arg-int32
  create-function-return-void-arg-int64
  create-function-return-void-arg-uint8array
  create-function-return-void-arg-uint32
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
