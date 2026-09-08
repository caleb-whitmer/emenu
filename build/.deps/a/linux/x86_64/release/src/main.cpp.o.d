{
    values = {
        "/usr/sbin/g++",
        {
            "-m64",
            "-std=c++23",
            "-isystem",
            "/usr/include/freetype2",
            "-isystem",
            "/usr/include/libpng16",
            "-fmodules",
            "-D_GLIBCXX_USE_CXX11_ABI=1",
            "-fmodule-mapper=/tmp/.xmake1000/260908/9b3e954ff264109e320c5805be904bee/main.cpp.mapper.txt"
        }
    },
    depfiles_format = "gcc",
    files = {
        "src/main.cpp"
    },
    depfiles = "build/.objs/a/linux/x86_64/release/src/main.cpp.o: src/main.cpp  /tmp/.xmake1000/260908/9b3e954ff264109e320c5805be904bee/main.cpp.mapper.txt  /home/seth/Development/emenu/build/.gens/a/linux/x86_64/release/rules/bmi/cache/interfaces/e9968aa214a321aa/std.gcm  /home/seth/Development/emenu/build/.gens/a/linux/x86_64/release/rules/bmi/cache/interfaces/99961de2887a9b53/UserInputState.gcm\
build/.objs/a/linux/x86_64/release/src/main.cpp.o: std.c++-module  UserInputState.c++-module\
CXX_IMPORTS += std.c++-module UserInputState.c++-module\
",
    lastmtime = 1788898180
}