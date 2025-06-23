Следующий файл из директории `include/` — `ast.h`

---

### Назначение `ast.h`:

Определяет структуру узла абстрактного синтаксического дерева (AST) и связанные константы и функции для создания, управления и освобождения AST. AST используется для представления синтаксической структуры ABAP-кода после парсинга.

---

## Пример реализации `ast.h`

```c
#ifndef AST_H
#define AST_H

#include <stdlib.h>

// Типы узлов AST
typedef enum {
    AST_UNKNOWN,
    AST_PROGRAM,
    AST_STATEMENT,
    AST_EXPRESSION,
    AST_IDENTIFIER,
    AST_LITERAL,
    AST_OPERATOR,
    AST_AUTHORITY_CHECK,    // Узел AUTHORITY-CHECK
    AST_AUTH_CHECK_PARAM,   // Параметр AUTHORITY-CHECK
    // Другие типы узлов по мере необходимости
} ASTNodeType;

// Структура узла AST
typedef struct ASTNode {
    ASTNodeType type;           // Тип узла
    char *string_value;         // Строковое значение (например, имя идентификатора или литерала)
    struct ASTNode **children;  // Массив дочерних узлов
    int child_count;            // Количество дочерних узлов
} ASTNode;

// Создание нового узла AST заданного типа
ASTNode *ast_node_create(ASTNodeType type);

// Освобождение памяти узла AST и всех его потомков
void ast_node_free(ASTNode *node);

#endif // AST_H
```

---

Если хочешь — могу помочь с реализацией `ast.c` или следующими файлами.
