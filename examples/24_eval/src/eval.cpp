// Copyright 2025 Vladislav Aleinik
#include <expression.hpp>

#include <iostream>
#include <cstring>

int main(int argc, char* argv[])
{
    if (argc != 5 || std::strcmp(argv[1], "--x") != 0 || std::strcmp(argv[3], "--y") != 0)
    {
        printf("Usage: eval --x <x value> --y <y value>\n");
        return EXIT_FAILURE;
    }

    long double x = stold(std::string(argv[2]));
    long double y = stold(std::string(argv[4]));

    std::map<std::string, Value_t> input_context =
    {
        {"x", x}, {"y", y}
    };

    // Создаём объект выражения на основе механизма синтаксического соответствия.
    Expression expr1 = 100.0_val + "x"_var * 2.0_val;
    expr1 += "y"_var * "x"_var;

    // Задаём контексты для вычисления выражений.
    std::map<std::string, Value_t> context1 =
    {
        {"x", 1.0}, {"y", 2.0}
    };

    std::map<std::string, Value_t> context2 =
    {
        {"x", 2.0}, {"y", 3.0}
    };

    // Вычисляем выражение в различных контекстах.
    printf("EVAL[%s]{x = %.1Lf, y = %.1Lf} = %Lf\n", expr1.to_string().c_str(), x, y, expr1.eval(input_context));

    printf("EVAL[%s]{x = 1.0, y = 2.0} = %Lf\n", expr1.to_string().c_str(), expr1.eval(context1));
    printf("EVAL[%s]{x = 2.0, y = 3.0} = %Lf\n", expr1.to_string().c_str(), expr1.eval(context2));

    return EXIT_SUCCESS;
}
