{
    files = {
        "/usr/include/c++/16/bits/std.compat.cc"
    },
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
\"primary-output\": \"build/.objs/a/linux/x86_64/release/usr/include/c++/16/bits/std.compat.cc.o\",\
\"provides\": [\
{\
\"logical-name\": \"std.compat\",\
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
"
}