#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

// Тип для описания типа данных
typedef struct TypeInfo {
    char *name;              // Имя типа (например, INT, STRING)
    struct TypeInfo *base;   // Для пользовательских типов — базовый тип
    // Дополнительные атрибуты типа
} TypeInfo;

// Структура таблицы символов для области видимости
typedef struct SymbolTable {
    char **names;           // Имена символов
    TypeInfo **types;       // Типы символов
    int count;              // Количество символов
    struct SymbolTable *parent; // Родительская область видимости
} SymbolTable;

// Инициализация глобальной таблицы символов
void semantic_init();

// Запуск семантического анализа AST
int semantic_check(ASTNode *root);

// Добавление символа в текущую область видимости
int symbol_table_add(SymbolTable *table, const char *name, TypeInfo *type);

// Поиск символа в области видимости и родительских областях
TypeInfo *symbol_table_lookup(SymbolTable *table, const char *name);

// Очистка и освобождение таблиц символов
void semantic_cleanup();

#endif // SEMANTIC_H
