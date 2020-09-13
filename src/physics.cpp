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

#include "physics.hpp"

Physics::Physics() :
  m_segments(),
  m_rays()
{
  //cast_ray(glm::vec2(205.0f, 50.0f), glm::normalize(glm::vec2(0.1, 1.0f)));
}

void
Physics::clear_rays()
{
  m_rays.clear();
}

void
Physics::clear_geometry()
{
  m_segments.clear();
}

void
Physics::add_line(Line const& line)
{
  m_segments.push_back(line);
}

void
Physics::add_rect(float x, float y, float w, float h)
{
  glm::vec2 p1(x, y);
  glm::vec2 p2(x+w, y);
  glm::vec2 p3(x+w, y+h);
  glm::vec2 p4(x, y+h);

  m_segments.push_back(Line(p1, p2));
  m_segments.push_back(Line(p2, p3));
  m_segments.push_back(Line(p3, p4));
  m_segments.push_back(Line(p4, p1));
}

bool intersect(const Ray& ray, const Line& seg, float& u)
{
  const float x1 = ray.pos.x;
  const float y1 = ray.pos.y;

  const float x3 = seg.p1.x;
  const float y3 = seg.p1.y;

  const float x4 = seg.p2.x;
  const float y4 = seg.p2.y;

  float quotient = ((y4 - y3) * (ray.dir.x) - (x4 - x3) * (ray.dir.y));

  if (quotient == 0.0f)
  {
    return false; // FIXME
  }
  else
  {
    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / quotient;
    float ub = ((ray.dir.x) * (y1 - y3) - (ray.dir.y) * (x1 - x3)) / quotient;

    //return (ua >= 0.0f && ua <= 1.0f);
    u = ua;
    return (ub >= 0.0f && ub <= 1.0f);
  }
}

void
Physics::cast_ray(const glm::vec2& position, const glm::vec2& direction, int depth)
{
  if (depth < 10)
  {
    Ray ray{position, direction};
    find_collision(ray, depth);
  }
}

void
Physics::find_collision(const Ray& ray, int depth)
{
  bool have_match = false;
  float nearest_match = std::numeric_limits<float>::max();
  Line segment;
  for(auto& seg : m_segments)
  {
    float t = std::numeric_limits<float>::max();
    if (intersect(ray, seg, t) && t > 0.001)
    {
      if (!have_match)
      {
        have_match = true;
        nearest_match = t;
        segment = seg;
      }
      else if (t < nearest_match)
      {
        nearest_match = t;
        segment = seg;
      }
    }
  }

  //std::cout << "have match: " << have_match << std::endl;

  if (have_match)
  {
    glm::vec2 seg_dir = segment.p2 - segment.p1;
    glm::vec2 colpos = ray.pos + ray.dir * nearest_match;
    //std::cout << "best: " << nearest_match << " = " << colpos.x << " " << colpos.y << std::endl;
    cast_ray(colpos, glm::reflect(ray.dir, glm::normalize(glm::vec2(-seg_dir.y, seg_dir.x))), depth+1);

    m_rays.push_back(RayLine{Line(ray.pos, colpos), depth});
  }
  else
  {
    m_rays.push_back(RayLine{Line(ray.pos, ray.pos + glm::normalize(ray.dir) * 10000.0f), depth});
  }
}

void
Physics::draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  cr->set_antialias(Cairo::ANTIALIAS_NONE);

  cr->set_source_rgb(0,0,0);
  cr->paint();

  cr->set_operator(Cairo::OPERATOR_ADD);
  cr->set_source_rgb(1,1,1);
  cr->set_line_width(1.0);
  for(auto& seg : m_segments)
  {
    cr->move_to(seg.p1.x, seg.p1.y);
    cr->line_to(seg.p2.x, seg.p2.y);
  }
  cr->stroke();

  cr->set_line_width(4.0);
  for(auto& ray : m_rays)
  {
    float alpha = 0.2f;
    if (ray.depth == 0)
      cr->set_source_rgba(0.1, 1, 0.1, alpha);
    else if (ray.depth == 1)
      cr->set_source_rgba(1, 0.1, 0.1, alpha);
    else
      cr->set_source_rgba(0.1, 0.1, 1, alpha);

    cr->move_to(ray.line.p1.x, ray.line.p1.y);
    cr->line_to(ray.line.p2.x, ray.line.p2.y);
    cr->stroke();

    // collision point
    if (false)
    {
      cr->set_source_rgba(1, 0, 0, 1.0);
      cr->arc(ray.line.p1.x, ray.line.p1.y, 4.0, 0.0, 2.0 * M_PI);
      cr->fill();
    }
  }
}

/* EOF */
