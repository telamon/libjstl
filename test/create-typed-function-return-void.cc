#include <assert.h>
#include <js.h>
#include <stdint.h>
#include <utf.h>
#include <uv.h>

#include "../include/jstl.h"

void
on_call(void) {}

int
main() {
  int e;

  uv_loop_t *loop = uv_default_loop();

  js_platform_options_t options = {
    .trace_optimizations = true,
  };

  js_platform_t *platform;
  e = js_create_platform(loop, &options, &platform);
  assert(e == 0);

  js_env_t *env;
  e = js_create_env(loop, platform, NULL, &env);
  assert(e == 0);

  js_handle_scope_t *scope;
  e = js_open_handle_scope(env, &scope);
  assert(e == 0);

  js_function_t<void> fn;
  e = js_create_typed_function<on_call>(env, "hello", fn);
  assert(e == 0);

  js_object_t global;
  e = js_get_global(env, global);
  assert(e == 0);

  e = js_set_named_property(env, global, "hello", fn);
  assert(e == 0);

  js_string_t<utf8_t> script;
  e = js_create_string(env, "let i = 0, j; while (i++ < 200000) hello()", script);
  assert(e == 0);

  js_value_t *result;
  e = js_run_script(env, NULL, 0, 0, script.value, &result);
  assert(e == 0);

  e = js_close_handle_scope(env, scope);
  assert(e == 0);

  e = js_destroy_env(env);
  assert(e == 0);

  e = js_destroy_platform(platform);
  assert(e == 0);

  e = uv_run(loop, UV_RUN_DEFAULT);
  assert(e == 0);
}
