#include <assert.h>
#include <js.h>
#include <uv.h>

#include "../include/jstl.h"

int
main() {
  int e;

  uv_loop_t *loop = uv_default_loop();

  js_platform_t *platform;
  e = js_create_platform(loop, NULL, &platform);
  assert(e == 0);

  js_env_t *env;
  e = js_create_env(loop, platform, NULL, &env);
  assert(e == 0);

  js_handle_scope_t *scope;
  e = js_open_handle_scope(env, &scope);
  assert(e == 0);

  js_typedarray_t<uint8_t> a;
  e = js_create_typedarray(env, 5, a);
  assert(e == 0);

  uint8_t *data;
  size_t len;
  e = js_get_typedarray_info(env, a, data, len);
  assert(e == 0);

  js_typedarray_t<uint8_t> b = a;

  e = js_close_handle_scope(env, scope);
  assert(e == 0);

  e = js_destroy_env(env);
  assert(e == 0);

  e = js_destroy_platform(platform);
  assert(e == 0);

  e = uv_run(loop, UV_RUN_DEFAULT);
  assert(e == 0);
}
