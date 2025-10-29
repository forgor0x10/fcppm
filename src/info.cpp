#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>

#include <fcppm/fcppm.hpp>

#include <brief/brief.hpp>

namespace fs = std::filesystem;

fn fcppm::info() -> Optional<String> {
    if (!fs::exists("fcpp.toml")) { return "No fcpp.toml file found"; }

    mut file = std::ifstream("fcpp.toml");
    if (!file.is_open()) { return "Error opening file fcpp.toml: " + String(std::strerror(errno)); }

    mut file_string_stream = std::stringstream();
    file_string_stream << file.rdbuf();

    let file_content = String(file_string_stream.str());

    let entries = Vec<fcppm::TomlEntry>(fcppm::parse_toml(file_content));

    mut project_name = String();
    mut project_description = String();
    mut project_version = String();
    mut project_license = String();

    let fields = UMap<String, String *>({
        {"name", &project_name},
        {"description", &project_description},
        {"version", &project_version},
        {"license", &project_license},
    });

    for (let &[ table, key, value ] : entries) {
        if (table.has_value() && table.value() == "project") {
            if (let iter = fields.find(key); iter != fields.end()) { *(iter->second) = value; }
        }
    }

    if (project_name.empty()) { return "Project name absent or empty"; }

    if (project_description.empty()) { return "Project description absent or empty"; }

    if (project_version.empty()) { return "Project version absent or empty"; }

    println(project_name, " ", project_version);
    println(project_description);

    return none;
}
