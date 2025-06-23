Следующий файл из директории `include/` — `debugger.h`

---

### Назначение `debugger.h`:

Объявляет интерфейс отладчика — поддержку точек останова, пошагового выполнения, инспекции переменных и состояния программы во время исполнения.

---

## Пример реализации `debugger.h`

```c
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "ir.h"

// Тип для точки останова
typedef struct {
    int instruction_index;  // Индекс инструкции, где установлен брейкпоинт
    int enabled;            // Включен ли брейкпоинт
} Breakpoint;

// Контекст отладчика
typedef struct {
    Breakpoint *breakpoints;
    int count;
    int capacity;
} DebuggerContext;

// Инициализация отладчика
void debugger_init(DebuggerContext *dbg);

// Добавление точки останова
int debugger_add_breakpoint(DebuggerContext *dbg, int instruction_index);

// Удаление точки останова
int debugger_remove_breakpoint(DebuggerContext *dbg, int instruction_index);

// Проверка наличия точки останова для данной инструкции
int debugger_check_breakpoint(DebuggerContext *dbg, int instruction_index);

// Очистка ресурсов отладчика
void debugger_cleanup(DebuggerContext *dbg);

#endif // DEBUGGER_H
```

---

Если нужно, могу помочь с реализацией `debugger.c` или следующим файлом.

