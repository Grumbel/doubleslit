#ifndef GTKMM_EXAMPLE_CLOCK_H
#define GTKMM_EXAMPLE_CLOCK_H

#include <gtkmm/drawingarea.h>

#include "line.hpp"

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
  std::vector<Line> m_rays;

public:
  Physics();

  void add_rect(float x, float y, float w, float h);
  void cast_ray(const glm::vec2& position, const glm::vec2& direction, int depth = 0);
  void find_collision(const Ray& ray, int depth);
  void draw(const Cairo::RefPtr<Cairo::Context>& cr);
  void clear_rays();
};

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

#endif // GTKMM_EXAMPLE_CLOCK_H
