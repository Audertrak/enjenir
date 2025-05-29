#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "src/nob.h" // Corrected include path
#undef rename

// Source and Library Paths
#define SRC_DIR "src/"
#define RAYLIB_WIN_DIR "lib/raylib-5.5_win64_mingw-w64/"
#define RAYLIB_WIN_INCLUDE_DIR RAYLIB_WIN_DIR "include/"
#define RAYLIB_WIN_LIB_FILE RAYLIB_WIN_DIR "lib/libraylib.a"
// #define L_LINUX "lib/raylib-5.5_linux_amd64/" // For future
// #define L_WEB "lib/raylib-5._webassembly/" // For future

// Build Output Base Paths
#define BUILD_DIR "build/"

// Windows Debug Paths
#define WIN_DEBUG_DIR BUILD_DIR "debug/"
#define WIN_DEBUG_ARTIFACTS_DIR WIN_DEBUG_DIR "artifacts/"
#define WIN_DEBUG_EXE WIN_DEBUG_DIR "enjenir-debug.exe"

// Windows Release Paths
#define WIN_RELEASE_DIR BUILD_DIR "release/"
#define WIN_RELEASE_ARTIFACTS_DIR WIN_RELEASE_DIR "artifacts/"
#define WIN_RELEASE_EXE WIN_RELEASE_DIR "enjenir.exe"

// --- Compiler and Linker Flags ---

// Common CFLAGS for Windows native builds
const char *cflags_win_common[] = {"-Wall",
                                   "-Wextra",
                                   "-std=c11",
                                   "-DPLATFORM_DESKTOP",
                                   "-DNOGDI",
                                   "-I" SRC_DIR,
                                   "-I" RAYLIB_WIN_INCLUDE_DIR};
size_t cflags_win_common_count = NOB_ARRAY_LEN(cflags_win_common);

// Debug specific CFLAGS for Windows
const char *cflags_win_debug_extra[] = {"-g", "-DDEBUG"};
size_t cflags_win_debug_extra_count = NOB_ARRAY_LEN(cflags_win_debug_extra);

// Release specific CFLAGS for Windows
const char *cflags_win_release_extra[] = {"-O3", "-DNDEBUG"};
size_t cflags_win_release_extra_count = NOB_ARRAY_LEN(cflags_win_release_extra);

// Common LDFLAGS prefix for Windows native builds (before objects and output
// exe)
const char *ldflags_win_prefix_common[] = {"zig",
                                           "cc",
                                           "-target",
                                           "x86_64-windows-gnu",
                                           "-static",
                                           "-static-libgcc",
                                           "-static-libstdc++"};
size_t ldflags_win_prefix_common_count =
    NOB_ARRAY_LEN(ldflags_win_prefix_common);

// Common LDFLAGS suffix for Windows native builds (after objects - libraries)
const char *ldflags_win_suffix_common[] = {
    RAYLIB_WIN_LIB_FILE, "-lopengl32", "-lgdi32",    "-lwinmm", "-lkernel32",
    "-luser32",          "-lshell32",  "-ladvapi32", "-lole32"};
size_t ldflags_win_suffix_common_count =
    NOB_ARRAY_LEN(ldflags_win_suffix_common);

// Debug specific LDFLAGS for Windows (subsystem)
const char *ldflags_win_debug_subsystem[] = {"-Wl,/subsystem:console"};
size_t ldflags_win_debug_subsystem_count =
    NOB_ARRAY_LEN(ldflags_win_debug_subsystem);

// Release specific LDFLAGS for Windows (subsystem)
const char *ldflags_win_release_subsystem[] = {"-Wl,/subsystem:windows"};
size_t ldflags_win_release_subsystem_count =
    NOB_ARRAY_LEN(ldflags_win_release_subsystem);

// --- Build Functions ---

Proc spawn_compile(const char *src_file, const char *obj_file,
                   const char **cflags_list, size_t cflags_list_count) {
  Cmd cmd = {0};
  nob_cmd_append(&cmd, "zig", "cc");
  nob_da_append_many(&cmd, cflags_list, cflags_list_count);
  nob_cmd_append(&cmd, "-c", src_file, "-o", obj_file);
  return nob_cmd_run_async_and_reset(&cmd);
}

