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
   Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

   Gtk::Window win;
   win.set_size_request(768, 768);
   win.set_title("Cairomm Clock");

   Clock c;
   win.add(c);
   c.show();

   return app->run(win);
}

/* EOF */
