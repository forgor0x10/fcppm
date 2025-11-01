#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <system_error>

#include <fcppm/fcppm.hpp>

#include <brief/aliases.hpp>
#include <brief/result.hpp>

fn fcppm::create_file(const fs::path &path, const str &content) -> Result<Unit, String> {
    mut stream = std::ofstream(path);
    if (!stream.is_open()) {
        return Err("Error creating file " + path.string() + ": " + std::strerror(errno));
    }

    stream << content;
    if (!stream.is_open()) {
        return Err("Error writing to file " + path.string() + ": " + std::strerror(errno));
    }

    return Ok(Unit());
}

fn fcppm::create_dir(const fs::path &path) -> Result<Unit, String> {
    mut err_code = std::error_code();
    fs::create_directory(path, err_code);

    if (err_code) {
        return Err("Error creating directory " + path.string() + ": " + err_code.message());
    }

    return Ok(Unit());
}

fn fcppm::remove_dir(const fs::path &path) -> Result<Unit, String> {
    if (!fs::exists(path)) {
        return Err("Error removing directory " + path.string() + ": directory does not exist");
    }

    mut err_code = std::error_code();
    fs::remove_all(path, err_code);

    if (err_code) {
        return Err("Error removing directory " + path.string() + ": " + err_code.message());
    }

    return Ok(Unit());
}
