// No copyright. Vladislav Aleinik, 2024
//=======================================
// A X11-based renderer
//=======================================
#ifndef HEADER_GUARD_OBJECTS_HPP_INCLUDED
#define HEADER_GUARD_OBJECTS_HPP_INCLUDED

#include <renderer.hpp>

//----------------------------
// Physical values dimensions
//----------------------------

typedef double Time_t;
typedef double Pos;
typedef double Vel;
typedef double Acc;
typedef double Mass;
typedef double Force;

//-----------------
// Physical object
//-----------------

class Object
{
public:
    Object(Pos x, Pos y, Pos sz_x, Pos sz_y, Vel v_x, Vel v_y);

    void move(Time_t dt);
    void accelerate(Acc a_x, Acc a_y, Time_t dt);
    void dissipate(double koeff, Time_t dt);
    void wall_bounce(Pos x0, Pos y0, Pos x1, Pos y1);

protected:
    Pos x_, y_;
    Pos sz_x_, sz_y_;
    Vel v_x_, v_y_;
};

//----------------
// Massive object
//----------------

class MassiveObject : public Object
{
public:
    MassiveObject(Pos x, Pos y, Pos sz_x, Pos sz_y, Vel v_x, Vel v_y, Mass mass);

    void apply_force(Force f_x, Force f_y, Time_t dt);

private:
    Mass mass_;
};

//------------------
// Object rendering
//------------------

class RenderedObject : public MassiveObject
{
public:
    RenderedObject(Pos x, Pos y, Pos sz_x, Pos sz_y, Vel v_x, Vel v_y, Mass mass);

    virtual void render(Renderer& renderer) = 0;

    virtual ~RenderedObject() = default;
};

class Round : public RenderedObject
{
public:
    Round(Pos x, Pos y, Pos diameter, Vel v_x, Vel v_y, Mass mass);

    virtual void render(Renderer& renderer) override;

    virtual ~Round() override;
};

class Square : public RenderedObject
{
public:
    Square(Pos x, Pos y, Pos sz_x, Pos sz_y, Vel v_x, Vel v_y, Mass mass);

    virtual void render(Renderer& renderer) override;

    virtual ~Square() override;
};

#endif // HEADER_GUARD_OBJECTS_HPP_INCLUDED
