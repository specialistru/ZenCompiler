Следующий файл из директории `include/` — `interpreter.h`

---

### Назначение `interpreter.h`:

Объявляет интерфейс интерпретатора промежуточного представления (IR) — модуль, который выполняет байткод или IR-программу, поддерживает стековую машину, управление функциями, рекурсией и локальными переменными.

---

## Пример реализации `interpreter.h`

```c
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ir.h"

// Структура контекста интерпретатора
typedef struct {
    IRProgram *program;     // IR-программа для исполнения
    int *stack;             // Стек выполнения
    int stack_size;         // Размер стека
    int sp;                 // Указатель стека
    int pc;                 // Счётчик команд (program counter)
} Interpreter;

// Инициализация интерпретатора с IR-программой
void interpreter_init(Interpreter *interp, IRProgram *program);

// Запуск интерпретатора
int interpreter_run(Interpreter *interp);

// Очистка ресурсов интерпретатора
void interpreter_cleanup(Interpreter *interp);

#endif // INTERPRETER_H
```

---

Если хочешь, могу помочь с исходником `interpreter.c` или следующим файлом.
