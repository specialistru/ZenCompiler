// Compiler/src/core/config.c
// Модуль управления конфигурацией компилятора ABAP.
// Отвечает за чтение параметров из конфигурационного файла,
// хранение настроек и предоставление доступа к ним другим модулям.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

// Максимальные размеры для строк конфигурации
#define MAX_LINE_LENGTH 256
#define MAX_PATH_LENGTH 256

// Структура хранения конфигурации
typedef struct {
    char output_dir[MAX_PATH_LENGTH];
    int optimization_level;
    int debug_enabled;
    int verbose;
} CompilerConfig;

static CompilerConfig g_config;

// Инициализация конфигурации значениями по умолчанию
void config_init_defaults() {
    strncpy(g_config.output_dir, "./build", MAX_PATH_LENGTH);
    g_config.optimization_level = 1;  // 0 - без оптимизации, 1 - базовая, 2 - агрессивная
    g_config.debug_enabled = 0;       // Отладочная информация отключена
    g_config.verbose = 0;             // Подробный вывод отключен
}

// Загрузка конфигурации из файла
// Формат файла конфигурации: ключ=значение, комментарии начинаются с #
// Пример:
// output_dir=./out
// optimization_level=2
// debug_enabled=1
// verbose=1
int config_load_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Не удалось открыть файл конфигурации: %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Игнорируем комментарии и пустые строки
        if (line[0] == '#' || line[0] == '\n') continue;

        // Убираем символ новой строки в конце
        line[strcspn(line, "\r\n")] = 0;

        // Разделяем строку на ключ и значение
        char *eq = strchr(line, '=');
        if (!eq) continue; // Некорректная строка, пропускаем

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;

        if (strcmp(key, "output_dir") == 0) {
            strncpy(g_config.output_dir, value, MAX_PATH_LENGTH - 1);
            g_config.output_dir[MAX_PATH_LENGTH - 1] = '\0';
        } else if (strcmp(key, "optimization_level") == 0) {
            g_config.optimization_level = atoi(value);
            if (g_config.optimization_level < 0) g_config.optimization_level = 0;
            if (g_config.optimization_level > 2) g_config.optimization_level = 2;
        } else if (strcmp(key, "debug_enabled") == 0) {
            g_config.debug_enabled = atoi(value) ? 1 : 0;
        } else if (strcmp(key, "verbose") == 0) {
            g_config.verbose = atoi(value) ? 1 : 0;
        }
        // Можно добавить дополнительные параметры при необходимости
    }

    fclose(file);
    return 1;
}

// Получить каталог вывода
const char *config_get_output_dir() {
    return g_config.output_dir;
}

// Получить уровень оптимизации
int config_get_optimization_level() {
    return g_config.optimization_level;
}

// Включен ли режим отладки
int config_is_debug_enabled() {
    return g_config.debug_enabled;
}

// Включен ли подробный вывод
int config_is_verbose() {
    return g_config.verbose;
}

// Установка verbose программно
void config_set_verbose(int verbose) {
    g_config.verbose = verbose ? 1 : 0;
}

