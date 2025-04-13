#pragma once

#include <span>
#include <string>
#include <type_traits>
#include <utility>

#include <js.h>
#include <utf.h>

struct js_handle_t {
  js_value_t *value;

  js_handle_t() : value(nullptr) {}

  js_handle_t(js_value_t *value) : value(value) {}

  virtual ~js_handle_t() = default;
};

struct js_primitive_t : js_handle_t {
  js_primitive_t() : js_handle_t() {}

  js_primitive_t(js_value_t *value) : js_handle_t(value) {}
};

struct js_numeric_t : js_primitive_t {
  js_numeric_t() : js_primitive_t() {}

  js_numeric_t(js_value_t *value) : js_primitive_t(value) {}
};

struct js_number_t : js_numeric_t {
  js_number_t() : js_numeric_t() {}

  js_number_t(js_value_t *value) : js_numeric_t(value) {}
};

struct js_integer_t : js_number_t {
  js_integer_t() : js_number_t() {}

  js_integer_t(js_value_t *value) : js_number_t(value) {}
};

struct js_name_t : js_primitive_t {
  js_name_t() : js_primitive_t() {}

  js_name_t(js_value_t *value) : js_primitive_t(value) {}
};

struct js_symbol_t : js_name_t {
  js_symbol_t() : js_name_t() {}

  js_symbol_t(js_value_t *value) : js_name_t(value) {}
};

template <typename T>
struct js_string_t : js_name_t {
  js_string_t() : js_name_t() {}

  js_string_t(js_value_t *value) : js_name_t(value) {}
};

struct js_array_t : js_handle_t {
  js_array_t() : js_handle_t() {}

  js_array_t(js_value_t *value) : js_handle_t(value) {}
};

struct js_object_t : js_handle_t {
  js_object_t() : js_handle_t() {}

  js_object_t(js_value_t *value) : js_handle_t(value) {}
};

struct js_arraybuffer_t : js_handle_t {
  js_arraybuffer_t() : js_handle_t() {}

  js_arraybuffer_t(js_value_t *value) : js_handle_t(value) {}
};

template <typename T>
struct js_typedarray_t : js_handle_t {
  js_env_t *env;
  js_typedarray_view_t *view;

  js_typedarray_t() : js_handle_t(), env(nullptr), view(nullptr) {}

  js_typedarray_t(js_env_t *env, js_value_t *value) : js_handle_t(value), env(env), view(nullptr) {}

  ~js_typedarray_t() {
    if (view == nullptr) return;

    int err;
    err = js_release_typedarray_view(env, view);
    assert(err == 0);
  }
};

template <typename R, typename... A>
struct js_function_t : js_handle_t {
  js_function_t() : js_handle_t() {}

  js_function_t(js_value_t *value) : js_handle_t(value) {}
};

struct js_receiver_t : js_handle_t {
  js_receiver_t() : js_handle_t() {}

  js_receiver_t(js_value_t *value) : js_handle_t(value) {}
};

template <typename T>
struct js_type_container_t;

template <>
struct js_type_container_t<js_receiver_t> {
  using type = js_value_t *;

  static constexpr auto
  signature() {
    return js_object;
  }

  static auto
  unmarshall(js_env_t *, js_typed_callback_info_t *, js_value_t *value) {
    return js_receiver_t(value);
  }

  static auto
  unmarshall(js_env_t *, js_callback_info_t *, js_value_t *value) {
    return js_receiver_t(value);
  }
};

template <>
struct js_type_container_t<void> {
  using type = void;

  static constexpr auto
  signature() {
    return js_undefined;
  }

  static auto
  marshall(js_env_t *env, js_value_t *&result) {
    return js_get_undefined(env, &result);
  }
};

template <>
struct js_type_container_t<bool> {
  using type = bool;

  static constexpr auto
  signature() {
    return js_boolean;
  }