void compile_source_files_in_dir(const char *src_dir, const char *obj_dir,
                                 Procs *procs, const char **cflags_list,
                                 size_t cflags_list_count) {
  File_Paths files = {0};
  if (!read_entire_dir(src_dir, &files)) {
    nob_log(ERROR, "Failed to read source directory: %s", src_dir);
    return;
  }

  for (size_t i = 0; i < files.count; ++i) {
    String_View name_sv = sv_from_cstr(files.items[i]);
    if (!sv_end_with(name_sv, ".c"))
      continue;

    char src_path[PATH_MAX];
    snprintf(src_path, sizeof(src_path), "%s%s", src_dir, files.items[i]);

    char obj_name_base[FILENAME_MAX];
    snprintf(obj_name_base, sizeof(obj_name_base), "%s", files.items[i]);
    size_t len = strlen(obj_name_base);
    if (len >= 2 && strcmp(obj_name_base + len - 2, ".c") == 0) {
      obj_name_base[len - 2] = '\0'; // Remove ".c"
    }
    strncat(obj_name_base, ".o",
            sizeof(obj_name_base) - strlen(obj_name_base) - 1);

    char obj_path[PATH_MAX];
    snprintf(obj_path, sizeof(obj_path), "%s%s", obj_dir, obj_name_base);

    da_append(procs, spawn_compile(src_path, obj_path, cflags_list,
                                   cflags_list_count));
  }
  da_free(files);
}

void link_objects_to_executable(const char *obj_dir, const char *exe_path,
                                const char **base_ldflags,
                                size_t base_ldflags_count,
                                const char **specific_ldflags,
                                size_t specific_ldflags_count,
                                const char **lib_ldflags,
                                size_t lib_ldflags_count) {
  File_Paths obj_files_in_dir = {0};
  if (!read_entire_dir(obj_dir, &obj_files_in_dir)) {
    nob_log(ERROR, "Failed to read object directory: %s", obj_dir);
    return;
  }

  const char *collected_obj_paths[obj_files_in_dir.count];
  size_t actual_obj_count = 0;

  for (size_t i = 0; i < obj_files_in_dir.count; ++i) {
    String_View name_sv = sv_from_cstr(obj_files_in_dir.items[i]);
    if (sv_end_with(name_sv, ".o")) {
      collected_obj_paths[actual_obj_count++] =
          nob_temp_sprintf("%s%s", obj_dir, obj_files_in_dir.items[i]);
    }
  }

  Cmd cmd = {0};
  nob_da_append_many(&cmd, base_ldflags, base_ldflags_count);
  nob_da_append_many(&cmd, specific_ldflags, specific_ldflags_count);
  nob_cmd_append(&cmd, "-o", exe_path);
  nob_da_append_many(&cmd, collected_obj_paths, actual_obj_count);
  nob_da_append_many(&cmd, lib_ldflags, lib_ldflags_count);

  if (!nob_cmd_run_sync_and_reset(&cmd)) {
    nob_log(ERROR, "Linking failed for: %s", exe_path);
  }

  da_free(obj_files_in_dir);
}

void do_clean(const char *path_to_clean) {
  nob_log(INFO, "Cleaning directory: %s", path_to_clean);
  if (!nob_file_exists(path_to_clean)) {
    nob_log(INFO, "Directory %s does not exist, nothing to clean.",
            path_to_clean);
    return;
  }
  Cmd cmd = {0};
#ifdef _WIN32
  nob_cmd_append(&cmd, "cmd", "/c", "rmdir", "/s", "/q", path_to_clean);
#else
  nob_cmd_append(&cmd, "rm", "-rf", path_to_clean);
#endif
  if (!nob_cmd_run_sync_and_reset(&cmd)) {
    nob_log(ERROR, "Failed to clean directory: %s", path_to_clean);
  }
}

