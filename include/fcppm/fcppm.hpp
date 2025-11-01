#include <brief/aliases.hpp>
#include <brief/io.hpp>
#include <brief/result.hpp>

namespace fcppm {
constexpr let version = str("1.0.0");

struct TomlEntry {
    Optional<String> table;
    String key;
    String value;
};

fn create_file(const fs::Path &path, const str &content = "") -> Result<Unit, String>;
fn create_dir(const fs::Path &path) -> Result<Unit, String>;
fn remove_dir(const fs::Path &path) -> Result<Unit, String>;

fn parse_toml(const str &code) -> Vec<TomlEntry>;

fn new_project(const str &name) -> Result<Unit, String>;
fn info() -> Result<Unit, String>;
fn build(bool skip_tidying = false) -> Result<Unit, String>;
fn clean() -> Result<Unit, String>;
} // namespace fcppm
