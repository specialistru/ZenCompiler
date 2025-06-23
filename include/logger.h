#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

// Уровни логирования
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

// Инициализация логгера (например, открытие файла логов)
int logger_init(const char *log_file_path);

// Завершение работы логгера (закрытие файла)
void logger_cleanup();

// Логирование сообщения с указанным уровнем
void logger_log(LogLevel level, const char *format, ...);

#endif // LOGGER_H
