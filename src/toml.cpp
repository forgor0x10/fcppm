#include <fcppm/fcppm.hpp>

#include <brief/aliases.hpp>

namespace {
fn split_string(const str &string, const str &delim) -> Vec<str> {
    mut tokens = Vec<str>();

    let delim_len = u64(delim.length());
    mut pos_start = u64(0);
    mut pos_end = u64(0);

    while ((pos_end = string.find(delim, pos_start)) != str::npos) {
        let token = string.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        tokens.push_back(token);
    }

    tokens.push_back(string.substr(pos_start));
    return tokens;
}

fn trim_string(const str &string) -> String {
    mut trimmed = String(string);

    while (!trimmed.empty() && trimmed.front() == ' ') { trimmed = trimmed.substr(1); }

    while (!trimmed.empty() && trimmed.back() == ' ') {
        trimmed = trimmed.substr(0, trimmed.length() - 1);
    }

    return trimmed;
}
} // namespace

fn fcppm::parse_toml(const str &code) -> Vec<fcppm::TomlEntry> {
    mut entries = Vec<fcppm::TomlEntry>();

    let lines = split_string(code, "\n");
    mut table = Optional<String>();

    for (let line in lines) {
        if (line.front() == '[' && line.back() == ']') {
            table = line.substr(1, line.length() - 2);
            continue;
        }

        let eq_pos = u64(line.find("="));

        if (eq_pos == str::npos) { continue; }

        let key = String(trim_string(line.substr(0, eq_pos)));
        mut value = String(trim_string(line.substr(eq_pos + 1)));

        if (value.front() == '\"' && value.back() == '\"') {
            value = value.substr(1, value.length() - 2);
        }

        entries.push_back({table, key, value});
    }

    return entries;
}
