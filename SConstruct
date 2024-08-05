#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/", "src/voro/src/"])
sources = Glob("src/*.cpp")
sources.extend(Glob("src/voro/src/*.cc"))
sources = [s for s in sources if not "_wl" in s.name]
sources = [s for s in sources if not "voro++" in s.name]
names = [s.name for s in sources]
print(names)

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libfracture.{}.{}.framework/libfracture.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demo/bin/libfracture{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
