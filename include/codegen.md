Следующий файл из директории `include/` — `codegen.h`

---

### Назначение `codegen.h`:

Объявляет интерфейс генератора кода — модуль, который преобразует промежуточное представление (IR) в целевой код, будь то байткод, машинный код или другой формат.

---

## Пример реализации `codegen.h`

```c
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
```

---

Если нужно, могу помочь с реализацией `codegen.c` или следующим файлом.

