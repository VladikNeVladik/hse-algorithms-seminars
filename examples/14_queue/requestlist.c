// Copyright 2024 Vladislav Aleinik
#include <stdint.h>
#include <time.h>
#include <unistd.h>

//=============//
// Комментарий //
//=============//

// Оставь надежду всяк сюда входящий!
//
// Код этого примера использования структуры данных сложен для понимания и не соответсвует программе курса.
// Рекомендуется ознакомиться со слайдами соответствующей презентации, или с самой структурой данной (см. queue.h).

//=================================================//
// Модель системы с несколькими очередями запросов //
//=================================================//

// Тип, представляющий категорию запроса
typedef uint8_t RequestCategory;

// Тип отложенного запроса
typedef struct {
    // Время поступления запроса в систему
    struct timespec arrival_time;

    // Время выполнения запроса
    struct timespec time_capacity;

    // Категория запросов, к которой относится данный запрос
    RequestCategory category;
} Request;

// Подключение структуры данных "Очередь Запросов"
typedef Request Data_t;

#include "queue.h"

typedef Queue RequestQueue;

// Тип, представляющий категорию запросов
typedef struct {
    // Гарантия производительности обработки запросов (в запросах в секунду)
    uint32_t requests_per_sec;

    // Время начала обработки последнего запроса
    struct timespec last_request;

    // Очередь запросов для данной категории
    RequestQueue requests;
} RequestCategoryHandler;

//========================================================//
// Модель работы системы с несколькими очередями запросов //
//========================================================//

// Количество наносекунд в одной миллисекунде
#define ONE_MS 1000000U

// Количество категорий запросов
#define NUM_CATEGORIES 3U

// Формат сообщения об обработке запроса
typedef struct
{
    // Запрос, пуступивший в обработку
    Request req;

    // Время начала и конца обработки запроса
    struct timespec service_start_time;
    struct timespec service_end_time;
} LogEntry;

// Количество запросов в системе
#define NUM_REQUESTS 100U

struct timespec current_time(void)
{
    struct timespec time;

    // Считываем текущее время (учитываем только время выполнения данного процесса).
    int ret = clock_gettime(CLOCK_REALTIME, &time);
    verify_contract(ret == 0, "Unable to get time with clock_gettime\n");

    return time;
}

// Модель процедуры обработки запросов
void handle_request(const Request* request, LogEntry* log_entry)
{
    // Записываем время передачи запроса в исполнение
    log_entry->service_start_time = current_time();

    // Сохраняем информацию о запросе
    log_entry->req = *request;

    // Производим ожидание, пропорциональное времени выполнения запросов
    int ret = nanosleep(&request->time_capacity, NULL);
    verify_contract(ret == 0, "Unable to sleep\n");

    // Записываем время окончания обработки запроса
    log_entry->service_end_time = current_time();
}

