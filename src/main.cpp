#include <fcppm/fcppm.hpp>

#include <brief/brief.hpp>

namespace {
struct CommandHelp {
    String summary;
    String usage;
    Vec<str> options;
};

fn print_help_for(const Vec<Pair<str, CommandHelp>> &cmds, const str &cmd = "") -> void {
    if (cmd.empty()) {
        constexpr let name_align = i32(8);

        println("fcppm - A lightweight C++ project manager");
        println();
        println("Usage: fcppm <command> [<args>]");
        println();
        println("Commands:");

        for (let & [ name, data ] in cmds) {
            println("    ", name, String(name_align - name.length(), ' '), data.summary);
        }

        println();
        println("Use 'fcppm help <command>' to read about a specific subcommand");
        return;
    }

    for (let &[ name, data ] : cmds) {
        if (name == cmd) {
            println(data.summary);
            println("\nUsage: ", data.usage);

            if (!data.options.empty()) {
                println("\nOptions:");
                for (let &line : data.options) { println("    ", line); }
            }

            return;
        }
    }

    eprintln("\033[31m> Error: \033[0mUnknown command \"", cmd, "\"");
}

fn run_command(const str &cmd, const Vec<str> &args) -> i32 {
    if (cmd == "info") {
        if (let err = fcppm::info(); err.has_value()) {
            eprintln("\033[31m> Error: \033[0m", err.value());
            return 1;
        }

        return 0;
    }

    if (cmd == "new") {
        mut name = String();

        for (mut i = u64(0); i + 1 < args.size(); ++i) {
            if (args[i] == "-n" || args[i] == "--name") { name = args[i + 1]; }
        }

        if (let err = fcppm::new_project(name); err.has_value()) {
            eprintln("\033[31m> Error: \033[0m", err.value());
            return 1;
        }

        return 0;
    }

    if (cmd == "build") {
        mut skip_tidying = false;

        for (mut i = u64(0); i + 1 < args.size(); ++i) {
            if (args[i] == "-s" || args[i] == "--skip-tidying") { skip_tidying = true; }
        }

        if (let err = fcppm::build(skip_tidying); err.has_value()) {
            eprintln("\033[31m> Error: \033[0m", err.value());
            return 1;
        }

        return 0;
    }

    if (cmd == "clean") {
        if (let err = fcppm::clean(); err.has_value()) {
            eprintln("\033[31m> Error: \033[0m", err.value());
            return 1;
        }

        return 0;
    }

    eprintln("\033[31m> Error: \033[0mUnrecognized command \"", cmd, "\"");
    return 1;
}
} // namespace

fn main(i32 argc, char *argv[]) -> i32 {
    let args = Vec<str>(argv + 1, argv + argc);
    println("\033[30mfcppm " + String(fcppm::version) + "\033[0m");

    let command_help = Vec<Pair<str, CommandHelp>>(
        {{"help", {"Show help", "fcppm help <command>", {}}},
         {"info", {"Show info about the current project", "fcppm info", {}}},
         {"new",
          {"Create a new project", "fcppm new [options]", {"-n, --name <name>    Project name"}}},
         {"build",
          {"Build a project", "fcppm build", {"-s, --skip-tidying    Skip clang-tidy checks"}}},
         {"clean", {"Clean a project's build files", "fcppm clean", {}}}});

    if (args.empty()) {
        print_help_for(command_help);
        return 0;
    }

    if (args[0] == "help") {
        if (args.size() < 2) {
            print_help_for(command_help);
        } else {
            print_help_for(command_help, args[1]);
        }

        return 0;
    }

    return run_command(args[0], args);
}
