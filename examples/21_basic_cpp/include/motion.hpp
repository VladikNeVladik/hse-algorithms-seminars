#pragma once

#include <vector>
#include <string>

class Motion
{
public:
    Motion(double vx, double vy, double scale);

    void apply(double &x, double &y) const;
private:
    double get_velocity_x() const;

    double get_velocity_y() const;

    double get_velocity_scale() const;

    double vx_;
    double vy_;
    double scale_;
};

std::string calculate_motion_result(const std::vector<Motion>& motions, double x, double y);