void do_build_windows_debug() {
  nob_log(INFO, "Building Windows Debug target...");
  mkdir_if_not_exists(BUILD_DIR);
  mkdir_if_not_exists(WIN_DEBUG_DIR);
  mkdir_if_not_exists(WIN_DEBUG_ARTIFACTS_DIR);

  // Prepare debug CFLAGS
  Cmd debug_cflags_cmd = {0}; // Use Cmd as a temporary dynamic array for flags
  nob_da_append_many(&debug_cflags_cmd, cflags_win_common,
                     cflags_win_common_count);
  nob_da_append_many(&debug_cflags_cmd, cflags_win_debug_extra,
                     cflags_win_debug_extra_count);

  Procs procs = {0};
  compile_source_files_in_dir(SRC_DIR, WIN_DEBUG_ARTIFACTS_DIR, &procs,
                              debug_cflags_cmd.items, debug_cflags_cmd.count);
  if (!procs_wait_and_reset(&procs)) {
    nob_log(ERROR, "Debug compilation failed.");
    nob_cmd_free(debug_cflags_cmd);
    return;
  }
  nob_cmd_free(debug_cflags_cmd);

  link_objects_to_executable(
      WIN_DEBUG_ARTIFACTS_DIR, WIN_DEBUG_EXE, ldflags_win_prefix_common,
      ldflags_win_prefix_common_count, ldflags_win_debug_subsystem,
      ldflags_win_debug_subsystem_count, ldflags_win_suffix_common,
      ldflags_win_suffix_common_count);
  nob_log(INFO, "Windows Debug build complete: %s", WIN_DEBUG_EXE);
}

void do_build_windows_release() {
  nob_log(INFO, "Building Windows Release target...");
  mkdir_if_not_exists(BUILD_DIR);
  mkdir_if_not_exists(WIN_RELEASE_DIR);
  mkdir_if_not_exists(WIN_RELEASE_ARTIFACTS_DIR);

  // Prepare release CFLAGS
  Cmd release_cflags_cmd = {0};
  nob_da_append_many(&release_cflags_cmd, cflags_win_common,
                     cflags_win_common_count);
  nob_da_append_many(&release_cflags_cmd, cflags_win_release_extra,
                     cflags_win_release_extra_count);

  Procs procs = {0};
  compile_source_files_in_dir(SRC_DIR, WIN_RELEASE_ARTIFACTS_DIR, &procs,
                              release_cflags_cmd.items,
                              release_cflags_cmd.count);
  if (!procs_wait_and_reset(&procs)) {
    nob_log(ERROR, "Release compilation failed.");
    nob_cmd_free(release_cflags_cmd);
    return;
  }
  nob_cmd_free(release_cflags_cmd);

  link_objects_to_executable(
      WIN_RELEASE_ARTIFACTS_DIR, WIN_RELEASE_EXE, ldflags_win_prefix_common,
      ldflags_win_prefix_common_count, ldflags_win_release_subsystem,
      ldflags_win_release_subsystem_count, ldflags_win_suffix_common,
      ldflags_win_suffix_common_count);
  nob_log(INFO, "Windows Release build complete: %s", WIN_RELEASE_EXE);
}

void print_usage() {
  nob_log(INFO, "Usage: nob.exe [target]");
  nob_log(INFO, "Targets:");
  nob_log(INFO, "  debug          Build the Windows debug version (default if "
                "no target).");
  nob_log(INFO, "  release        Build the Windows release version.");
  nob_log(
      INFO,
      "  all            Build all default Windows versions (debug, release).");
  nob_log(INFO, "  clean [target] Clean build artifacts. Target can be 'all', "
                "'debug', 'release'.");
  nob_log(INFO, "                 If no clean target, 'all' is assumed.");
}

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);
  shift_args(&argc, &argv); // Consume the program name

  if (argc == 0) {
    nob_log(INFO, "No target specified. Building Windows Debug by default.");
    do_build_windows_debug();
  } else {
    const char *target = argv[0];
    if (strcmp(target, "clean") == 0) {
      if (argc > 1) {
        const char *clean_target = argv[1];
        if (strcmp(clean_target, "all") == 0)
          do_clean(BUILD_DIR);
        else if (strcmp(clean_target, "debug") == 0)
          do_clean(WIN_DEBUG_DIR);
        else if (strcmp(clean_target, "release") == 0)
          do_clean(WIN_RELEASE_DIR);
        else {
          nob_log(ERROR, "Unknown clean target: `%s`", clean_target);
          print_usage();
        }
      } else {
        nob_log(INFO, "Cleaning all build artifacts.");
        do_clean(BUILD_DIR);
      }
    } else if (strcmp(target, "debug") == 0) {
      do_build_windows_debug();
    } else if (strcmp(target, "release") == 0) {
      do_build_windows_release();
    } else if (strcmp(target, "all") == 0) {
      nob_log(INFO, "Building all Windows targets (Debug and Release).");
      do_build_windows_debug();
      do_build_windows_release();
    } else {
      nob_log(ERROR, "Unknown target: `%s`", target);
      print_usage();
    }
  }

  return 0;
}
