#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

//===================================//
// Макрос для удобной проверки ввода //
//===================================//

#define VERIFY_CONTRACT(contract, format, ...) \
    do {                                       \
        if (!(contract)) {                     \
            printf((format), ##__VA_ARGS__);   \
            exit(EXIT_FAILURE);                \
        }                                      \
    } while (0)

//===========================//
// Функция "проверки" пароля //
//===========================//

const bool VERIFY_PASSWORD = true;
const char* PASSWORD = "Now you see me!";

//==================================================================================================
// Функция: password_is_ok
// Назначение: проверяет пароль на правильность
//--------------------------------------------------------------------------------------------------
// Параметры:
// str (in) - строка с паролем
//
// Возвращаемое значение:
// true - пароль правильный
// false - пароль непраивльный
//
// Используемые внешние переменные:
// отсутствуют
//
// Примечания:
// - Легко заметить, что данная функция легко "взламывается" при прочтении бинарного файла.
// - Также возможно извлечение информации о длине пароля при замера времени выполнения функции.
//==================================================================================================
bool password_is_ok(const char* check)
{
    // Последовательно сравниваем символы обеих строк
    for (const char* pswd = PASSWORD; *pswd != '\0' && *check != '\0'; pswd++, check++)
    {
        if (*pswd != *check)
        {
            // При несовпадении любого символа строки не равны
            return false;
        }
    }

    // В противном случае строки равны
    return true;
}

//=========================================================//
// Опасная программа, некорректно производящая ввод строки //
//=========================================================//

void grant_access(void)
{
    printf("Password is OK\n");
}

void prohibit_access(void)
{
    printf("Password is WRONG\n");
}

#define MAX_ARRAY_SIZE 32U

int main(void)
{
    // Условие необходимости проверки пароля
    bool check_password = VERIFY_PASSWORD;
    char input_password[MAX_ARRAY_SIZE];

    // Считываем пароль от пользователя
    int ret = scanf("%s", input_password);
    VERIFY_CONTRACT(ret == 1, "ERROR: unable to input string\n");

    // Проверяем пароль и выполняем соответсвующее действие
    if (!check_password || password_is_ok(input_password))
    {
        grant_access();
    }
    else
    {
        prohibit_access();
    }

    return EXIT_SUCCESS;
}
