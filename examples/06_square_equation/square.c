#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

//==================================================================================================
// Функция: double_equal
// Назначение: сравнивает два числа с плавающей точкой двойной точности на предмет равенства
// с заданной точностью
//--------------------------------------------------------------------------------------------------
// Параметры:
// first (in) - первое число с плавающей точкой
// second (in) - второе число с плавающей точкой
// epsilon (in) - точность, используемая при сравнении чисел
//
// Возвращаемое значение: индикатор равенства двух чисел
//
// Используемые внешние переменные: отсутствуют
//
// Примечания:
// На использование функции определены следующие ограничения:
// - Аргументы first, second, epsilon задают конечные числа.
//==================================================================================================
bool double_equal(double first, double second, double epsilon)
{
    return fabs(first - second) < epsilon;
}

// Возможные возвращаемые значения
#define NO_ROOTS  0
#define ONE_ROOT  1
#define TWO_ROOTS 2
#define INF_ROOTS 3

// Точность входных значений ф-ии solve_square
#define EPSILON 1.0e-9

//==================================================================================================
// Функция: solve_square
// Назначение: вычисляет действительные корни квадратного уравнения по заданным коэффициентам
//--------------------------------------------------------------------------------------------------
// Параметры:
// a (in) - коэффициент при старшем члене квадратного уравнения (при x^2)
// b (in) - коэффициент при среднем члене квадратного уравнения (при x)
// c (in) - свободный член квадратного уравнения
// x0 (out) - указатель на место под первый корень квадратного уравнения
// x1 (out) - указатель на место под второй корень квадратного уравнения
//
// Возвращаемое значение: количество корней квадратного уравнения
// - NO_ROOTS  - у уравнения нет действительных решений
// - ONE_ROOT  - у уравнения одно решение
// - TWO_ROOTS - у уравнения два решения
// - INF_ROOTS - у уравнения бесконечность решений
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - На использование функции определены следующие ограничения:
//   - Аргументы a, b, c задают конечные числа.
//   - Указатели x0 и x1 доступны на запись.
// - Значение по указателю x0 перезаписывается, если возвращаемое значение равно ONE_ROOT или TWO_ROOTS.
// - Значение по указателю x1 перезаписывается, если возвращаемое значение равное TWO_ROOTS.
//==================================================================================================
unsigned solve_square(double a, double b, double c, double* x0, double* x1)
{
    assert(x0 != NULL);
    assert(x1 != NULL);

    // Проверяем степень уравнения
    if (double_equal(a, 0.0, EPSILON))
    { // Уравнение является линейным
        // Проверяем линейное уравнение на наличие решения
        if (double_equal(b, 0.0, EPSILON))
        {
            // Проверяем уравнение нулевой степени на наличие решений
            if (double_equal(c, 0.0, EPSILON))
            {
                // У уравнения бесконечное число решений
                return INF_ROOTS;
            }

            // У уравнения нет решений
            return NO_ROOTS;
        }

        // Линейное уравнение имеет одно решение
        *x0 = -c / b;
        return ONE_ROOT;
    }

    // Это квадратное уравнение
    // Вычисляем дискриминант:
    double D = b*b - 4*a*c;

    // Определяем кол-во корней уравнения
    if (D < 0)
    { // У уравнения нет действительных корней
        return NO_ROOTS;
    }
    else if (double_equal(D, 0.0, EPSILON))
    { // У уравнения ровно один корень
        *x0 = -b / 2.0 / a;
        return ONE_ROOT;
    }
    else
    { // У уравнения ровно два действительных решения
        double sqrtD = sqrt(D);

        *x0 = (-b - sqrtD) / 2.0 / a;
        *x1 = (-b + sqrtD) / 2.0 / a;
        return TWO_ROOTS;
    }
}

int main(void)
{
    double a = 0.0, b = 0.0, c = 0.0;

    printf("Enter square equation coefficients:\n");

    // Получаем коэффициенты квадратного уравнения от пользователя:
    printf("A = ");
    scanf("%lf", &a);

    printf("B = ");
    scanf("%lf", &b);

    printf("C = ");
    scanf("%lf", &c);

    // Вычисляем корни и печатаем корни квадратного уравнения:
    double x0, x1;
    unsigned num_roots = solve_square(a, b, c, &x0, &x1);

    switch (num_roots)
    {
        case NO_ROOTS:
        {
            printf("Equation has no roots!\n");
            break;
        }
        case ONE_ROOT:
        {
            printf("Equation has one root: %lf\n", x0);
            break;
        }
        case TWO_ROOTS:
        {
            printf("Equation has two roots: %lf and %lf\n", x0, x1);
            break;
        }
        case INF_ROOTS:
        {
            printf("Equation has infinite roots!\n");
            break;
        }
    }

    return 0;
}
