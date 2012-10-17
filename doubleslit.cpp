#include <random>
#include <limits>
#include <boost/optional.hpp>
#include <vector>
#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <glm/ext.hpp>

#include "doubleslit.hpp"

Physics::Physics() :
  m_segments(),
  m_rays()
{
  if (true)
  {
    add_rect(100, 400, 200, 10);
    add_rect(304, 400, 2, 10);
    add_rect(310, 400, 200, 10);
  }

  if (false)
    for(int i = 0; i < 30; ++i)
    {
      add_rect(rand()%600, rand()%600, rand()%200, rand()%200);
    }

  if (false)
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
        m_segments.push_back(Line(last, p));
      }
      last = p;
    }
  }

  // parabola
  if (false)
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
        m_segments.push_back(Line(last, p));
      }
      last = p;
    }
  }
  
  cast_ray(glm::vec2(205.0f, 50.0f), glm::normalize(glm::vec2(0.1, 1.0f)));
}

void
Physics::clear_rays()
{
  m_rays.clear();
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
    depth += 1;
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
    cast_ray(colpos, glm::reflect(ray.dir, glm::normalize(glm::vec2(-seg_dir.y, seg_dir.x))), depth);

    m_rays.push_back(Line(ray.pos, colpos));
  }
  else
  {
    m_rays.push_back(Line(ray.pos, ray.pos + ray.dir * 100000.0f));
  }
}

void
Physics::draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  cr->set_source_rgb(0,0,0);
  cr->paint();

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
    cr->set_source_rgba(0, 1, 0, 0.02);
    cr->move_to(ray.p1.x, ray.p1.y);
    cr->line_to(ray.p2.x, ray.p2.y);
    cr->stroke();

    // collision point
    if (false)
    {
    cr->set_source_rgba(1, 0, 0, 1.0);
    cr->arc(ray.p1.x, ray.p1.y, 4.0, 0.0, 2.0 * M_PI);
    cr->fill();
    }
  }
}

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
  std::cout << "key" << std::endl;
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
  std::cout << "button release" << std::endl;
  return true;
}

bool
Doubleslit::on_button_press_event (GdkEventButton* ev)
{
  bool recast = false;
  if (ev->button == 1)
  {
    glm::vec2 dir = p2 - p1;

    p1.x = ev->x;
    p1.y = ev->y;

    p2 = p1 + dir;
    recast = true;
  }
  else if (ev->button == 3)
  {
    p2.x = ev->x;
    p2.y = ev->y;
    recast = true;
  }
  else if (ev->button == 2)
  {
    scroll += glm::vec2(ev->x - get_width()/2, ev->y - get_height()/2);
  }

  if (recast)
  {
  std::mt19937 engine;
  //std::uniform_real_distribution<float> distribution(-1, 1);
  std::normal_distribution<float> distribution(0.0f, 0.2);
  auto generator = std::bind(distribution, engine);

  // scatter
  if (true)
  {
    m_physics.clear_rays();
    int n = 50;
    for(int i = 0; i < n; i += 1)
    {
      auto fn = [&]() -> float {return generator(); };
      /*
      auto fn = [&]() -> float { 
        float p = ((static_cast<float>(i)/(n-1)) - 0.5f) * 2.0f;
        float o =0.5f;
        float norm = 1.0f - (pow(2.71828182846, -0.5 * pow(p/o, 2.0)) / 2.71828182846);
        return norm *p;
        };*/
      
      glm::vec2 dir = p2 - p1;
      dir = glm::rotate(dir, fn() * 5.0f);

      glm::vec2 off(fn() * -40.0f,
                    fn() * -40.0f);
      m_physics.cast_ray(p1 + off, dir);
    }
  }

  // parallel rays
  if (false)
  {
    m_physics.clear_rays();
    int d = 20;
    for(int i = -d; i <= d; i += 3)
    {
      glm::vec2 dir = glm::normalize(p2 - p1);
      glm::vec2 off(-dir.y, dir.x);
      m_physics.cast_ray(p1 + 35.0f * static_cast<float>(i) * off, dir);
    }
  } 
  }
  queue_draw();

  std::cout << "button press" << std::endl;
  return true;
}

/* EOF */
