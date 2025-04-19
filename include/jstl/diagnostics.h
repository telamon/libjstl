/**
 * Usage:
 *
 * // binding.cc
 *
 *   #define JSTL_DIAGNOSTICS
 *   #include <jstl.h>
 *
 *   static js_value_t *
 *   bare_addon_exports(js_env_t *env, js_value_t *exports) {
 *     // ...
 *
 *     int err = jstl_diag_exports(env, exports);
 *
 *     return exports;
 *   }
 *
 * // script.js
 *
 *   const binding = require('./binding')
 *
 *   binding.__print_counters()
 */

#include <string>
#include <unordered_map>
#include <stdio.h>
#include <js.h>

namespace jstl::diag {
  static std::unordered_map<void *, size_t> untyped_counters;
  static std::unordered_map<void *, size_t> typed_counters;
  static std::unordered_map<void *, std::string> names;

  static inline void
  increase_untyped (void *function_ptr) {
    untyped_counters[function_ptr]++;
  }

  static inline void
  increase_typed (void *function_ptr) {
    typed_counters[function_ptr]++;
  }

  static inline void
  name (void *function_ptr, const char* name, size_t len) {
    std::string key = std::string(name, len);
    names[function_ptr] = key;
  }

  // TODO: maybe export a js object/map instead
  static void
  print_counters () {
    printf("== call counts ==\n");

    printf("\nUntyped calls\n");

    for (const auto &[fnptr, counter] : untyped_counters) {
      if (names.contains(fnptr)) {
        printf("%s \t=> %zu\n", names[fnptr].c_str(), counter);
      } else {
        printf("Unknown[%p] \t=> %zu\n", fnptr, counter);
      }
    }

    printf("\nTyped calls\n");

    for (const auto &[fnptr, counter] : typed_counters) {
      if (names.contains(fnptr)) {
        printf("%s \t=> %zu\n", names[fnptr].c_str(), counter);
      } else {
        printf("Unknown[%p] \t=> %zu\n", fnptr, counter);
      }
    }
  }

  js_value_t *
  on_print (js_env_t *env, js_callback_info_t *info) {
    print_counters();
    return nullptr;
  }
}

static inline void
jstl_diag_exports(js_env_t *env, js_value_t *exports) {
  int err = 0;

  const char *name = "__print_counters";

  js_value_t *fn;
  err = js_create_function(env, name, -1, jstl::diag::on_print, NULL, &fn);
  assert(err == 0);

  err = js_set_named_property(env, exports, name, fn);
  assert(err == 0);
}
