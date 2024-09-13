#include <stdio.h>
#include <math.h>

//==================//
// Home assignments //
//==================//

#define NUM_TASKS 7U

// NOTE: each hwi[j] must be in interval [0; 1.0].
//                     |             Task №№              |
// Homework #1:        | 01 | 02 | 03 | 04 | 05 | 06 | 07 |
float hw1[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// Homework #2:        |    |    |    |    |    |    |    |
float hw2[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// Homework #3:        |    |    |    |    |    |    |    |
float hw3[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// Homework #4:        |    |    |    |    |    |    |    |
float hw4[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// Homework #5:        |    |    |    |    |    |    |    |
float hw5[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
// Homework #6:        |    |    |    |    |    |    |    |
float hw6[NUM_TASKS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

// Penalties for plagiarism:
// NOTE: each hwi[j] must be no more than
//       the sum of the corresponding homework.
float penalties[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

//==================//
// Test assignments //
//==================//

// NOTE: "papertest" stands for "Самостоятельная работа".
// NOTE: "test" stands for "Контрольная работа".
// NOTE: papertesti and testi must be in interval [0; 1.0].
float papertest1 = 1.0, papertest2 = 1.0;
float test1 = 1.0, test2 = 1.0, test3 = 1.0;

//======//
// Exam //
//======//

// NOTE: exam must be in interval [0; 1.0].
float exam = 1.0;

//=============================//
// Semester result calculation //
//=============================//

int main(void)
{
    // Compute total for home assignments:
    float hw1_sum = 0.0, hw2_sum = 0.0, hw3_sum = 0.0;
    float hw4_sum = 0.0, hw5_sum = 0.0, hw6_sum = 0.0;

    // Compute number of tasks done in each homework:
    int hw1_num_tasks = 0, hw2_num_tasks = 0, hw3_num_tasks = 0;
    int hw4_num_tasks = 0, hw5_num_tasks = 0, hw6_num_tasks = 0;
    for (int i = 0; i < NUM_TASKS; ++i)
    {
        // Determine whether task #i is done in every homework:
        hw1_num_tasks += (hw1[i] > 0.01);
        hw2_num_tasks += (hw2[i] > 0.01);
        hw3_num_tasks += (hw3[i] > 0.01);
        hw4_num_tasks += (hw4[i] > 0.01);
        hw5_num_tasks += (hw5[i] > 0.01);
        hw6_num_tasks += (hw6[i] > 0.01);

        // Compute the sum for each HW:
        hw1_sum += hw1[i];
        hw2_sum += hw2[i];
        hw3_sum += hw3[i];
        hw4_sum += hw4[i];
        hw5_sum += hw5[i];
        hw6_sum += hw6[i];
    }

    hw1_sum = hw1_sum / NUM_TASKS - penalties[0];
    hw2_sum = hw2_sum / NUM_TASKS - penalties[1];
    hw3_sum = hw3_sum / NUM_TASKS - penalties[2];
    hw4_sum = hw4_sum / NUM_TASKS - penalties[3];
    hw5_sum = hw5_sum / NUM_TASKS - penalties[4];
    hw6_sum = hw6_sum / NUM_TASKS - penalties[5];

    // Account for the fact that all HW total is zero if
    // there is at least one HW with 2 or less tasks:
    float hw_total = 0.0;
    if (hw1_num_tasks >= 3 && hw2_num_tasks >= 3 && hw3_num_tasks >= 3 &&
        hw4_num_tasks >= 3 && hw5_num_tasks >= 3 && hw6_num_tasks >= 3)
    {
        hw_total = 0.05 * (hw1_sum + hw2_sum + hw3_sum +
                           hw4_sum + hw5_sum + hw6_sum);
    }

    // Compute total for test assignments:
    float test_total = 0.01 * (papertest1 + papertest2)
                     + 0.09 * (test1 + test2 + test3);

    // Compute semester total:
    float exam_total = 0.40 * exam;

    float sem_total = 0.0;
    // NOTE: use "<" instead of "<=" for floating point comparison.
    if (exam < 0.3)
    {
        sem_total = exam_total;
    }
    else
    {
        sem_total = hw_total + test_total + exam_total;
    }

    float result = roundf(10.0 * sem_total);

    printf("Your grade is: %.02f\n", result);

    return 0;
}
