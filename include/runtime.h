#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdint.h>

// Тип для внутренней таблицы ABAP
typedef struct {
    void *rows;       // Указатель на строки таблицы
    int row_count;    // Количество строк
    int row_capacity; // Вместимость таблицы
    // Дополнительные метаданные
} InternalTable;

// Инициализация среды выполнения
void runtime_init();

// Завершение работы среды выполнения
void runtime_cleanup();

// Работа с внутренними таблицами
InternalTable* runtime_create_internal_table();
void runtime_append_row(InternalTable *table, void *row);
void runtime_delete_row(InternalTable *table, int index);

// Ошибки и исключения
void runtime_raise_error(const char *message);
void runtime_handle_exception();

// Поддержка параллелизма
void runtime_lock();
void runtime_unlock();

#endif // RUNTIME_H
