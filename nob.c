#define NOB_IMPLEMENTATION

#include "lib/nob.h" 

#undef rename

#define CC "gcc"

// source and Library Paths
#define ASSETS "assets/"
#define FONTS "assets/fonts/"
#define ICONS "assets/icons/"
#define LIB "lib/"
#define SRC "src/"
#define WEB_I "web/"
#define RAYLIB_WEB "lib/raylib-5._webassembly/"
#define RAYLIB_WEB_I RAYLIB_WEB "include/"
#define RAYLIB_WEB_L RAYLIB_WEB "lib/libraylib.a"

#ifdef _WIN32 // windows inc/link paths
#define RAYLIB "lib/raylib-5.5_win64_mingw-w64/"
#define RAYLIB_I RAYLIB_WIN_DIR "include/"
#define RAYLIB_L RAYLIB_WIN_DIR "lib/libraylib.a"
#endif // _WIN32

#ifdef __linux__ // linux inc/link paths
#define RAYLIB "lib/raylib_5.5_linux_amd64/"
#define RAYLIB_I RAYLIB_WIN_DIR "include/"
#define RAYLIB_L RAYLIB_WIN_DIR "lib/libraylib.a"
#endif // __linux__

// output paths
#define BUILD "build/"
#define WEB BUILD "web/"
#define DEBUG BUILD "debug/"
#define DEBUG_ARTIFACTS DEBUG "artifacts/"
#define DEBUG_EXE DEBUG "enjenir-debug.exe"
#define RELEASE BUILD "release/"
#define RELEASE_ARTIFACTS RELEASE "artifacts/"
#define RELEASE_EXE RELEASE "enjenir.exe"

// Common CFLAGS for Windows native builds
const char *cflags_win_common[] = {"-Wall",
                                   "-Wextra",
                                   "-std=c11",
                                   "-DPLATFORM_DESKTOP",
                                   "-DNOGDI",
                                   "-I" SRC,
                                   "-I" RAYLIB_I};

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
  compile_source_files_in_dir(SRC, WIN_DEBUG_ARTIFACTS_DIR, &procs,
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
  compile_source_files_in_dir(SRC, WIN_RELEASE_ARTIFACTS_DIR, &procs,
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
  set_log_handler(&cancer_log_handler);

  Nob_File_Paths input_folders = {0};
  da_append(&input_folders, ASSETS);
  da_append(&input_folders, FONTS);
  da_append(&input_folders, ICONS);
  da_append(&input_folders, SRC);
  da_append(&input_folders, LIB);
  da_append(&input_folders, WEB);

  Nob_File_Paths output_folders = {0};
  da_append(&output_folders, BUILD);
  da_append(&output_folders, DEBUG);
  da_append(&output_folders, RELEASE);

  da_foreach(const char *, folder_path, &output_folders) {
    // TODO: "walk dir" and make if not exists
    if (!mkdir_if_not_exists(*folder_path)) {
      nob_log(NOB_ERROR, "Could not create folder %s", *folder_path);
      continue;
    }
  }

  printf("\n");

  Nob_File_Paths files = {0};
  // loop through the list of folders and add their files to a dynamic array
  da_foreach(const char *, folder_path, &input_folders) {
    Nob_File_Paths tmp = {0}; // temp file buffer
    if (!read_entire_dir(*folder_path, &tmp)) {
      nob_log(NOB_ERROR, "Could not read directory %s", *folder_path);
      da_free(tmp);
      continue;
    }

    da_foreach(const char *, entry, &tmp) {
      const char *name = *entry;
      if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        continue;
      }
      size_t name_len = strlen(name);
      if (name_len > 2 && name[name_len - 2] == '.' &&
          name[name_len - 1] == 'c') {
        String_Builder full_path = {0};
        sb_append_cstr(&full_path, *entry);

        size_t folder_len = strlen(*entry);
        if ((size_t)folder_path[folder_len - 1] == '/') {
          sb_append_cstr(&full_path, name);
        } else {
          sb_append_cstr(&full_path, "/");
          sb_append_cstr(&full_path, name);
        }
        sb_append_null(&full_path);

        da_append(&files, temp_strdup(full_path.items));
        sb_free(full_path);
      }
    }
    da_free(tmp);
  }
  
  // TODO: make sure windows flags are all handled correctly
  // TODO: add control flow for web arg to invoke emscripten etc
  Cmd cmd = {0};

  // TODO: remove debug ifdef and instead add control flow for debug arg and add debug flags
  //  - no optimizations for debug build
  //  - include debug symbols
  //  - "console" app
  if (argc == 0) {

    nob_log(INFO, "No target specified. Building Windows Debug by default.");
    cmd_append(&cmd, CC, "-Wall", "-Wextra","-std=c11" );

    #ifdef WIN32
    //do_build_windows_debug();
    cmd_append(&cmd, "-g", "-DDEBUG");
    cmd_append(&cmd, "-WL,/subsystem:console");
    #endif // WIN32

  } else {

    // TODO: clean
    const char *arg = argv[0];
    if (strcmp(arg, "clean") == 0) {
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

  // TODO: release build
  //  - O2
  //  - no console
  //  - no debug symbols
    cmd_append("-O2");
    cmd_append(&cmd, "-I"LIB, "-I"SRC, "-I"RAYLIB_I);
    #ifdef WIN32
    {
      cmd_append(&cmd, "-target", "x86_64-windows-gnu", "-lopengl32", "-lgdi32", "-lwinm", "-lkernel32", "-luser32", "-lshell32", "-ladvapi32", "-lole32", "-WL,/subsystem:windows", "-DPLATFORM_DESKTOP", "-DNOGDI" );
    }
    #endif



    } else if (strcmp(arg, "debug") == 0) {
      do_build_windows_debug();
    } else if (strcmp(arg, "release") == 0) {
      do_build_windows_release();
    } else if (strcmp(arg, "all") == 0) {
      nob_log(INFO, "Building all Windows targets (Debug and Release).");
      do_build_windows_debug();
      do_build_windows_release();
    } else {
      nob_log(ERROR, "Unknown target: `%s`", arg);
      print_usage();
    }
  }

  return 0;
}
