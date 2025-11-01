#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>

#include <fcppm/fcppm.hpp>

#include <brief/aliases.hpp>
#include <brief/io.hpp>
#include <brief/result.hpp>

fn fcppm::info() -> Result<Unit, String> {
    if (!fs::exists("fcpp.toml")) { return Err(String("No fcpp.toml file found")); }

    mut file = std::ifstream("fcpp.toml");
    if (!file.is_open()) {
        return Err("Error opening file fcpp.toml: " + String(std::strerror(errno)));
    }

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

    if (project_name.empty()) { return Err(String("Project name absent or empty")); }
    if (project_description.empty()) { return Err(String("Project description absent or empty")); }
    if (project_version.empty()) { return Err(String("Project version absent or empty")); }

    println(project_name, " ", project_version);
    println(project_description);

    return Ok(Unit());
}
