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
\"primary-output\": \"build/.objs/a/linux/x86_64/release/src/main.cpp.o\",\
\"requires\": [\
{\
\"logical-name\": \"std\"\
}\
,\
{\
\"logical-name\": \"UserInputState\"\
}\
]\
}\
],\
\"version\": 0,\
\"revision\": 0\
}\
",
    files = {
        "src/main.cpp"
    }
}