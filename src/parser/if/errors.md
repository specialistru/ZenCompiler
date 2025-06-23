Следующий файл — `parser/if/errors.c`.

---

### Задача `errors.c`:

Обработка ошибок парсинга внутри блока IF — предоставление информативных сообщений об ошибках для операторов IF.

---

## Реализация `errors.c`

```c
#include "../../include/error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * report_if_error - Выводит информативное сообщение об ошибке парсинга IF.
 * @msg: Сообщение ошибки.
 * @line: Номер строки, где произошла ошибка.
 */
void report_if_error(const char *msg, int line) {
    if (!msg) return;
    fprintf(stderr, "Parsing error in IF statement at line %d: %s\n", line, msg);
}
```

---

### Объяснение:

* Простая функция вывода ошибок с указанием строки.
* Можно расширить для интеграции с общей системой ошибок.

---

Если нужно, могу сделать следующий файл.
