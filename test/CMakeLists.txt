fetch_package("github:holepunchto/libjs")

list(APPEND tests
  create-function-pointer
  create-function-receiver
  create-function-receiver-no-env
  create-function-return-array-int32
  create-function-return-bool
  create-function-return-double
  create-function-return-int32
  create-function-return-int64
  create-function-return-pointer
  create-function-return-string
  create-function-return-string-literal
  create-function-return-uint8array
  create-function-return-uint16array
  create-function-return-uint32
  create-function-return-vector-int32
  create-function-return-void
  create-function-return-void-arg-array-int32
  create-function-return-void-arg-bool
  create-function-return-void-arg-double
  create-function-return-void-arg-int32
  create-function-return-void-arg-int64
  create-function-return-void-arg-pointer
  create-function-return-void-arg-string
  create-function-return-void-arg-string-literal
  create-function-return-void-arg-uint8array
  create-function-return-void-arg-uint16array
  create-function-return-void-arg-uint32
  create-function-return-void-arg-vector-int32
  create-reference-get-value
  create-reference-get-value-optional
  create-reference-move-assign
  create-typedarray-data-cast
  create-typedarray-get-info
  create-typedarray-get-info-copy
  create-typedarray-get-info-move-assign
  get-typedarray-info-data-cast
  set-get-property-literal-function-pointer
  set-get-property-literal-int32
  set-get-property-literal-uint32
)

foreach(test IN LISTS tests)
  add_executable(${test} ${test}.cc)

  set_target_properties(
    ${test}
    PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 20
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
