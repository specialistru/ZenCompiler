// Compiler/src/core/cli.c
// Модуль обработки командной строки для ABAP компилятора.
// Отвечает за парсинг аргументов, вывод справки и запуск компиляции.

// Включаем стандартные библиотеки
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Подключаем заголовочные файлы компилятора (условно)
#include "cli.h"
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "ir.h"
#include "codegen.h"

// Максимальная длина имени файла
#define MAX_FILENAME_LEN 256

// Структура параметров запуска
typedef struct {
    char input_file[MAX_FILENAME_LEN];
    char output_file[MAX_FILENAME_LEN];
    int show_help;
    int verbose;
} CLIOptions;

// Функция вывода справки
static void print_help() {
    printf("ABAP Compiler v0.1\n");
    printf("Использование:\n");
    printf("  abapc [опции] <input_file>\n\n");
    printf("Опции:\n");
    printf("  -h, --help       Показать эту справку\n");
    printf("  -o <file>        Указать имя выходного файла (байткод или исполняемый файл)\n");
    printf("  -v, --verbose    Подробный вывод информации\n");
}

// Функция парсинга аргументов командной строки
static int parse_arguments(int argc, char **argv, CLIOptions *opts) {
    if (argc < 2) {
        fprintf(stderr, "Ошибка: не указан входной файл\n");
        return 0;
    }

    opts->input_file[0] = '\0';
    opts->output_file[0] = '\0';
    opts->show_help = 0;
    opts->verbose = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            opts->show_help = 1;
            return 1;  // Дальше не нужно
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            opts->verbose = 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                strncpy(opts->output_file, argv[i + 1], MAX_FILENAME_LEN - 1);
                opts->output_file[MAX_FILENAME_LEN - 1] = '\0';
                i++;  // пропускаем имя файла
            } else {
                fprintf(stderr, "Ошибка: после -o должен следовать файл\n");
                return 0;
            }
        } else {
            // Если input_file еще не задан, считаем этот аргумент входным файлом
            if (opts->input_file[0] == '\0') {
                strncpy(opts->input_file, argv[i], MAX_FILENAME_LEN - 1);
                opts->input_file[MAX_FILENAME_LEN - 1] = '\0';
            } else {
                fprintf(stderr, "Ошибка: неизвестный параметр или повтор входного файла: %s\n", argv[i]);
                return 0;
            }
        }
    }

    if (opts->input_file[0] == '\0' && !opts->show_help) {
        fprintf(stderr, "Ошибка: не указан входной файл\n");
        return 0;
    }

    return 1;
}

// Основная функция CLI — запускает весь процесс компиляции
int cli_run(int argc, char **argv) {
    CLIOptions opts;

    if (!parse_arguments(argc, argv, &opts)) {
        print_help();
        return EXIT_FAILURE;
    }

    if (opts.show_help) {
        print_help();
        return EXIT_SUCCESS;
    }

    if (opts.verbose) {
        printf("Входной файл: %s\n", opts.input_file);
        if (opts.output_file[0] != '\0') {
            printf("Выходной файл: %s\n", opts.output_file);
        } else {
            printf("Выходной файл не задан, будет использован файл по умолчанию.\n");
        }
    }

    // Открываем входной файл
    FILE *input = fopen(opts.input_file, "r");
    if (!input) {
        fprintf(stderr, "Ошибка: не удалось открыть входной файл: %s\n", opts.input_file);
        return EXIT_FAILURE;
    }

    // Лексический анализ
    if (opts.verbose) printf("Запуск лексического анализа...\n");
    Lexer *lexer = lexer_create(input);
    if (!lexer) {
        fprintf(stderr, "Ошибка: не удалось инициализировать лексер\n");
        fclose(input);
        return EXIT_FAILURE;
    }

    // Синтаксический анализ
    if (opts.verbose) printf("Запуск синтаксического анализа...\n");
    Parser *parser = parser_create(lexer);
    if (!parser) {
        fprintf(stderr, "Ошибка: не удалось инициализировать парсер\n");
        lexer_destroy(lexer);
        fclose(input);
        return EXIT_FAILURE;
    }

    ASTNode *ast = parser_parse(parser);
    parser_destroy(parser);
    lexer_destroy(lexer);
    fclose(input);

    if (!ast) {
        fprintf(stderr, "Ошибка: синтаксический анализ завершился с ошибками\n");
        return EXIT_FAILURE;
    }

    if (opts.verbose) printf("Запуск семантического анализа...\n");
    if (!semantic_check(ast)) {
        fprintf(stderr, "Ошибка: семантический анализ завершился с ошибками\n");
        ast_destroy(ast);
        return EXIT_FAILURE;
    }

    if (opts.verbose) printf("Генерация промежуточного представления...\n");
    IR *ir = ir_generate(ast);
    ast_destroy(ast);

    if (!ir) {
        fprintf(stderr, "Ошибка: генерация IR не удалась\n");
        return EXIT_FAILURE;
    }

    if (opts.verbose) printf("Генерация кода...\n");
    int codegen_result = codegen_generate(ir, opts.output_file[0] ? opts.output_file : NULL);
    ir_destroy(ir);

    if (codegen_result != 0) {
        fprintf(stderr, "Ошибка: генерация кода завершилась с ошибками\n");
        return EXIT_FAILURE;
    }

    if (opts.verbose) printf("Компиляция завершена успешно.\n");
    return EXIT_SUCCESS;
}
