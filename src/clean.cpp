#include <array>
#include <filesystem>

#include <fcppm/fcppm.hpp>

#include <brief/brief.hpp>

fn fcppm::clean() -> Optional<String> {
    let build_path = fs::path("build");
    let target_path = fs::path("target");

    mut cleaned = Array<bool, 2>();

    if (fs::exists(build_path)) {
        cleaned[0] = true;
        try_fn_opt(fcppm::remove_dir(build_path));
    }

    if (fs::exists(target_path)) {
        cleaned[1] = true;
        try_fn_opt(fcppm::remove_dir(target_path));
    }

    if (cleaned[0] || cleaned[1]) {
        if (cleaned[0]) { println("\033[30m> \033[0mCleaned build"); }
        if (cleaned[1]) { println("\033[30m> \033[0mCleaned target"); }
    } else {
        println("\033[30m> \033[0mNothing to clean");
    }

    return none;
}
