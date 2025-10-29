#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <system_error>

#include <fcppm/fcppm.hpp>

#include <brief/brief.hpp>

fn fcppm::create_file(const fs::path &path, const str &content) -> Optional<String> {
    mut stream = std::ofstream(path);
    if (!stream.is_open()) {
        return String("Error creating file " + path.string() + ": " + std::strerror(errno));
    }

    stream << content;
    if (!stream.is_open()) {
        return String("Error writing to file " + path.string() + ": " + std::strerror(errno));
    }

    return none;
}

fn fcppm::create_dir(const fs::path &path) -> Optional<String> {
    mut err_code = std::error_code();
    fs::create_directory(path, err_code);

    if (err_code) {
        return "Error creating directory " + path.string() + ": " + err_code.message();
    }

    return none;
}

fn fcppm::remove_dir(const fs::path &path) -> Optional<String> {
    if (!fs::exists(path)) {
        return "Error removing directory " + path.string() + ": directory does not exist";
    }

    mut err_code = std::error_code();
    fs::remove_all(path, err_code);

    if (err_code) {
        return "Error removing directory " + path.string() + ": " + err_code.message();
    }

    return none;
}
