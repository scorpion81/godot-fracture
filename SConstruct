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
env.Append(CPPPATH=['src/',
                    'external/dist/voro/include/',
                    'external/dist/pycork/include/',
                    'external/dist/pycork/include/cork/',
                    'external/dist/pycork/include/eigen/',
                   ]),
sources = Glob("src/*.cpp")

if env["target"] == "template_debug":
    env.Append(CPPDEFINES=["DEBUG_ENABLED", "DEBUG_METHODS_ENABLED"])

if env["platform"] == "windows":
    env.Append(CPPDEFINES=["_USE_MATH_DEFINES"])

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libfracture.{}.{}.framework/libfracture.{}.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"], env["arch"]
        ),
        source=sources,
        LIBPATH=['external/dist/voro/lib','external/dist/pycork/lib/cork'],
        LIBS=['voro++', 'cork'],
    )
elif env["platform"] == "windows":
    library = env.SharedLibrary(
        "demo/bin/libfracture{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
        LIBPATH=['external/dist/voro/lib','external/dist/pycork/lib/cork'],
        LIBS=['voro++', 'cork'],
        LINKFLAGS=['/WHOLEARCHIVE:voro++.lib', '/WHOLEARCHIVE:cork.lib']
    )
else:
    library = env.SharedLibrary(
        "demo/bin/libfracture{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
        LIBPATH=['external/dist/voro/lib','external/dist/pycork/lib/cork'],
        LIBS=['voro++', 'cork']
    )

Default(library)