  static constexpr auto
  marshall(js_env_t *, bool value, bool &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  marshall(js_env_t *env, bool value, js_value_t *&result) {
    return js_get_boolean(env, value, &result);
  }

  static constexpr auto
  unmarshall(js_env_t *, bool value, bool &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  unmarshall(js_env_t *env, js_value_t *value, bool &result) {
    return js_get_value_bool(env, value, &result);
  }
};

template <>
struct js_type_container_t<int32_t> {
  using type = int32_t;

  static constexpr auto
  signature() {
    return js_int32;
  }

  static constexpr auto
  marshall(js_env_t *, int32_t value, int32_t &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  marshall(js_env_t *env, int32_t value, js_value_t *&result) {
    return js_create_int32(env, value, &result);
  }

  static constexpr auto
  unmarshall(js_env_t *, int32_t value, int32_t &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  unmarshall(js_env_t *env, js_value_t *value, int32_t &result) {
    return js_get_value_int32(env, value, &result);
  }
};

template <>
struct js_type_container_t<uint32_t> {
  using type = uint32_t;

  static constexpr auto
  signature() {
    return js_uint32;
  }

  static constexpr auto
  marshall(js_env_t *, uint32_t value, uint32_t &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  marshall(js_env_t *env, uint32_t value, js_value_t *&result) {
    return js_create_uint32(env, value, &result);
  }

  static constexpr auto
  unmarshall(js_env_t *env, uint32_t value, uint32_t &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  unmarshall(js_env_t *env, js_value_t *value, uint32_t &result) {
    return js_get_value_uint32(env, value, &result);
  }
};

template <>
struct js_type_container_t<int64_t> {
  using type = int64_t;

  static constexpr auto
  signature() {
    return js_int64;
  }

  static constexpr auto
  marshall(js_env_t *, int64_t value, int64_t &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  marshall(js_env_t *env, int64_t value, js_value_t *&result) {
    return js_create_int64(env, value, &result);
  }

  static constexpr auto
  unmarshall(js_env_t *, int64_t value, int64_t &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  unmarshall(js_env_t *env, js_value_t *value, int64_t &result) {
    return js_get_value_int64(env, value, &result);
  }
};

template <>
struct js_type_container_t<double> {
  using type = double;

  static constexpr auto
  signature() {
    return js_float64;
  }

  static constexpr auto
  marshall(js_env_t *, double value, double &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  marshall(js_env_t *env, double value, js_value_t *&result) {
    return js_create_double(env, value, &result);
  }

  static constexpr auto
  unmarshall(js_env_t *, double value, double &result) {
    result = value;

    return 0;
  }

  static constexpr auto
  unmarshall(js_env_t *env, js_value_t *value, double &result) {
    return js_get_value_double(env, value, &result);
  }
};

template <>
struct js_type_container_t<js_arraybuffer_t> {
  using type = js_value_t *;

  static constexpr auto
  signature() {
    return js_object;
  }

  static constexpr auto
  marshall(js_env_t *env, const js_arraybuffer_t &arraybuffer, js_value_t *&result) {
    result = arraybuffer.value;

    return 0;
  }

  static auto
  unmarshall(js_env_t *env, js_value_t *value, js_arraybuffer_t &result) {
    result = js_arraybuffer_t(value);

    return 0;
  }
};

template <typename T>
struct js_type_container_t<js_typedarray_t<T>> {
  using type = js_value_t *;

  static constexpr auto
  signature() {
    return js_object;
  }

  static auto
  marshall(js_env_t *env, const js_typedarray_t<T> &typedarray, js_value_t *&result) {
    result = typedarray.value;

    return 0;
  }

  static auto
  unmarshall(js_env_t *env, js_value_t *value, js_typedarray_t<T> &result) {
    result = js_typedarray_t<T>(env, value);

    return 0;
  }
};

template <typename T>
constexpr auto
js_marshall_typed_value(js_env_t *env, T value) {
  int err;

  typename js_type_container_t<T>::type result;
  err = js_type_container_t<T>::marshall(env, value, result);
  assert(err == 0);

  return result;
}

template <typename T>
constexpr auto
js_marshall_untyped_value(js_env_t *env, T value) {
  int err;

  js_value_t *result;
  err = js_type_container_t<T>::marshall(env, value, result);
  assert(err == 0);

  return result;
}

constexpr auto
js_marshall_untyped_value(js_env_t *env) {
  int err;

  js_value_t *result;
  err = js_type_container_t<void>::marshall(env, result);
  assert(err == 0);

  return result;
}

template <typename T>
constexpr auto
js_unmarshall_typed_value(js_env_t *env, typename js_type_container_t<T>::type value) {
  int err;

  T result;
  err = js_type_container_t<T>::unmarshall(env, value, result);
  assert(err == 0);

  return result;
}

template <typename T>
constexpr auto
js_unmarshall_untyped_value(js_env_t *env, js_value_t *value) {
  int err;

  T result;
  err = js_type_container_t<T>::unmarshall(env, value, result);
  assert(err == 0);

  return result;
}

template <auto fn, typename R, typename... A>
constexpr auto
js_typed_callback() {
  return +[](js_type_container_t<A>::type... args, js_typed_callback_info_t *info) -> js_type_container_t<R>::type {
    int err;

    js_env_t *env;
    err = js_get_typed_callback_info(info, &env, nullptr);
    assert(err == 0);

    if constexpr (std::is_same<R, void>()) {
      fn(env, js_unmarshall_typed_value<A>(env, args)...);
    } else {
      auto result = fn(env, js_unmarshall_typed_value<A>(env, args)...);

      return js_marshall_typed_value<R>(env, std::move(result));
    }
  };
}

template <auto fn, typename R, typename... A, size_t... I>
constexpr auto
js_untyped_callback(std::index_sequence<I...>) {
  return +[](js_env_t *env, js_callback_info_t *info) -> js_value_t * {
    int err;

    size_t argc = sizeof...(A);
    js_value_t *argv[sizeof...(A)];

    if constexpr (std::tuple_size<std::tuple<A...>>() > 0) {
      using head = std::tuple_element<0, std::tuple<A...>>::type;

      if constexpr (std::is_same<head, js_receiver_t>()) {
        argc--;

        err = js_get_callback_info(env, info, &argc, &argv[1], &argv[0], nullptr);
        assert(err == 0);

        argc++;
      } else {
        err = js_get_callback_info(env, info, &argc, argv, nullptr, nullptr);
        assert(err == 0);
      }

      assert(argc == sizeof...(A));
    }

    if constexpr (std::is_same<R, void>()) {
      fn(env, js_unmarshall_untyped_value<A>(env, argv[I])...);

      return js_marshall_untyped_value(env);
    } else {
      auto result = fn(env, js_unmarshall_untyped_value<A>(env, argv[I])...);

      return js_marshall_untyped_value<R>(env, std::move(result));
    }
  };
}

template <auto fn, typename R, typename... A>
constexpr auto
js_untyped_callback() {
  return js_untyped_callback<fn, R, A...>(std::index_sequence_for<A...>());
}

template <auto fn, typename R, typename... A>
constexpr auto
js_create_function(js_env_t *env, const char *name, size_t len, js_function_t<R, A...> &result) {
  auto typed = js_typed_callback<fn, R, A...>();

  auto untyped = js_untyped_callback<fn, R, A...>();

  js_callback_signature_t signature;

  int args[] = {
    js_type_container_t<A>::signature()...
  };

  signature.version = 0;
  signature.result = js_type_container_t<R>::signature();
  signature.args_len = sizeof...(A);
  signature.args = args;

  return js_create_typed_function(env, name, len, untyped, &signature, (const void *) typed, nullptr, &result.value);
}

template <auto fn, typename R, typename... A>
constexpr auto
js_create_function(js_env_t *env, const std::string &name, js_function_t<R, A...> &result) {
  return js_create_function<fn, R, A...>(env, name.data(), name.length(), result);
}

template <typename... A>
constexpr auto
js_call_function(js_env_t *env, const js_function_t<void, js_receiver_t, A...> &function, const js_receiver_t &receiver, const A &...args) {
  int err;

  size_t argc = sizeof...(A);

  js_value_t *argv[] = {
    js_marshall_untyped_value<A>(env, args...)...
  };

  return js_call_function(env, receiver.value, function.value, argc, argv, nullptr);
}

template <typename R, typename... A>
constexpr auto
js_call_function(js_env_t *env, const js_function_t<R, js_receiver_t, A...> &function, const js_receiver_t &receiver, const A &...args, R &result) {
  int err;

  size_t argc = sizeof...(A);

  js_value_t *argv[] = {
    js_marshall_untyped_value<A>(env, args...)...
  };

  js_value_t *value;
  err = js_call_function(env, receiver, function.value, argc, argv, &value);
  if (err < 0) return err;

  result = js_unmarshall_untyped_value<R>(env, value);

  return 0;
}

template <typename... A>
constexpr auto
js_call_function(js_env_t *env, const js_function_t<void, A...> &function, const A &...args) {
  int err;

  size_t argc = sizeof...(A);

  js_value_t *argv[] = {
    js_marshall_untyped_value<A>(env, args...)...
  };

  js_value_t *receiver;
  err = js_get_global(env, &receiver);
  assert(err == 0);

  return js_call_function(env, receiver, function.value, argc, argv, nullptr);
}

template <typename R, typename... A>
constexpr auto
js_call_function(js_env_t *env, const js_function_t<R, A...> &function, const A &...args, R &result) {
  int err;

  size_t argc = sizeof...(A);

  js_value_t *argv[] = {
    js_marshall_untyped_value<A>(env, args...)...
  };

  js_value_t *receiver;
  err = js_get_global(env, &receiver);
  assert(err == 0);

  js_value_t *value;
  err = js_call_function(env, receiver, function.value, argc, argv, &value);
  if (err < 0) return err;

  result = js_unmarshall_untyped_value<R>(env, value);

  return 0;
}

constexpr auto
js_create_object(js_env_t *env, js_object_t &result) {
  return js_create_object(env, &result.value);
}

constexpr auto
js_create_string(js_env_t *env, const utf8_t *str, size_t len, js_string_t<utf8_t> &result) {
  return js_create_string_utf8(env, str, len, &result.value);
}

constexpr auto
js_create_string(js_env_t *env, const std::string &str, js_string_t<utf8_t> &result) {
  return js_create_string_utf8(env, (const utf8_t *) str.data(), str.length(), &result.value);
}

template <typename T>
constexpr auto
js_create_arraybuffer(js_env_t *env, size_t len, T *&data, js_arraybuffer_t &result) {
  return js_create_arraybuffer(env, len * sizeof(T), (void **) &data, &result.value);
}

template <typename T>
constexpr auto
js_create_arraybuffer(js_env_t *env, T *&data, js_arraybuffer_t &result) {
  return js_create_arraybuffer(env, sizeof(T), (void **) &data, &result.value);
}

constexpr auto
js_create_arraybuffer(js_env_t *env, size_t len, js_arraybuffer_t &result) {
  return js_create_arraybuffer(env, len, nullptr, &result.value);
}

template <typename T>
constexpr auto
js_create_arraybuffer(js_env_t *env, size_t len, js_arraybuffer_t &result) {
  return js_create_arraybuffer(env, len * sizeof(T), nullptr, &result.value);
}

template <typename T>
constexpr auto
js_create_arraybuffer(js_env_t *env, size_t len, std::span<T> &view, js_arraybuffer_t &result) {
  int err;

  T *data;
  err = js_create_arraybuffer(env, len, data, result);
  if (err < 0) return err;

  view = std::span(data, len);

  return 0;
}

template <typename T>
constexpr auto
js_create_typedarray(js_env_t *env, size_t len, const js_arraybuffer_t &arraybuffer, size_t offset, js_typedarray_t<T> &result) {
  js_typedarray_type_t type;

  if constexpr (std::is_same<T, int8_t>()) {
    type = js_int8array;
  } else if constexpr (std::is_same<T, uint8_t>()) {
    type = js_uint8array;
  } else if constexpr (std::is_same<T, int16_t>()) {
    type = js_int16array;
  } else if constexpr (std::is_same<T, uint16_t>()) {
    type = js_uint16array;
  } else if constexpr (std::is_same<T, int32_t>()) {
    type = js_int32array;
  } else if constexpr (std::is_same<T, uint32_t>()) {
    type = js_uint32array;
  } else if constexpr (std::is_same<T, float>()) {
    type = js_float32array;
  } else if constexpr (std::is_same<T, double>()) {
    type = js_float64array;
  } else if constexpr (std::is_same<T, int64_t>()) {
    type = js_bigint64array;
  } else if constexpr (std::is_same<T, uint64_t>()) {
    type = js_biguint64array;
  } else {
    static_assert(false, "Unsupported typed array element type");
  }

  return js_create_typedarray(env, type, len, arraybuffer.value, offset, &result.value);
}

template <typename T>
constexpr auto
js_create_typedarray(js_env_t *env, size_t len, const js_arraybuffer_t &arraybuffer, js_typedarray_t<T> &result) {
  return js_create_typedarray(env, len, arraybuffer, 0, result);
}

template <typename T>
constexpr auto
js_create_typedarray(js_env_t *env, size_t len, T *&data, js_typedarray_t<T> &result) {
  int err;

  js_arraybuffer_t arraybuffer;
  err = js_create_arraybuffer(env, len, data, arraybuffer);
  if (err < 0) return err;

  return js_create_typedarray(env, len, arraybuffer, result);
}

template <typename T>
constexpr auto
js_create_typedarray(js_env_t *env, size_t len, std::span<T> &view, js_typedarray_t<T> &result) {
  int err;

  js_arraybuffer_t arraybuffer;
  err = js_create_arraybuffer(env, len, view, arraybuffer);
  if (err < 0) return err;

  return js_create_typedarray(env, len, arraybuffer, result);
}

template <typename T>
constexpr auto
js_create_typedarray(js_env_t *env, size_t len, js_typedarray_t<T> &result) {
  int err;

  js_arraybuffer_t arraybuffer;
  err = js_create_arraybuffer<T>(env, len, arraybuffer);
  if (err < 0) return err;

  return js_create_typedarray(env, len, arraybuffer, result);
}

template <typename T>
constexpr auto
js_get_arraybuffer_info(js_env_t *env, const js_arraybuffer_t &arraybuffer, T *&data, size_t &len) {
  int err;
  err = js_get_arraybuffer_info(env, arraybuffer.value, (void **) &data, &len);
  if (err < 0) return err;

  len /= sizeof(T);

  return 0;
}

template <typename T>
constexpr auto
js_get_arraybuffer_info(js_env_t *env, const js_arraybuffer_t &arraybuffer, T *&data) {
  return js_get_arraybuffer_info(env, arraybuffer.value, (void **) &data, nullptr);
}

template <typename T>
constexpr auto
js_get_arraybuffer_info(js_env_t *env, const js_arraybuffer_t &arraybuffer, std::span<T> &view) {
  int err;

  T *data;
  size_t len;
  err = js_get_arraybuffer_info(env, arraybuffer, data, len);
  if (err < 0) return err;

  view = std::span(data, len);

  return 0;
}

template <typename T>
constexpr auto
js_get_typedarray_info(js_env_t *env, js_typedarray_t<T> &typedarray, T *&data, size_t &len) {
  int err;

  if (typedarray.view) {
    err = js_release_typedarray_view(env, typedarray.view);
    if (err < 0) return err;
  }

  return js_get_typedarray_view(env, typedarray.value, nullptr, (void **) &data, &len, &typedarray.view);
}

template <typename T>
constexpr auto
js_get_typedarray_info(js_env_t *env, js_typedarray_t<T> &typedarray, std::span<T> &view) {
  int err;

  T *data;
  size_t len;
  err = js_get_typedarray_info(env, typedarray, data, len);
  if (err < 0) return err;

  view = std::span(data, len);

  return 0;
}

constexpr auto
js_get_global(js_env_t *env, js_object_t &result) {
  return js_get_global(env, &result.value);
}

constexpr auto
js_get_property(js_env_t *env, const js_object_t &object, const js_name_t &name, js_handle_t &result) {
  return js_get_property(env, object.value, name.value, &result.value);
}

constexpr auto
js_get_property(js_env_t *env, const js_object_t &object, const char *name, js_handle_t &result) {
  return js_get_named_property(env, object.value, name, &result.value);
}

constexpr auto
js_set_property(js_env_t *env, const js_object_t &object, const js_name_t &name, const js_handle_t &value) {
  return js_set_property(env, object.value, name.value, value.value);
}

constexpr auto
js_set_property(js_env_t *env, const js_object_t &object, const char *name, const js_handle_t &value) {
  return js_set_named_property(env, object.value, name, value.value);
}

template <typename T>
constexpr auto
js_run_script(js_env_t *env, const char *file, size_t len, int offset, const js_string_t<T> &source, js_handle_t &result) {
  return js_run_script(env, file, len, offset, source.value, &result.value);
}

template <typename T>
constexpr auto
js_run_script(js_env_t *env, const std::string &file, int offset, const js_string_t<T> &source, js_handle_t &result) {
  return js_run_script(env, file.data(), file.length(), offset, source.value, &result.value);
}

template <typename T>
constexpr auto
js_run_script(js_env_t *env, const js_string_t<T> &source, js_handle_t &result) {
  return js_run_script(env, nullptr, 0, 0, source.value, &result.value);
}
