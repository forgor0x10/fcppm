#include <filesystem>

#include <fcppm/fcppm.hpp>

#include <brief/brief.hpp>

fn fcppm::new_project(const str &name) -> Optional<String> {
    if (name.empty()) { return "Project name is absent or empty"; }

    constexpr let template_project_format = str("{project}");

    let gitignore_template = String("build\n"
                                    "target\n");

    mut fcpp_toml_template = String("[project]\n"
                                    "name = \"{project}\"\n"
                                    "description = \"A fcpp project\"\n"
                                    "version = \"1.0.0\"\n");

    fcpp_toml_template.replace(fcpp_toml_template.find(template_project_format),
                               template_project_format.length(), name);

    let clang_format_template = String("---\n"
                                       "BasedOnStyle: LLVM\n"
                                       "IndentWidth: 4\n"
                                       "ColumnLimit: 100\n"
                                       "AllowShortBlocksOnASingleLine: Always\n"
                                       "AllowShortIfStatementsOnASingleLine: Always\n"
                                       "AllowShortLoopsOnASingleLine: true\n");

    let clang_tidy_template =
        String("---\n"
               "Checks: >\n"
               "  bugprone-*,\n"
               "  misc-*,\n"
               "  modernize-*,\n"
               "  perfomance-*,\n"
               "  portability-*,\n"
               "  readability-*,\n"
               "WarningsAsErrors: \"*\"\n"
               "FormatStyle: file\n"
               "CheckOptions:\n"
               "  - { key: readability-identifier-naming.NamespaceCase, value: lower_case }\n"
               "  - { key: readability-identifier-naming.ClassCase, value: CamelCase }\n"
               "  - { key: readability-identifier-naming.StructCase, value: CamelCase }\n"
               "  - { key: readability-identifier-naming.FunctionCase, value: lower_case }\n"
               "  - { key: readability-identifier-naming.VariableCase, value: lower_case }\n"
               "  - { key: readability-identifier-naming.EnumCase, value: CamelCase }\n"
               "  - { key: readability-identifier-naming.ConstantCase, value: lower_case }\n");

    mut main_cpp_template = String("#include <brief/brief.hpp>\n"
                                   "\n"
                                   "fn main() -> i32 {\n"
                                   "    println(\"Hello {project}!\");\n"
                                   "    return 0;\n"
                                   "};\n");

    main_cpp_template.replace(main_cpp_template.find(template_project_format),
                              template_project_format.length(), name);

    let project_path = fs::path(name);

    try_fn_opt(fcppm::create_dir(project_path));
    try_fn_opt(fcppm::create_file(project_path / ".gitignore", gitignore_template));
    try_fn_opt(fcppm::create_file(project_path / "fcpp.toml", fcpp_toml_template));
    try_fn_opt(fcppm::create_file(project_path / ".clang-format", clang_format_template));
    try_fn_opt(fcppm::create_file(project_path / ".clang-tidy", clang_tidy_template));
    try_fn_opt(fcppm::create_dir(project_path / "src"));
    try_fn_opt(fcppm::create_file(project_path / "src" / "main.cpp", main_cpp_template));

    println("\033[36m> Success: \033[0mCreated project ", name);

    return none;
}
