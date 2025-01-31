// Copyright 2025 Vladislav Aleinik

// Данный файл демонстрирует основные возможности C++,
// необходимые для решения задач контестов №1 и №2.

#include <cinttypes>
#include <cstdio>

#include <algorithm>
#include <vector>
#include <string_view>
#include <functional>
#include <stdexcept>
#include <cmath>

#include <motion.hpp>

//======================//
// Контест №1, задача A //
//======================//

struct Date {
    int32_t year;
    uint8_t month;
    uint8_t day;
};

void sort_dates(std::vector<Date>& dates, std::string_view order)
{
    auto compare_dates =
        [&](const Date& a, const Date& b) -> bool
        {
            for (char ch : order)
            {
                switch (ch)
                {
                case 'y':
                    if (a.year != b.year)
                    {
                        return a.year < b.year;
                    }
                    break;
                case 'm':
                    if (a.month != b.month)
                    {
                        return a.month < b.month;
                    }
                    break;
                case 'd':
                    if (a.day != b.day)
                    {
                        return a.day < b.day;
                    }
                    break;
                }
            }

            return false;
        };

    std::sort(dates.begin(), dates.end(), compare_dates);
}

void print_dates(const std::vector<Date>& dates, std::string_view order)
{
    printf("Dates in order \"%s\":\n", order.data());
    for (auto date : dates)
    {
        printf("y%04d m%02d d%02d\n", date.year, date.month, date.day);
    }
    printf("\n");
}

//======================//
// Контест №2, задача C //
//======================//

void Motion::apply(double &x, double &y) const
{
    double vx    = get_velocity_x();
    double vy    = get_velocity_y();
    double scale = get_velocity_scale();

    if (std::isinf(vx) || std::isnan(vx))
    {
        throw std::runtime_error("get_velocity_x return " + std::to_string(vx));
    }

    if (std::isinf(vy) || std::isnan(vy))
    {
        throw std::runtime_error("get_velocity_y return " + std::to_string(vy));
    }

    if (std::isinf(scale) || std::isnan(scale) || std::abs(scale) < 1e-10)
    {
        throw std::runtime_error("get_velocity_scale return " + std::to_string(scale));
    }

    x += vx / scale;
    y += vy / scale;
}

std::string calculate_motion_result(const std::vector<Motion>& motions, double x, double y)
{

    for (size_t i = 0; i < motions.size(); ++i)
    {
        try
        {
            motions[i].apply(x, y);
        }
        catch (const std::runtime_error& exc)
        {
            return std::string("Error on motion ") + std::to_string(i) + std::string(": ") + exc.what();

        }
    }

    return std::to_string(x) + " " + std::to_string(y);
}

int main(void)
{
    //----------------------//
    // Контест №1, задача A //
    //----------------------//

    std::vector<Date> dates =
    {
        {.year = 2000, .month = 12, .day = 23},
        {.year = 2000, .month = 11, .day = 23},
        {.year = 2000, .month = 11, .day = 15},
        {.year = 2000, .month = 11, .day = 15},
        {.year = 2001, .month =  2, .day = 13},
        {.year = 1998, .month =  3, .day =  3},
        {.year = 2005, .month = 10, .day = 30},
        {.year = 1997, .month =  6, .day =  5},
        {.year = 1996, .month =  6, .day =  5},
        {.year = 1993, .month =  1, .day = 18},
        {.year = 1984, .month =  8, .day =  1},
        {.year = 1981, .month =  8, .day =  1},
    };

    sort_dates(dates, "ymd");
    print_dates(dates, "ymd");

    sort_dates(dates, "dmy");
    print_dates(dates, "dmy");

    //----------------------//
    // Контест №1, задача A //
    //----------------------//

    std::vector<Motion> motions =
    {
        Motion( 1.0,  0.0, 1.0),
        Motion( 0.0,  1.0, 1.0),
        Motion(-1.0,  0.0, 1.0),
        Motion( 0.0, -1.0, 1.0),
        Motion( 2.0,  0.0, 1.0),
        Motion( 0.0,  2.0, 1.0),
        Motion(-2.0,  0.0, 1.0),
        Motion( 0.0, -2.0, 1.0),
        Motion( 3.0,  0.0, 1.0),
        Motion( 0.0,  3.0, 1.0),
        Motion(-3.0,  0.0, 1.0),
        Motion( 0.0, -3.0, 1.0)
    };

    printf("Iteration 1: %s\n", calculate_motion_result(motions, 1.0, 1.0).c_str());
    printf("Iteration 2: %s\n", calculate_motion_result(motions, 2.0, 2.0).c_str());
    printf("Iteration 3: %s\n", calculate_motion_result(motions, 3.0, 3.0).c_str());
    printf("Iteration 4: %s\n", calculate_motion_result(motions, 4.0, 4.0).c_str());
    printf("Iteration 5: %s\n", calculate_motion_result(motions, 5.0, 5.0).c_str());
}
