// Copyright 2024 Vladislav Aleinik
#ifndef HEADER_GUARD_UTILS_H_INCLUDED
#define HEADER_GUARD_UTILS_H_INCLUDED

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>

void verify_contract(bool condition, const char* format, ...);

// Тип данных, задающий код возврата
enum class RetCode : uint8_t
{
    // Операция выполнена без ошибок
    RET_OK   = 0,
    // Операция не выполнена, т.к. для её выполнения не хватает памяти
    RET_NOMEM = 1,
    // Операция не выполнена, т.к. аргументы операции некорректны
    RET_INVAL = 2,
    // Операция не выполнена, т.к. операция с файлом вернула ошибку
    RET_FILEIO = 3,
    // Операция не выполнена из-за несоответствующей конфигурации системы.
    RET_SYSTEM_CONFIG_ERROR = 4
};

const char* retcode_str(RetCode code);

#endif // HEADER_GUARD_UTILS_H_INCLUDED
