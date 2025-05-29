#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "src/nob.h"
#undef rename

#define SRC "src/"
#define BUILD "build/"
#define ARTIFACTS "build/artifacts/"
#define DEBUG "build/enjenir-debug.exe"
#define RELEASE "build/enjenir.exe"
#define L_WIN "lib/raylib-5.5_win64_mingw-w64/"
#define L_LINUX "lib/raylib-5.5_linux_amd64/"
#define L_WEB "lib/raylib-5._webassembly/"

/* TODO:
 * [ ] args
 *   [ ] clean
 *   [ ] debug (default)
 *   [ ] release
 *   [ ] cross/conditional compilation
 *     [ ] windows
 *     [ ] web
 *     [ ] linux
 *     [ ] android
 *   [ ] all
 * [ ] copy assets
 *   [ ] embed assets
 */

Proc spawn_compile(const char *src, const char *obj) {
  Cmd cmd = {0};
  nob_cmd_append(&cmd, "zig", "cc", "-Wall", "-Wextra", "-Isrc",
                 "-I" L_WIN "include", "-c", src, "-o", obj);
  return nob_cmd_run_async_and_reset(&cmd);
}

void compile_dir(const char *src_dir, const char *obj_dir, Procs *procs) {
  File_Paths files = {0};
  if (!read_entire_dir(src_dir, &files)) {
    nob_log(ERROR, "Failed to read directory: %s", src_dir);
    return;
  }

  for (size_t i = 0; i < files.count; ++i) {
    String_View name_sv = sv_from_cstr(files.items[i]);
    if (!sv_end_with(name_sv, ".c"))
      continue;

    char src_path[512];
    snprintf(src_path, sizeof(src_path), "%s%s", src_dir, files.items[i]);

    char obj_name[256];
    snprintf(obj_name, sizeof(obj_name), "%s", files.items[i]);
    size_t len = strlen(obj_name);
    if (len >= 2 && strcmp(obj_name + len - 2, ".c") == 0) {
      obj_name[len - 2] = '\0'; // Remove ".c"
    }
    strncat(obj_name, ".o", sizeof(obj_name) - strlen(obj_name) - 1);

    char obj_path[512];
    snprintf(obj_path, sizeof(obj_path), "%s%s", obj_dir, obj_name);

    da_append(procs, spawn_compile(src_path, obj_path));
  }

  da_free(files);
}

void link_dir(const char *obj_dir, const char *exe_path) {
  File_Paths files = {0};
  if (!read_entire_dir(obj_dir, &files)) {
    nob_log(ERROR, "Failed to read directory: %s", obj_dir);
    return;
  }

  // Build a temporary array of object file paths
  const char *obj_paths[files.count];
  size_t obj_count = 0;

  for (size_t i = 0; i < files.count; ++i) {
    String_View name_sv = sv_from_cstr(files.items[i]);
    if (!sv_end_with(name_sv, ".o"))
      continue;

    char *obj_path = nob_temp_sprintf("%s%s", obj_dir, files.items[i]);
    obj_paths[obj_count++] = obj_path;
  }

  Cmd cmd = {0};
  nob_cmd_append(&cmd, "zig", "cc", "-target", "x86_64-windows-gnu", "-Wall",
                 "-Wextra", "-std=c11", "-DPLATFORM_DESKTOP", "-DNOGDI",
                 "-Isrc", "-I" L_WIN "include", "-static", "-static-libgcc",
                 "-static-libstdc++", // Keep these
                 "-o", exe_path);

  // Add all object files
  nob_da_append_many(&cmd, obj_paths, obj_count);

  // Add Raylib via direct path, then system libs
  nob_cmd_append(&cmd, L_WIN "lib/libraylib.a", "-lopengl32", "-lgdi32",
                 "-lwinmm", "-lkernel32", "-luser32", "-lshell32", "-ladvapi32",
                 "-lole32", "-Wl,/subsystem:console");
  if (!nob_cmd_run_sync_and_reset(&cmd)) {
    nob_log(ERROR, "Linking failed");
  }

  da_free(files);
}

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  mkdir_if_not_exists(BUILD);
  mkdir_if_not_exists(ARTIFACTS);

  Procs procs = {0};

  compile_dir(SRC, ARTIFACTS, &procs);
  if (!procs_wait_and_reset(&procs)) {
    return 1;
  }

  link_dir(ARTIFACTS, RELEASE);

  return 0;
}
