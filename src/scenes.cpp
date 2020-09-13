// Doubleslit - A 2D ray casting toy
// Copyright (C) 2012-2020 Ingo Ruhnke <grumbel@gmail.com>
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

#include "scenes.hpp"

#include <math.h>
#include <glm/glm.hpp>

#include "line.hpp"
#include "physics.hpp"

void scene_create_doubleslit(Physics& phy)
{
  phy.add_rect(100, 400, 200, 1);
  phy.add_rect(304, 400, 2, 1);
  phy.add_rect(310, 400, 200, 1);
}

void scene_create_rectangles(Physics& phy)
{
  for(int i = 0; i < 30; ++i)
  {
    phy.add_rect(static_cast<float>(rand()%600),
                 static_cast<float>(rand()%600),
                 static_cast<float>(rand()%200),
                 static_cast<float>(rand()%200));
  }
}

void scene_create_half_circle(Physics& phy)
{
  glm::vec2 p;
  glm::vec2 last;
  float r = 300.0f;
  for(float x = -r; x <= r; x += 1.0f)
  {
    p.x = x;
    p.y = sqrtf(r*r - x*x);

    //p *= 50.0f;
    p += glm::vec2(400, 400);

    if (x != -r)
    {
      phy.add_line(Line(last, p));
    }
    last = p;
  }
}

void scene_create_parabola(Physics& phy)
{
  glm::vec2 p;
  glm::vec2 last;
  float start = -10.0f;
  float end   =  10.0f;
  for(float x = start; x <= end; x += 0.1f)
  {
    p.x = x;
    p.y = -(x*x) * 0.1f;

    p *= 50.0f;
    p += glm::vec2(400, 600);

    if (x != start)
    {
      phy.add_line(Line(last, p));
    }
    last = p;
  }
}

/* EOF */
