// Copyright 2024 Vladislav Aleinik
#include <string.h>

// Осторожно: API не из стандартной библиотеки (API POSIX)
#include <arpa/inet.h>

#include "database.h"
#include "utils.h"

// Имя файла с базой данных
const char* DB_FILENAME = "res/database.db";

// Магические числа
// Используются для идентификации формата базы даных
const uint32_t MAGIC0 = 0xDEADBEEF;
const uint32_t MAGIC1 = 0xB01DFACE;

int main(void)
{
    // Открываем файл на чтение в бинарном виде
    FILE* file = fopen(DB_FILENAME, "rb+");
    verify_contract(file != NULL, "Unable to open file \'%s\'\n", DB_FILENAME);

    // Измеряем размер файла
    int ret = fseek(file, 0L, SEEK_END);
    verify_contract(
        ret != -1,
        "Unable to find end of file: %s\n", DB_FILENAME
    );

    long file_size = ftell(file);
    verify_contract(
        file_size != -1,
        "Unable to measure size for file: %s\n", DB_FILENAME
    );

    // Возвращаем каретку на исходную точку
    ret = fseek(file, 0L, SEEK_SET);
    verify_contract(
        ret != -1,
        "Unable to rewind to start of file: %s\n", DB_FILENAME
    );

    // Считываем и проверяем магические числа
    uint32_t magics[2];

    size_t retf = fread(magics, sizeof(MAGIC0), 2, file);
    verify_contract(retf == 2, "Unable to read magic number from file \'%s\'\n", DB_FILENAME);
    verify_contract(
        ntohl(magics[0]) == MAGIC0 && ntohl(magics[1]) == MAGIC1,
        "Invalid magic numbers\n");

    // База данных для хранения ассоциативного массива
    struct Database db;

    // Вычисляем количество элементов в базе данных
    db.num_entries = (file_size - sizeof(MAGIC0) - sizeof(MAGIC1)) / sizeof(Entry_t);

    // Выделяем массив для хранения элементов базы данных
    db.entries = (Entry_t*) calloc(db.num_entries, sizeof(Entry_t));
    verify_contract(db.entries != NULL, "Unable to allocate memory\n");

    // Считываем элементы базы данных
    retf = fread(db.entries, sizeof(Entry_t), db.num_entries, file);
    verify_contract(retf == db.num_entries,
        "Unable to read magic number from file \'%s\'\n", DB_FILENAME);

    // Закрываем файл
    ret = fclose(file);
    verify_contract(ret != EOF,
        "Detected error on file close operation \'%s\'\n", DB_FILENAME);

    // Выводим считанное содержимое БД
    for (size_t db_i = 0; db_i < db.num_entries; ++db_i)
    {
        // Записываем IP-адрес
        uint32_t ip = ntohl(db.entries[db_i].key);
        uint8_t byte3 = (ip >> 24U) & 0xFFU;
        uint8_t byte2 = (ip >> 16U) & 0xFFU;
        uint8_t byte1 = (ip >>  8U) & 0xFFU;
        uint8_t byte0 = (ip >>  0U) & 0xFFU;

        // Считываем обладателя IP-адреса из базы данных
        char hostname[VALUE_SIZE + 1];

        strncpy(hostname, db.entries[db_i].value, VALUE_SIZE);
        hostname[VALUE_SIZE] = '\0';

        // Выводим на экран содержимое строки БД
        printf("%-10s %hhu.%hhu.%hhu.%hhu\n", hostname, byte3, byte2, byte1, byte0);
    }

    // Освобожаем память под базу данных
    free(db.entries);

    return EXIT_SUCCESS;
}
