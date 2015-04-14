// Doubleslit - A 2D ray casting toy
// Copyright (C) 2012, 2015  Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <glm/glm.hpp>
#include <gtkmm.h>
#include <gtkmm/window.h>
#include <iostream>

#include "doubleslit.hpp"

//            ----X-----
//
// -----------+  +-+  +-----
//            |  | |  |
// -----------+  +-+  +-----

int main(int argc, char** argv)
{
   Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.doubleslit");

   Gtk::Window win;
   win.set_size_request(768, 768);
   win.set_title("Cairomm Clock");

   Doubleslit c;
   win.add(c);
   c.show();

   return app->run(win);
}

/* EOF */
