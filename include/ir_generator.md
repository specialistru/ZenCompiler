Конечно! Вот пример полного и подробного заголовочного файла `ir_generator.h` для модуля `ir_generator.c`, который отвечает за генерацию промежуточного представления (IR). Включены описания структур, функций и комментарии для понимания и дальнейшей поддержки:

```c
#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "ir.h"        // Определения структур IR и типов
#include "parser.h"    // AST и типы парсера
#include <stdbool.h>

/**
 * @file ir_generator.h
 * @brief Интерфейс генератора промежуточного представления (IR) из AST.
 * 
 * Этот модуль отвечает за преобразование дерева разбора (AST), полученного в результате парсинга исходного кода ABAP,
 * в промежуточное представление, удобное для последующей оптимизации и генерации машинного кода.
 */

/**
 * Контекст генерации IR.
 * Хранит состояние во время обхода AST и генерации IR,
 * например счётчики меток для условных переходов, флаг возврата из функции и т.д.
 */
typedef struct IRGenContext {
    int label_counter;       ///< Счётчик для генерации уникальных меток
    bool has_return;         ///< Флаг, указывающий была ли встречена инструкция RETURN
    // Можно добавить указатели на таблицы символов, текущий блок и др.
} IRGenContext;

/**
 * Инициализация контекста генерации IR.
 * Обнуляет счётчики и флаги.
 * 
 * @param ctx Указатель на структуру контекста.
 */
void irgen_init_context(IRGenContext *ctx);

/**
 * Генерация IR для всего AST.
 * 
 * @param root Указатель на корень AST (абстрактного синтаксического дерева).
 * @param ctx Контекст генерации IR.
 * @return Указатель на корень сгенерированного IR.
 */
IRNode* irgen_generate_ir(ASTNode *root, IRGenContext *ctx);

/**
 * Генерация IR для выражений.
 * Обрабатывает разные типы выражений: литералы, переменные, операции, вызовы функций и т.д.
 * 
 * @param expr Указатель на узел AST выражения.
 * @param ctx Контекст генерации IR.
 * @return Указатель на IR узел, соответствующий выражению.
 */
IRNode* irgen_generate_expression(ASTNode *expr, IRGenContext *ctx);

/**
 * Генерация IR для условных операторов IF.
 * Создаёт метки переходов, ветвления и блоки кода для всех частей условия.
 * 
 * @param if_node Узел AST с условием IF.
 * @param ctx Контекст генерации IR.
 * @return Указатель на корневой IR узел условия.
 */
IRNode* irgen_generate_if(ASTNode *if_node, IRGenContext *ctx);

/**
 * Генерация IR для циклов (LOOP, WHILE, DO).
 * Обрабатывает тело цикла, условие и управляющие конструкции.
 * 
 * @param loop_node Узел AST цикла.
 * @param ctx Контекст генерации IR.
 * @return Указатель на корневой IR узел цикла.
 */
IRNode* irgen_generate_loop(ASTNode *loop_node, IRGenContext *ctx);

/**
 * Генерация IR для присваиваний.
 * 
 * @param assign_node Узел AST присваивания.
 * @param ctx Контекст генерации IR.
 * @return Указатель на IR узел присваивания.
 */
IRNode* irgen_generate_assignment(ASTNode *assign_node, IRGenContext *ctx);

/**
 * Генерация IR для вызова функции или метода.
 * 
 * @param call_node Узел AST вызова функции.
 * @param ctx Контекст генерации IR.
 * @return Указатель на IR узел вызова функции.
 */
IRNode* irgen_generate_function_call(ASTNode *call_node, IRGenContext *ctx);

/**
 * Генерация IR для возврата из функции.
 * Обрабатывает оператор RETURN.
 * 
 * @param return_node Узел AST возврата.
 * @param ctx Контекст генерации IR.
 * @return Указатель на IR узел возврата.
 */
IRNode* irgen_generate_return(ASTNode *return_node, IRGenContext *ctx);

#endif // IR_GENERATOR_H
```

---

### Объяснение

* Заголовок структур и функций снабжён подробными комментариями в стиле Doxygen.
* Контекст генерации `IRGenContext` содержит состояние генератора (например, счётчик меток для уникальности).
* Функции описаны по основным видам синтаксических конструкций, что облегчает расширение.
* Включены необходимые заголовки для определения AST и IR.

Если нужно — могу помочь и с реализацией этих функций или с другими модулями.
