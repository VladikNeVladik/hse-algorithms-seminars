// Copyright 2024 Vladislav Aleinik

#include <objects.hpp>

#include <cmath>

//-----------------
// Physical object
//-----------------

Object::Object(Pos x, Pos y, Pos sz_x, Pos sz_y, Vel v_x, Vel v_y) :
    x_    (x),    y_    (y),
    sz_x_ (sz_x), sz_y_ (sz_y),
    v_x_  (v_x),  v_y_  (v_y)
{ }

void Object::move(Time_t dt)
{
    x_ += v_x_ * dt;
    y_ += v_y_ * dt;
}

void Object::accelerate(Acc a_x, Acc a_y, Time_t dt)
{
    v_x_ += a_x * dt;
    v_y_ += a_y * dt;
}

void Object::dissipate(double koeff, Time_t dt)
{
    v_y_ *= std::exp(-koeff * dt);
    v_x_ *= std::exp(-koeff * dt);
}

void Object::wall_bounce(Pos wall_x0, Pos wall_y0, Pos wall_x1, Pos wall_y1)
{
    // Resolve collision and update velocity:
    if (x_ < wall_x0)
    {
        x_   = wall_x0;
        v_x_ = +std::fabs(v_x_);
    }
    if (y_ < wall_y0)
    {
        y_   = wall_y0;
        v_y_ = +std::fabs(v_y_);
    }
    if (x_ + sz_x_ > wall_x1)
    {
        x_   = wall_x1 - sz_x_;
        v_x_ = -std::fabs(v_x_);
    }
    if (y_ + sz_y_ > wall_y1)
    {
        y_   = wall_y1 - sz_y_;
        v_y_ = -std::fabs(v_y_);
    }
}

//----------------
// Massive object
//----------------

MassiveObject::MassiveObject(Pos x, Pos y, Pos sz_x, Pos sz_y, Vel v_x, Vel v_y, Mass mass) :
    Object(x, y, sz_x, sz_y, v_x, v_y),
    mass_ (mass)
{ }

void MassiveObject::apply_force(Force f_x, Force f_y, Time_t dt)
{
    accelerate(f_x / mass_, f_y / mass_, dt);
}

//------------------
// Object rendering
//------------------

RenderedObject::RenderedObject(Pos x, Pos y, Pos sz_x, Pos sz_y, Vel v_x, Vel v_y, Mass mass) :
    MassiveObject(x, y, sz_x, sz_y, v_x, v_y, mass)
{ }

Round::Round(Pos x, Pos y, Pos diameter, Vel v_x, Vel v_y, Mass mass) :
    RenderedObject(x, y, diameter, diameter, v_x, v_y, mass)
{ }

Round::~Round()
{
    printf("~Round() is called\n");
}

void Round::render(Renderer& renderer)
{
    renderer.draw_circle(x_, y_, sz_x_);
}

Square::Square(Pos x, Pos y, Pos sz_x, Pos sz_y, Vel v_x, Vel v_y, Mass mass) :
    RenderedObject(x, y, sz_x, sz_y, v_x, v_y, mass)
{ }

Square::~Square()
{
    printf("~Square() is called\n");
}

void Square::render(Renderer& renderer)
{
    renderer.draw_rect(x_, y_, sz_x_, sz_y_);
}
