#include <filesystem>

#include <brief/brief.hpp>

#define try_fn_opt(e)                                                                              \
    if (auto _err = (e); _err.has_value()) { return _err; }

namespace fs = std::filesystem;

namespace fcppm {
constexpr let version = str("1.0.0");

struct TomlEntry {
    Optional<String> table;
    String key;
    String value;
};

fn create_file(const fs::path &path, const str &content = "") -> Optional<String>;
fn create_dir(const fs::path &path) -> Optional<String>;
fn remove_dir(const fs::path &path) -> Optional<String>;
fn parse_toml(const str &code) -> Vec<TomlEntry>;

fn new_project(const str &name) -> Optional<String>;
fn info() -> Optional<String>;
fn build(bool skip_tidying = false) -> Optional<String>;
fn clean() -> Optional<String>;
} // namespace fcppm
