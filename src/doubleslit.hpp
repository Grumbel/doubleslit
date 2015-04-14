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

#ifndef DOUBLESLIT_HPP
#define DOUBLESLIT_HPP

#include <gtkmm/drawingarea.h>

#include "physics.hpp"

class Doubleslit : public Gtk::DrawingArea
{
public:
  Doubleslit();
  virtual ~Doubleslit();

protected:
  //Override default signal handler:
  virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
  virtual bool on_key_release_event(GdkEventKey* event) override;
  virtual bool on_motion_notify_event (GdkEventMotion* event);
  virtual bool on_button_release_event (GdkEventButton* event);
  virtual bool on_button_press_event (GdkEventButton* event);
  bool on_timeout();

  Physics m_physics;
  glm::vec2 p1;
  glm::vec2 p2;
  glm::vec2 scroll;
};

#endif // DOUBLESLIT_HPP

/* EOF */
