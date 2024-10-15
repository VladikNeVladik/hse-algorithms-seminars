#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

//========================================//
// Макрос для удобной проверки контрактов //
//========================================//

#define VERIFY_CONTRACT(contract, format, ...) \
    do {                                       \
        if (!(contract)) {                     \
            printf((format), ##__VA_ARGS__);   \
            exit(EXIT_FAILURE);                \
        }                                      \
    } while (0)

//=========================================================//
// Компаратор для лексикографического сравнения двух строк //
//=========================================================//

//==================================================================================================
// Функция: comp_lexicographic
// Назначение: функция лексикографического сравнения двух строк по указателям
//--------------------------------------------------------------------------------------------------
// Параметры:
// p1 (in) - указатель на первую строку
// p2 (in) - указатель на вторую строку
//
// Возвращаемое значение:
// число > 0 - если строка p1 идёт лексикграфически после p2
// 0 - если строки совпадают
// число < 0 - если строка p1 идёт лексикграфически до p2
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// отсутствуют
//==================================================================================================

int comp_lexicographic(const void* p1, const void* p2)
{
    const char* str1 = *(const char**)p1;
    const char* str2 = *(const char**)p2;

    return strcmp(str1, str2);
}

//=============================//
// Line sorting implementation //
//=============================//

#define MAX_LINES 32
#define MAX_LINE_SIZE 64
#define BUFFER_SIZE (MAX_LINES * (MAX_LINE_SIZE + 1))

int main(void)
{
    // Буфер для хранения всех введённых строк
    char buffer[BUFFER_SIZE];

    // Массив для хранения указателей на строки
    char* lines[MAX_LINES];

    // Индекс первой свободной ячейки буфера
    size_t buffer_i = 0U;

    // Количество введённых строк
    size_t num_lines = 0U;

    // Считываем
    while (true)
    {
        // Проверяем количество строк во вводе
        VERIFY_CONTRACT(num_lines < MAX_LINES,
            "Error: there are too many input lines\n");

        // Считываем следующую строку с консоли
        char* str = fgets(&buffer[buffer_i], BUFFER_SIZE - buffer_i, stdin);
        if (str == NULL)
        {
            VERIFY_CONTRACT(feof(stdin), "Error: unable to read line #%zu\n", num_lines);

            // При обнаружении конца ввода данных заканчиваем считывание
            break;
        }

        // Вычисляем размер строки
        size_t len = strlen(str);

        // Игнорируем пустую строку
        if (len == 0)
        {
            continue;
        }

        // Проверяем наличие символа переноса строки
        VERIFY_CONTRACT(str[len - 1] == '\n',
            "Error: input line is too long\n");

        // Игнорируем строку, состоящую только из символа переноса строки
        if (len == 1)
        {
            continue;
        }

        // Сохраняем адрес на начало текущей строки
        lines[num_lines] = str;

        // Избавляемся от символа переноса строки
        lines[num_lines][len - 1] = '\0';

        // Сдвигаем индекс первой свободной ячейки буфера
        buffer_i += len + 1;

        // Увеличиваем количество сохранённых строк
        ++num_lines;
    }

    //  Выполняем сортировку с заданным компаратором
    qsort(lines, num_lines, sizeof(*lines), &comp_lexicographic);

    // Выводим символы в отсортированном порядке
    for (size_t line_i = 0U; line_i < num_lines; ++line_i)
    {
        printf("%s\n", lines[line_i]);
    }

    return EXIT_SUCCESS;
}
