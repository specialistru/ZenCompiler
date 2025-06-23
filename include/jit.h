#ifndef JIT_H
#define JIT_H

#include "ir.h"

// Структура контекста JIT-компилятора
typedef struct {
    IRProgram *program;      // IR-программа для компиляции
    void *native_code;       // Указатель на сгенерированный машинный код
    size_t code_size;        // Размер машинного кода
} JITContext;

// Инициализация JIT-компилятора
void jit_init(JITContext *ctx, IRProgram *program);

// Генерация машинного кода из IR
int jit_compile(JITContext *ctx);

// Выполнение скомпилированного кода
int jit_execute(JITContext *ctx);

// Очистка ресурсов JIT-компилятора
void jit_cleanup(JITContext *ctx);

#endif // JIT_H
