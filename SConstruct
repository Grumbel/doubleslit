env = Environment(CXXFLAGS=[ "-O2", "-g3",
                                "-ansi",
                                "-pedantic",
                                "-Wall",
                                "-Wextra",
                                "-Wnon-virtual-dtor",
                                "-Weffc++",
                                "-Wconversion",
                                "-Wold-style-cast",
                                "-Werror",
                                "-Wshadow",
                                "-Wcast-qual",
                                "-Winit-self", # only works with >= -O1
                                "-Wno-unused-parameter"])
env.ParseConfig("pkg-config --cflags --libs gtkmm-3.0 | sed 's/-I/-isystem/g'")
env.Program("doubleslit", ["doubleslit.cpp", "main.cpp"])

# EOF #
