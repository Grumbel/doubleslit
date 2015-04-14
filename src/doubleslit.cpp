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

#include <random>
#include <limits>
#include <boost/optional.hpp>
#include <vector>
#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <glibmm/main.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

#include "doubleslit.hpp"

Doubleslit::Doubleslit() :
  m_physics(),
  p1(0.0f, 0.0f),
  p2(0.0f, 1.0f),
  scroll()
{
  Glib::signal_timeout().connect( sigc::mem_fun(*this, &Doubleslit::on_timeout), 1000 );

  //Connect the signal handler if it isn't already a virtual method override:
  //  signal_draw().connect(sigc::mem_fun(*this, &Doubleslit::on_draw), false);
  add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
             Gdk::KEY_PRESS_MASK      | Gdk::KEY_RELEASE_MASK |
             Gdk::ENTER_NOTIFY_MASK   | Gdk::LEAVE_NOTIFY_MASK);

  signal_button_release_event().connect(sigc::mem_fun(this, &Doubleslit::on_button_release_event));
  signal_button_press_event().connect(sigc::mem_fun(this, &Doubleslit::on_button_press_event));
  signal_motion_notify_event().connect(sigc::mem_fun(this, &Doubleslit::on_motion_notify_event));
}

Doubleslit::~Doubleslit()
{
}

bool Doubleslit::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  //Gtk::Allocation allocation = get_allocation();
  //const int width = allocation.get_width();
  //const int height = allocation.get_height();

  // scale to unit square and translate (0, 0) to be (0.5, 0.5), i.e.
  // the center of the window
  //cr->scale(width, height);
  //cr->translate(0.5, 0.5);

  cr->translate(-scroll.x, -scroll. y);
  m_physics.draw(cr);

  return true;
}

bool Doubleslit::on_timeout()
{
  // force our program to redraw the entire clock.
  Glib::RefPtr<Gdk::Window> win = get_window();
  if (win)
  {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                     get_allocation().get_height());
    win->invalidate_rect(r, false);
  }
  return true;
}

bool
Doubleslit::on_key_release_event(GdkEventKey* ev)
{
  //std::cout << "key" << std::endl;
  return true;
}

bool
Doubleslit::on_motion_notify_event (GdkEventMotion*ev)
{
  //std::cout << "motion" << std::endl;
  return true;
}

bool
Doubleslit::on_button_release_event (GdkEventButton* ev)
{
  //std::cout << "button release" << std::endl;
  return true;
}

bool
Doubleslit::on_button_press_event (GdkEventButton* ev)
{
  if (ev->button == 1)
  {
    glm::vec2 dir = p2 - p1;

    p1.x = static_cast<float>(ev->x);
    p1.y = static_cast<float>(ev->y);

    p2 = p1 + dir;
  }
  else if (ev->button == 3)
  {
    p2.x = static_cast<float>(ev->x);
    p2.y = static_cast<float>(ev->y);
  }
  else if (ev->button == 2)
  {
    scroll += glm::vec2(ev->x - get_width()/2, ev->y - get_height()/2);
  }

  // scatter
  if (true)
  {
    std::mt19937 engine;
    //std::uniform_real_distribution<float> distribution(-1, 1);
    std::normal_distribution<float> distribution(0.0f, 0.2f);
    auto generator = std::bind(distribution, engine);

    m_physics.clear_rays();
    int n = 20;
    for(int i = 0; i < n; i += 1)
    {
      auto fn = [&]() -> float { return generator(); };
      /*
        auto fn = [&]() -> float {
        float p = ((static_cast<float>(i)/(n-1)) - 0.5f) * 2.0f;
        float o =0.5f;
        float norm = 1.0f - (pow(2.71828182846, -0.5 * pow(p/o, 2.0)) / 2.71828182846);
        return norm *p;
        };*/

      glm::vec2 dir = p2 - p1;
      dir = glm::rotate(dir, glm::radians(fn() * 10.0f));

      glm::vec2 off(fn() * -45.0f,
                    fn() * -45.0f);
      m_physics.cast_ray(p1+off, dir);
    }
  }

  // parallel rays
  if (false)
  {
    m_physics.clear_rays();
    int d = 20;
    for(int i = -d; i <= d; i += 1)
    {
      glm::vec2 dir = glm::normalize(p2 - p1);
      glm::vec2 off(-dir.y, dir.x);
      m_physics.cast_ray(p1 + 35.0f * static_cast<float>(i) * off, dir);
    }
  }

  queue_draw();

  //std::cout << "button press" << std::endl;
  return true;
}

/* EOF */
