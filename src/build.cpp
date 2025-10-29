#include <stdio.h> // NOLINT: Necessary for popen and pclose

#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <variant>

#include <brief/brief.hpp>
#include <fcppm/fcppm.hpp>

namespace fs = std::filesystem;

namespace {
struct CommandTask {
    const String &command;
    const String &name;
    const String &file;
};

fn needs_rebuild(const fs::path &src_path, const fs::path &object_path) -> bool {
    if (!fs::exists(object_path)) { return true; }

    let src_time = fs::file_time_type(fs::last_write_time(src_path));
    let dst_time = fs::file_time_type(fs::last_write_time(object_path));

    return src_time > dst_time;
}

fn get_project_name() -> Tuple<bool, String> {
    if (!fs::exists("fcpp.toml")) { return {false, "No fcpp.toml file found"}; }

    mut file = std::ifstream("fcpp.toml");
    if (!file.is_open()) {
        return {false, "Error opening file fcpp.toml: " + String(std::strerror(errno))};
    }

    mut file_string_stream = std::stringstream();
    file_string_stream << file.rdbuf();

    let file_content = String(file_string_stream.str());

    let entries = Vec<fcppm::TomlEntry>(fcppm::parse_toml(file_content));

    mut project_name = String();

    for (let &[ table, key, value ] : entries) {
        if (table.has_value() && table.value() == "project") {
            if (key == "name") { project_name = value; }
        }
    }

    if (project_name.empty()) { return {false, "Project name absent or empty"}; }

    return {true, project_name};
}

fn run_command(const CommandTask &task) -> Optional<String> {
    constexpr let buffer_size = i32(128);
    print("\033[30m> \033[0m", task.name, ": ", task.file, "\n");

    let pipe = popen(task.command.c_str(), "r");
    if (pipe == nullptr) { return "Failed to run command: " + task.command; }

    mut buffer = Array<char, buffer_size>();
    mut output = String();

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) { output += buffer.data(); }

    let ret = i32(pclose(pipe));

    if (ret != 0) {
        println();
        eprintln(output);
        return "Failed: " + task.name + ": " + task.file;
    }

    return none;
}

fn collect_source_files(const fs::path &src_dir) -> Variant<String, Vec<String>> {
    if (!fs::exists(src_dir) || !fs::is_directory(src_dir)) {
        return src_dir.string() + " does not exist or is not a directory";
    }

    mut files = Vec<String>();

    for (let &entry in fs::directory_iterator(src_dir)) {
        if (fs::is_regular_file(entry.status())) {
            files.push_back(entry.path().filename().string());
        }
    }

    return files;
}

fn object_file_path(const String &src_file) -> String {
    constexpr let cpp_ext = str(".cpp");
    mut object_file = String(src_file);

    object_file.replace(object_file.find(cpp_ext), cpp_ext.length(), ".o");

    return object_file;
}

fn tidy_files(const Vec<String> &files, const fs::path &src_dir, const fs::path &build_dir)
    -> Optional<String> {
    for (const auto &file : files) {
        let src_path = src_dir / file;
        let object_path = build_dir / object_file_path(file);

        if (!needs_rebuild(src_path, object_path)) { continue; }

        let result = run_command({.command = "clang-tidy --quiet " + src_dir.string() + "/" + file +
                                             " -- -Iinclude 2>&1",
                                  .name = "Tidying",
                                  .file = src_dir.string() + "/" + file});
        if (result.has_value()) { return result; }
    }

    return none;
}

fn compile_files(const Vec<String> &files, const fs::path &src_dir, const fs::path &build_dir)
    -> Optional<String> {
    for (let &file in files) {
        let object_file = String(object_file_path(file));

        let src_path = src_dir / file;
        let object_path = build_dir / object_file;

        if (!needs_rebuild(src_path, object_path)) { continue; }

        mut command = String("g++ -Wall -Wextra -Iinclude -c " + src_dir.string() + "/" + file +
                             " -o " + build_dir.string() + "/" + object_file + " 2>&1");

        let result = run_command(
            {.command = command, .name = "Compiling", .file = src_dir.string() + "/" + file});
        if (result.has_value()) { return result; }
    }

    return none;
}

fn link_project(const Vec<String> &files, const fs::path &build_dir, const fs::path &target_dir,
                const str &project_name) -> Optional<String> {
    mut command = String("g++ -Wall -Wextra -Iinclude -o " + target_dir.string() + "/" +
                         String(project_name) + " ");

    for (let &file in files) { command += build_dir.string() + "/" + object_file_path(file) + " "; }

    command += "2>&1";

    let result = run_command({.command = command,
                              .name = "Linking",
                              .file = target_dir.string() + "/" + String(project_name)});
    if (result.has_value()) { return result; }

    return none;
}
} // namespace

fn fcppm::build(bool skip_tidying) -> Optional<String> {
    let src_dir = fs::path("src");
    let build_dir = fs::path("build");
    let target_dir = fs::path("target");

    mut project_name = String();

    if (let err = get_project_name(); std::get<bool>(err)) {
        project_name = std::get<String>(err);
    } else {
        return std::get<String>(err);
    }

    mut files = Vec<String>();

    if (let err = collect_source_files(src_dir); std::holds_alternative<Vec<String>>(err)) {
        files = std::get<Vec<String>>(err);
    } else {
        return std::get<String>(err);
    }

    if (!skip_tidying) { try_fn_opt(tidy_files(files, src_dir, build_dir)); }

    if (!fs::exists(build_dir)) {
        println("\033[30m> \033[0mCreating build directory");
        try_fn_opt(fcppm::create_dir(build_dir));
    }

    try_fn_opt(compile_files(files, src_dir, build_dir));

    if (!fs::exists(target_dir)) {
        println("\033[30m> \033[0mCreating target directory");
        try_fn_opt(fcppm::create_dir(target_dir));
    }

    try_fn_opt(link_project(files, build_dir, target_dir, project_name));

    println("\033[36m> Success: \033[0mBuilt project ", project_name);

    return none;
}