int main(void)
{
    // Получаем время начала бенчмарка
    struct timespec start_time = current_time();

    // Очередь запросов для добавления в систему.
    RequestQueue arrival_queue;
    RetCode ret = queue_alloc(&arrival_queue);
    verify_contract(ret == RET_OK, "Unable to create arrival queue!\n");

    // Время выдачи следующего запроса
    struct timespec next_arrival_time = start_time;

    struct timespec time_capacity = {
        .tv_sec  = 0U,
        .tv_nsec = 100U * ONE_MS,
    };

    // Генерируем запросы трёх категорий
    for (size_t req_i = 0U; req_i < NUM_REQUESTS; ++req_i)
    {
        // Обновляем время запроса каждые 50 мс
        next_arrival_time.tv_nsec += 50U * ONE_MS;

        if (next_arrival_time.tv_nsec >= 1000U * ONE_MS)
        {
            next_arrival_time.tv_nsec -= 1000U * ONE_MS;
            next_arrival_time.tv_sec  += 1U;
        }

        // Запрос для добавления в очередь запросов для обработки
        Request req = {
            .arrival_time  = next_arrival_time,
            .time_capacity = time_capacity,
            .category      = req_i % 3U,
        };

        // Добавляем новый запрос в очередь запросов
        ret = queue_add_tail(&arrival_queue, &req);
        verify_contract(ret == RET_OK, "Unable to add request#%zu to arrival queue\n", req_i);
    }

    // Создаём обработчики запросов для каждой категории запросов
    RequestCategoryHandler request_handlers[NUM_CATEGORIES];
    for (RequestCategory cat_i = 0U; cat_i < NUM_CATEGORIES; ++cat_i)
    {
        // Инициализируем очередь запросов выбранной категории
        ret = queue_alloc(&request_handlers[cat_i].requests);
        verify_contract(ret == RET_OK, "Unable to initialize category queue\n");

        // Задаём требуемые гарантии обработки запросов
        request_handlers[cat_i].requests_per_sec = 1U + cat_i;
    }

    // Флаг пустоты всех очередей обработки запросов
    bool service_queues_empty = false;

    // Массив сообщений об обработке запросов
    LogEntry log_entries[NUM_REQUESTS];
    // Индекс текущего сообщений в массиве
    size_t log_entry_i = 0U;

    // Основной цикл обработки запросов
    while (!queue_empty(&arrival_queue) || !service_queues_empty)
    {
        // Флаг необходимости добавления запросов в систему.
        bool new_arrived_requests = true;

        // Добавляем все поступившие запросы в очереди обработки
        while (new_arrived_requests && !queue_empty(&arrival_queue))
        { // Очередь на добавление запросов в систему была не пуста
            // Указатель на следующий запрос на добавление в систему
            Request* next_request;

            // Рассматриваем следующий запрос на возможность добавления в систему
            ret = queue_peek(&arrival_queue, &next_request);
            verify_contract(ret == RET_OK, "Unable to read queue head element\n");

            // Текущее время
            struct timespec now = current_time();

            // Сравниваем время выдачи запроса с текущим временем
            if (now.tv_sec  > next_request->arrival_time.tv_sec ||
               (now.tv_sec == next_request->arrival_time.tv_sec && now.tv_nsec >= next_request->arrival_time.tv_nsec))
            {   // Время добавления запроса в систему прошло
                // Новый запрос для добавления в одну из очередей планировщика
                Request new_request;

                // Извлекаем запрос из очереди
                ret = queue_remove_head(&arrival_queue, &new_request);
                verify_contract(ret == RET_OK, "Unable to remove head arrival queue\n");

                // Обновляем время поступления запроса в систему на текущее время
                new_request.arrival_time = now;

                // Категория текущего запроса
                RequestCategory category = new_request.category;

                // Определяем очередь для категории запроса
                RequestQueue* request_queue = &request_handlers[category].requests;

                // Добавляем запрос в хвост очереди данной категории
                ret = queue_add_tail(request_queue, &new_request);
                verify_contract(ret == RET_OK, "Unable to add request to category#%u queue\n", category);

                service_queues_empty = false;
            }
            else
            {   // Время на добавление запроса в систему ещё не наступило
                new_arrived_requests = false;
            }
        }

        // Обработка запроса согласно стратегии планирования запросов
        // Стратегия планирования запросов:
        // 1. Будем выбирать категорию запросов только из тех категорий, для которых очередь запросов непуста.
        // 2. Будем выбирать категорию запросов, для которой отношение
        //    (время с последнего обработанного запроса)/(ожидаемое кол-во запросов в секунду) будет максимально.

        // Текущее время
        struct timespec now = current_time();

        // Категория запроса, оптимальная для обработки в данный момент
        RequestCategory optimal_cat_i = NUM_CATEGORIES;

        // Степень оптимальности текущей категории запроса
        double best_optimality = 0.0;

        for (RequestCategory cat_i = 0U; cat_i < NUM_CATEGORIES; ++cat_i)
        {
            // Текущая категория запросов
            RequestCategoryHandler* cat = &request_handlers[cat_i];

            if (queue_empty(&cat->requests))
            {
                // Очередь запросов текущей категории пуста
                continue;
            }

            // Время последнего обработанного запроса
            struct timespec last = cat->last_request;

            // Время в наносекундах с последнего обработанного запроса данной категории
            uint64_t time_diff_ns = (now.tv_sec - last.tv_sec) * 1000U * ONE_MS + now.tv_nsec - last.tv_nsec;

            // Коэффициент оптимальности текущей категории
            double optimalilty = time_diff_ns / cat->requests_per_sec;

            // Проверяем текущую категорию на оптимальность
            if (optimalilty > best_optimality)
            {
                optimal_cat_i = cat_i;
            }
        }

        // Проверяем наличие хоть одной категории запросов с запросами в очереди на обработку
        if (optimal_cat_i == NUM_CATEGORIES)
        {   // Все очереди запросов на обработку пусты

            // Устанавливаем флаг пустоты всех очередей обработки запросов
            service_queues_empty = true;
        }
        else
        {   // Хотя бы один запрос на обработку имеется в очередях
            // Запрос направленный на выполнение
            Request to_process;

            // Считываем запрос из очереди
            ret = queue_remove_head(&request_handlers[optimal_cat_i].requests, &to_process);
            verify_contract(ret == RET_OK, "Unable to remove request from service queue#%u\n", optimal_cat_i);

            // Выполняем обработку запроса
            handle_request(&to_process, &log_entries[log_entry_i]);

            // Обновляем время последней обработки запроса данной категории
            request_handlers[optimal_cat_i].last_request = log_entries[log_entry_i].service_end_time;

            // Переходим к записи следующего сообщения об обработке запросов
            log_entry_i++;
        }
    }

    // Производим вывод информации о всех обработанных запросах
    for (RequestCategory cat_i = 0U; cat_i < NUM_CATEGORIES; ++cat_i)
    {
        printf("Category#%u arrival time:\n", cat_i);

        // Выводим первую точку графика
        printf("  0.000000, 0\n");

        size_t req_i = 1U;
        for (size_t log_entry_i = 0U; log_entry_i < NUM_REQUESTS; ++log_entry_i)
        {
            // Текущая запись о статусе обработке запроса
            LogEntry entry = log_entries[log_entry_i];

            if (entry.req.category != cat_i)
            {
                // Обработка запросов производится по категориям
                continue;
            }

            // Сдвигаем ось абсцисс в 0
            entry.req.arrival_time.tv_sec -= start_time.tv_sec;

            // Выводим информацию об обработке запроса
            printf("%3lu.%06lu, %lu\n",
                entry.req.arrival_time.tv_sec, entry.req.arrival_time.tv_nsec / 1000U,
                req_i);

            // Обновляем номер запроса заданной категории
            req_i++;
        }

        printf("Category#%u service end time:\n", cat_i);

        // Выводим первую точку графика
        printf("  0.000000, 0\n");

        req_i = 1U;
        for (size_t log_entry_i = 0U; log_entry_i < NUM_REQUESTS; ++log_entry_i)
        {
            // Текущая запись о статусе обработке запроса
            LogEntry entry = log_entries[log_entry_i];

            if (entry.req.category != cat_i)
            {
                // Обработка запросов производится по категориям
                continue;
            }

            // Сдвигаем ось абсцисс в 0
            entry.service_end_time.tv_sec -= start_time.tv_sec;

            // Выводим информацию об обработке запроса
            printf("%3lu.%06lu, %lu\n",
                entry.service_end_time.tv_sec, entry.service_end_time.tv_nsec / 1000U,
                req_i);

            // Обновляем номер запроса заданной категории
            req_i++;
        }
    }

    // Освобождаем очередь запросов для добавления в систему.
    queue_free(&arrival_queue);

    // Освобождаем очереди обработки запросов
    for (RequestCategory cat_i = 0U; cat_i < NUM_CATEGORIES; ++cat_i)
    {
        queue_free(&request_handlers[cat_i].requests);
    }
}
