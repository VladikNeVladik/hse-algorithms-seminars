#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>

#include <time.h>

//========================================//
// Макрос для удобной проверки контрактов //
//========================================//

// Для применения макроса VERIFY_CONTRACT требуется
// подключение библиотеки stdlib.h.
#define VERIFY_CONTRACT(contract, format, ...) \
    do {                                       \
        if (!(contract)) {                     \
            printf((format), ##__VA_ARGS__);   \
            exit(EXIT_FAILURE);                \
        }                                      \
    } while (0)

//============================//
// Алгоритмы поиска подстроки //
//============================//

//==================================================================================================
// Функция: naive_strstr
// Назначение: производит поиск подстроки в строке
//--------------------------------------------------------------------------------------------------
// Параметры:
// haystack (in) - исходная строка, в которой производится поиск.
// needle (in) - искомая строка.
//
// Возвращаемое значение:
// Указатель на фрагамент внутри строки haystack, равный строке needle, или NULL,
// если подстрока needle не содержится в строке haystack.
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Алгоритм не является оптимальным.
//==================================================================================================
const char* naive_strstr(const char* haystack, const char* needle)
{
    // Вычисляем размеры обеих строк:
    size_t haystack_len = strlen(haystack);
    size_t needle_len   = strlen(needle);

    if (needle_len > haystack_len)
    {
        // Поиск бОльшей подстроки в меньшей результата не даст
        return NULL;
    }

    // Полный перебор всех начальных позиций внутри строки haystack
    const char* cur = haystack;
    while (cur + needle_len <= haystack + haystack_len)
    {
        // Посимвольно проверяем факт совпадения фрагмента строки с искомой строкой
        bool match = true;
        for (size_t index = 0U; index < needle_len; ++index)
        {
            if (cur[index] != needle[index])
            {
                match = false;
                break;
            }
        }

        // Возвращаем первое совпадение
        if (match)
        {
            return cur;
        }

        // При несоответствии фрагмента переходим к следующему кандидату
        cur += 1;
    }

    // Искомая строка не содержится в исходной строке
    return NULL;
}

//=============================//
// Главная процедура программы //
//=============================//

// Имя файла, в котором производится поиск
const char* HAYSTACK_FILENAME = "res/book-war-and-peace.txt";

int main(void)
{
    //------------------------//
    // Чтение строки из файла //
    //------------------------//

    // Открываем файл
    FILE* haystack_file = fopen(HAYSTACK_FILENAME, "r");
    VERIFY_CONTRACT(
        haystack_file != NULL,
        "Unable to open file: %s\n", HAYSTACK_FILENAME
    );

    // Измеряем размер файла
    int ret = fseek(haystack_file, 0L, SEEK_END);
    VERIFY_CONTRACT(
        ret != -1,
        "Unable to find end of file: %s\n", HAYSTACK_FILENAME
    );

    long haystack_len = ftell(haystack_file);
    VERIFY_CONTRACT(
        haystack_len != -1,
        "Unable to measure size for file: %s\n", HAYSTACK_FILENAME
    );

    // Возвращаем каретку на исходную точку
    ret = fseek(haystack_file, 0L, SEEK_SET);
    VERIFY_CONTRACT(
        ret != -1,
        "Unable to rewind to start of file: %s\n", HAYSTACK_FILENAME
    );

    //--------------------------------//
    // Подготавливаем исходную строку //
    //--------------------------------//

    // Выделяем место для исходной строки
    // Добавляем +1 для символа конца строки
    char* haystack = (char*) malloc(haystack_len + 1);
    VERIFY_CONTRACT(
        haystack != NULL,
        "Unable to allocate %ld bytes of memory\n", haystack_len
    );

    // Считывеам строку из файла:
    size_t bytes_read = fread(haystack, 1U, haystack_len, haystack_file);
    VERIFY_CONTRACT(
        bytes_read == haystack_len,
        "Unable to read haystack from file (read %lu of %lu bytes)\n",
        bytes_read, haystack_len
    );

    // Выставляем символ конца строки
    haystack[haystack_len] = '\0';

    // Закрываем файл
    ret = fclose(haystack_file);
    VERIFY_CONTRACT(ret == 0,
        "Unable to close file \'%s\'\n",
        HAYSTACK_FILENAME);

    //---------------------------//
    // Подготовка искомой строки //
    //---------------------------//

    printf("Enter needle:\n");

    // Аллоцируем буфер для искомой строки
    // Добавляем +1 для символа конца строки
    const size_t MAX_NEEDLE_SIZE = 1000;
    char needle[MAX_NEEDLE_SIZE + 1];

    // Читаем строку из входного потока
    char* ndl = fgets(needle, MAX_NEEDLE_SIZE + 1, stdin);
    VERIFY_CONTRACT(
        ndl != NULL,
        "Unable to read needle string\n"
    );

    // Заменяем финальный символ переноса строки
    char* newline = strchr(needle, '\n');
    if (newline != NULL)
    {
        *newline = '\0';
    }

    //---------------------------------------------//
    // Ищем все вхождения подстроки в Войне и Мире //
    //---------------------------------------------//

    // Запускаем измерение времени
    clock_t ticks_start = clock();

    size_t occurences = 0U;
    const char* cur = haystack;
    while (true)
    {
        // Ищем следующее вхождение искомой строки
        cur = naive_strstr(cur, needle);
        if (cur == NULL)
        {
            break;
        }

        // Готовимся к поиску следующего вхождения
        cur += 1;
        occurences += 1U;
    }

    // Завершаем измерение времени
    clock_t ticks_end = clock();

    double ticks_delta = ticks_end - ticks_start;
    double seconds = ticks_delta / CLOCKS_PER_SEC;

    printf("Number of occurences: %lu\n", occurences);
    printf("Time: %10.6lfs\n", seconds);

    return EXIT_SUCCESS;
}
