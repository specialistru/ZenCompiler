#ifndef CODEGEN_H
#define CODEGEN_H

#include "ir.h"

// Структура для хранения состояния генератора кода
typedef struct {
    IRProgram *ir_program;   // Указатель на промежуточное представление
    // Здесь могут быть дополнительные поля, например, буферы для выходного кода
} CodeGenContext;

// Инициализация генератора кода
void codegen_init(CodeGenContext *ctx, IRProgram *ir);

// Генерация кода из IR
int codegen_generate(CodeGenContext *ctx);

// Очистка ресурсов генератора кода
void codegen_cleanup(CodeGenContext *ctx);

#endif // CODEGEN_H
