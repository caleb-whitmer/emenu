{
    moduleinfo = "{\
\"rules\": [\
{\
\"primary-output\": \"build/.objs/a/linux/x86_64/release/src/test.cppm.o\",\
\"provides\": [\
{\
\"logical-name\": \"test\",\
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
    values = {
        "-m64",
        "-std=c++23",
        "-fmodules",
        "-D_GLIBCXX_USE_CXX11_ABI=1"
    },
    files = {
        "src/test.cppm"
    }
}