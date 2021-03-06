#
# Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)
project(doubleslit VERSION 0.0.0)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
pkg_search_module(GTKMM REQUIRED gtkmm-3.0 IMPORTED_TARGET)
pkg_search_module(CAIROMM REQUIRED cairomm-1.0 IMPORTED_TARGET)

add_executable(doubleslit
  src/doubleslit.cpp
  src/line.cpp
  src/main.cpp
  src/physics.cpp
  src/scenes.cpp
  )
target_include_directories(doubleslit PRIVATE src/)
target_link_libraries(doubleslit PUBLIC
  PkgConfig::GTKMM
  PkgConfig::CAIROMM)

install(TARGETS doubleslit
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

# EOF #

