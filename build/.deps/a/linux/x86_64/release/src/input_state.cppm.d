{
    values = {
        "-m64",
        "-std=c++23",
        "-isystem",
        "/usr/include/freetype2",
        "-isystem",
        "/usr/include/libpng16",
        "-fmodules",
        "-D_GLIBCXX_USE_CXX11_ABI=1"
    },
    moduleinfo = "{\
\"rules\": [\
{\
\"primary-output\": \"build/.objs/a/linux/x86_64/release/src/input_state.cppm.o\",\
\"provides\": [\
{\
\"logical-name\": \"UserInputState\",\
\"is-interface\": true\
}\
],\
\"requires\": [\
{\
\"logical-name\": \"std\"\
}\
]\
}\
],\
\"version\": 0,\
\"revision\": 0\
}\
",
    files = {
        "src/input_state.cppm"
    }
}