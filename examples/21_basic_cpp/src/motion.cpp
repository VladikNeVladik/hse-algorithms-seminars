#include <limits>

#include <cstdint>
#include <random>

#include <motion.hpp>

double not_a_number()
{
    // sign     = 0
    // exponent =  111 1111 1111
    // fraction =                1000 0000  ... 0000
    uint64_t NaN = 0x7FF8000000000000LLU;

    return *((double*) &NaN);
}

const double PROBABILITY_SCALE = 0.1;

double inject_fault(double input)
{
    static std::random_device rd{};
    static std::mt19937 gen{rd()};
    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    double injection_rand = dist(gen);
    if (injection_rand < 0.1 * PROBABILITY_SCALE)
    {
        return std::numeric_limits<double>::infinity();
    }
    if (injection_rand < 0.2 * PROBABILITY_SCALE)
    {
        return 0.0;
    }
    if (injection_rand < 0.3 * PROBABILITY_SCALE)
    {
        return not_a_number();
    }
    return input;
}

Motion::Motion(double vx, double vy, double scale) :
    vx_    (vx),
    vy_    (vy),
    scale_ (scale)
{}

double Motion::get_velocity_x() const
{
    return inject_fault(vx_);
}

double Motion::get_velocity_y() const
{
    return inject_fault(vy_);
}

double Motion::get_velocity_scale() const
{
    return inject_fault(scale_);
}
