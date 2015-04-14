#ifndef HEADER_PHYSICS_HPP
#define HEADER_PHYSICS_HPP

#include <vector>
#include <cairomm/context.h>

#include "line.hpp"

struct RayLine
{
  Line line;
  int depth;
};

class Ray
{
public:
  glm::vec2 pos;
  glm::vec2 dir;
};

class Physics
{
private:
  std::vector<Line> m_segments;
  std::vector<RayLine> m_rays;

public:
  Physics();

  void add_rect(float x, float y, float w, float h);
  void cast_ray(const glm::vec2& position, const glm::vec2& direction, int depth = 0);
  void find_collision(const Ray& ray, int depth);
  void draw(const Cairo::RefPtr<Cairo::Context>& cr);
  void clear_rays();
};

#endif

/* EOF */
