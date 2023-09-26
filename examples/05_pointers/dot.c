#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int read_vector(double* x_ptr, double* y_ptr)
{
    assert(x_ptr);
    assert(y_ptr);

    double length, angle;
    int ret = scanf("%lf %lf", &length, &angle);
    if (ret != 2)
    {
        return 1;
    }

    angle *= M_PI / 180.0f;

    *x_ptr = fabs(length) * cos(angle);
    *y_ptr = fabs(length) * sin(angle);

   return 0;
}

int main(void)
{
    double x1 = 0, y1 = 0;
    int ret = read_vector(&x1, &y1);
    if (ret != 0)
    {
        printf("Unable to parse first vector (length-value format)! \n");
        return EXIT_FAILURE;
    }

    double x2 = 0, y2 = 0;
    ret = read_vector(&x2, &y2);
    if (ret != 0)
    {
        printf("Unable to parse second vector (length-value format)!\n");
        return EXIT_FAILURE;
    }

    printf("Dot product is: %.3lf\n", x1*x2 + y1*y2);

    return EXIT_SUCCESS;
}
