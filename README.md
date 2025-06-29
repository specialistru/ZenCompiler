# Детальная структура проекта компилятора (промышленный уровень)

### 📁 1. / — Корневая директория проекта

```
/ZenCompiler
├── include/                 # Заголовочные файлы (API, структуры, константы)
├── src/                     # Исходный код по модулям
├── tests/                   # Юнит-тесты и интеграционные тесты
├── examples/                # Примеры ABAP-программ
├── docs/                    # Документация (архитектура, гайды, спецификации)
├── scripts/                 # Скрипты для сборки, анализа, CI/CD
├── tools/                   # Вспомогательные инструменты (дебаггеры, профайлеры)
├── build/                   # Автоматическая сборка, артефакты
├── bin/
├── lib/
└── README.md
```

### 📁 2. include/ — Заголовочные файлы

> Общие заголовочные файлы. Используются во всех модулях проекта. Обеспечивают связь между компиляционными единицами и определяют базовые структуры, типы, макросы и интерфейсы.

```
include/
├── common.h                # Общие макросы, типы и утилиты, глобальные определения, inline-функции, используемые во многих частях проекта.
├── config.h                # Конфигурация сборки, параметры и флаги управления компилятором (включение JIT, уровень оптимизации и т.д.).
├── error.h                 # Унифицированная система кодов ошибок, макросы для их генерации и обработки.
├── utf8.h                  # Утилиты для работы с UTF-8 строками, символами Unicode и кодировкой.
├── token.h                 # 
├── ast.h                   # Синтаксический анализ и AST.
├── parser.h                # 
├── lexer.h                 # Лексический анализ.
├── semantic.h              # Семантический анализ (анализ типов, областей видимости).
├── ir.h                    #
├── ir_api.h                # Промежуточное представление и API работы с IR.
├── optimizer.h             # Оптимизации IR.
├── vm.h                    # Виртуальная машина (исполнение IR).
├── runtime.h               # 
├── symbol_table.h          # Таблицы символов и области видимости.
├── type_checker.h          # Проверка типов.
├── diagnostics.h           # Обработка ошибок и предупреждений.
├── logger.h                # Логирование.
├── debug.h                 # Отладка.
├── abap_keywords.h         # Список ключевых слов ABAP.
├── stdlib.h                # Стандартные библиотеки ABAP (константы, функции).
├── cli.h                   # Командная строка и парсинг параметров.
```

### 📁 3. src/ — Основной код компилятора

```
src/
├── core/                   # Точка входа, конфигурация, CLI
├── lexer/                  # Лексический анализ
├── parser/                 # Синтаксический анализ
├── semantic/               # Семантический анализ
├── ir/                     # Промежуточное представление
├── opt/                    # Оптимизации IR
├── vm/                     # Виртуальная машина
├── runtime/                # Среда выполнения ABAP
├── tools/                  # Инструменты разработчика
```

#### 📁 3.1. src/core/ - Точка входа, конфигурация, CLI

```
├── core/                   # Точка входа, конфигурация, CLI
│   ├── main.c              # Запуск компилятора, основной цикл, CLI интерфейс: принимает путь к файлу ABAP. Главный исполняемый файл, точка запуска CLI. Инициализирует лексер, парсер, анализатор и генератор кода. Принимает параметры командной строки.
│   ├── config.c            # Загрузка параметров. Чтение конфигурационных флагов компилятора. Загружает и обрабатывает параметры конфигурации, заданные пользователем или через переменные окружения.
│   ├── cli.c               # Обработка аргументов командной строки
```

#### 📁 3.2. src/lexer/ - Лексический анализ

> Отвечает за разбор исходного кода на токены: ключевые слова, идентификаторы, литералы, операторы и т.п.

```
├── lexer/                  # Лексический анализ
│   ├── lexer.c             # Парсер токенов. Разбор входного текста в токены. Основной лексический анализатор. Сканирует символы и генерирует последовательность токенов.
│   ├── token.c             # Определение токенов, потоки токенов. Структура и типизация токенов. Реализация структуры `Token`, определение типов токенов, их форматирование, сравнение.
│   ├── token_stream.c      # Буферизация токенов, lookahead и откат. Буферизация токенов, поддержка lookahead, сохранение позиции и откат.
```

#### 📁 3.3. src/parser/ - Синтаксический анализ. Парсер и модули синтаксического анализа

> Парсинг языка ABAP. Представляет синтаксис в виде дерева AST. Делится на подкатегории по типам конструкций языка.

```
├── parser/                 # Синтаксический анализ
│   ├── parser.c            # Центральный LL(1) или рекурсивный нисходящий парсер, управляющий разбором модулей.
│   ├── ast.c               # Создание и управление узлов AST для каждого синтаксического элемента, поддержка вывода дерева.
│   ├── ast_visitor.c       # Обход дерева AST (для отладки, IR, вывода), преобразования
```

#### Подкаталоги:

│   ├── parser/                 # Синтаксический анализ
│   │   ├── if/
│   │   ├── perform/
│   │   ├── loop/
│   │   ├── call_function/
│   │   ├── class/
│   │   ├── method/
│   │   ├── expression/
│   │   ├── form/
│   │   ├── module/
│   │   ├── try/
│   │   ├── select/
│   │   ├── table_ops/
│   │   ├── declarations/
│   │   ├── control/
│   │   ├── special/

> Каждый из них содержит пофайловое разбиение логики: простые, вложенные, с ошибками, с условиями, с блоками тела и т.д. Это позволяет гибко тестировать и расширять поддержку ABAP.

#### 📁 3.3.1. src/parser/if/ - Модули для if-конструкций

```
│   │   ├── if/                          # Модули для if-конструкций
│   │   │   ├── simple.c                 # Обработка простых IF-условий без ветвлений и логических операций.
│   │   │   ├── nested.c                 # Разбор вложенных конструкций IF внутри других IF/ELSE.
│   │   │   ├── elseif.c                 #
│   │   │   ├── else.c                   #
│   │   │   ├── endif.c                  #
│   │   │   ├── logical_ops.c            #
│   │   │   ├── not.c                    #
│   │   │   ├── bracketed.c              #
│   │   │   ├── errors.c                 #
│   │   │   ├── multilevel_elseif_else.c #
│   │   │   ├── deep_elseif.c            #
│   │   │   ├── complex_conditions.c     #
│   │   │   ├── body.c                   #
│   │   │   ├── complex_body.c           #
```

#### 📁 3.3.2. src/parser/perform/ - Модули для PERFORM

```
│   │   ├── perform/            # Модули PERFORM
│   │   │   ├── simple.c
│   │   │   ├── nested.c
│   │   │   ├── logical_ops.c
│   │   │   ├── and_or.c
│   │   │   ├── not.c
│   │   │   ├── elseif.c
│   │   │   ├── else.c
│   │   │   ├── bracketed.c
│   │   │   ├── complex.c
│   │   │   ├── errors.c
```

#### 📁 3.3.3. src/parser/call_function/ - Вызовы функций

```
│   │   ├── call_function/      # Вызовы функций
│   │   │   ├── simple.c
│   │   │   ├── dynamic.c
│   │   │   ├── exceptions.c
│   │   │   ├── bracketed.c
│   │   │   ├── complex.c
```

#### 📁 3.3.4. src/parser/class/ - Обработка классов

```
│   │   ├── class/               # Обработка классов
│   │   │   ├── def.c
│   │   │   ├── attributes.c
│   │   │   ├── interface.c
│   │   │   ├── method_def.c
│   │   │   ├── method_impl.c
│   │   │   ├── simple.c
│   │   │   ├── visibility.c
│   │   │   ├── implementation.c # Разбор блока реализации класса, включая методы.
│   │   │   ├── endinterface.c
│   │   │   ├── endclass.c
│   │   │   ├── errors.c
```

#### 📁 3.3.5. src/parser/method/ - Методы классов

```
│   │   ├── method/             # Методы классов
│   │   │   ├── definition.c
│   │   │   ├── implementation.c
│   │   │   ├── visibility.c
│   │   │   ├── error.c
```

#### 📁 3.3.6. src/parser/loop/ - Циклы (DO, WHILE)

```
│   │   ├── loop/                 # Циклы (DO, WHILE)
│   │   │   ├── loop.c
│   │   │   ├── do.c
│   │   │   ├── while.c           # Разбор конструкции WHILE ... ENDWHILE.
│   │   │   ├── other_construct.c
```

#### 📁 3.3.7. src/parser/expression/ - Выражения и операции

```
│   │   ├── expression/         # Выражения и операции
│   │   │   ├── literal.c
│   │   │   ├── identifier.c
│   │   │   ├── operator.c
│   │   │   ├── variable.c
│   │   │   ├── bracket.c
│   │   │   ├── function_call.c
│   │   │   ├── ternary.c
│   │   │   ├── conditional.c
│   │   │   ├── assignment.c    # Выражения присваивания с разной степенью вложенности.
│   │   │   ├── array_access.c
│   │   │   ├── logical.c
│   │   │   ├── complex.c
```

#### 📁 3.3.8. src/parser/assignment/ - Присваивания

```
│   │   ├── assignment/         # Присваивания
│   │   │   ├── simple.c
│   │   │   ├── chain.c
│   │   │   ├── complex.c
```

#### 📁 3.3.9. src/parser/form/ - Формы

```
│   │   ├── form/               # Формы
│   │   │   ├── simple.c
│   │   │   ├── complex.c
│   │   │   ├── syntax_error.c
```

#### 📁 3.3.10. src/parser/module/ - Модули

```
│   │   ├── module/             # Модули
│   │   │   ├── simple.c
│   │   │   ├── complex.c
│   │   │   ├── chain_and_field.c
```

#### 📁 3.3.11. src/parser/try/ - TRY-CATCH

```
│   │   ├── try/                # TRY-CATCH
│   │   │   ├── simple.c
│   │   │   ├── catch.c
│   │   │   ├── cleanup.c
│   │   │   ├── nested.c
│   │   │   ├── errors.c
```

#### 📁 3.3.12. src/parser/select/ - SQL SELECT и связанные

```
│   │   ├── select/             # SQL SELECT и связанные
│   │   │   ├── simple.c
│   │   │   ├── into_table.c
│   │   │   ├── join.c
│   │   │   ├── where.c
│   │   │   ├── endselect.c
```

#### 📁 3.3.13. src/parser/table_ops/ - Операции с таблицами

```
│   │   ├── table_ops/          # Операции с таблицами
│   │   │   ├── append.c
│   │   │   ├── insert.c
│   │   │   ├── delete.c
│   │   │   ├── modify.c
│   │   │   ├── read.c
│   │   │   ├── sort.c
```

#### 📁 3.3.14. src/parser/declarations/ - Объявления переменных и типов

```
│   │   ├── declarations/       # Объявления переменных и типов
│   │   │   ├── data.c
│   │   │   ├── constants.c
│   │   │   ├── parameters.c
│   │   │   ├── types.c
│   │   │   ├── field_symbols.c
│   │   │   ├── select_options.c
│   │   │   ├── ranges.c
```

#### 📁 3.3.15. src/parser/declarations/ - Управляющие конструкции (EXIT, CONTINUE)

```
│   │   ├── control/            # Управляющие конструкции (EXIT, CONTINUE)
│   │   │   ├── exit.c
│   │   │   ├── continue.c
│   │   │   ├── check.c
│   │   │   ├── return.c
│   │   │   ├── auth_check.c
```

#### 📁 3.3.16. src/parser/declarations/ - Специальные конструкции (MESSAGE, AUTH_CHECK)

```
│   │   ├── special/            # Специальные конструкции (MESSAGE, AUTH_CHECK)
│   │   │   ├── export_import.c
│   │   │   ├── memory_id.c
│   │   │   ├── set_get_parameter.c
│   │   │   ├── free_create_object.c
│   │   │   ├── authority_check.c
│   │   │   ├── message.c
```

#### 📁 3.4. src/semantic/ - Семантический анализ

> Семантический анализ AST: проверка типов, объявлений, областей видимости.

```
├── semantic/               # Семантический анализ
│   ├── semantic_analyzer.c # Проверка семантики AST. Анализ всех узлов AST. Основной анализатор: проходит по AST, строит таблицы символов, проверяет корректность конструкции.
│   ├── symbol_table.c      # Таблицы символов и области видимости. Иерархия областей видимости. Реализация таблиц символов с поддержкой вложенных областей.
│   ├── type_checker.c      # Проверка типов и совместимости. Статическая проверка типов, сопоставление типов переменных, параметров и возвращаемых значений.
```

#### 📁 3.5. src/ir/ - Промежуточное представление

> Генерация промежуточного представления (IR), пригодного для интерпретации или JIT-компиляции.

```
├── ir/                     # Промежуточное представление
│   ├── ir.c                # Определения IR-команд. Реализация структуры IR-команд, операндов, функций IR-VM.
│   ├── ir_generator.c      # Генерация IR из AST. Преобразование AST в IR-код. Отвечает за генерацию управляющих конструкций, вызовов, переменных.
│   ├── ir_api.c            # API для работы с IR (создание, изменение)
```

#### 📁 3.6. src/opt/ - Оптимизации IR

> Оптимизации IR до и после генерации: мёртвый код, свёртывание констант, инлайнинг.

```
├── opt/                    # Оптимизации IR
│   ├── optimizer.c         # Контроллер оптимизации: управляет стадиями и последовательностью оптимизаций.. Центральная точка управления оптимизациями.
│   ├── dead_code_elim.c    # Удаление мёртвого кода. Удаляет команды и ветви IR, которые не влияют на итоговое выполнение.
│   ├── const_fold.c        # Константное свёртывание. Заменяет выражения с константами на предвычисленные значения.
│   ├── inlining.c          # Инлайнинг функций. Вставка тела функций в место вызова для уменьшения накладных расходов.
│   ├── loop_opt.c          # Разворачивание циклов, распараллеливание, оптимизация условий выхода. Оптимизация циклов. 
```

#### 📁 3.7. src/vm/ - Виртуальная машина

> Исполнение IR-кода — интерпретация или JIT.

```
├── vm/                     # Реализация виртуальной машины на основе стека или регистров. Поддержка вызовов, условий, циклов.
│   ├── vm.c                # Исполнение и интерпретатор IR
│   ├── call_stack.c        # Стек вызовов для управления локальными переменными, параметрами, рекурсией. Управление вызовами и локальными переменными.
│   ├── jit.c               # Модуль JIT-компиляции: преобразование IR в нативный машинный код (опционально).
```

#### 📁 3.8. src/runtime/ - Среда выполнения ABAP

> Эмуляция среды выполнения ABAP: работа с внутренними таблицами, объектами, исключениями и базой данных.

```
├── runtime/                # Среда выполнения ABAP
│   ├── sql_api.c           # Поддержка Интерфейса и реализация OpenSQL (SELECT, INSERT, UPDATE и т.д.). Эмуляция поведения на C.
│   ├── internal_tables.c   # Работа с внутренними таблицами. Реализация табличных операций. Поддержка внутренних таблиц ABAP: SORT, READ, APPEND, MODIFY.
│   ├── objects.c           # Объекты и классы в рантайме. Работа с классами, объектами, интерфейсами.
│   ├── exceptions.c        # Реализация механизма TRY-CATCH и обработка исключений. Генерация исключения, TRY-CATCH
│   ├── threads.c           # Потоки и синхронизация. Поддержка параллелизма, блокировки и синхронизации в стиле ABAP.
```

#### 📁 3.9. src/tools/ - Инструменты разработчика

> Утилиты для разработчиков: отладка, логирование, трассировка, профилирование.

```
tools/
├── debugger.c              # Пошаговое выполнение, Отладчик, брейкпоинты. Установка точек останова, пошаговое выполнение, вывод текущего состояния.
├── profiler.c              # Сбор статистики по выполнению IR: время, количество вызовов, глубина стека. Профайлер исполнения IR.
├── logger.c                # Система логирование событий и ошибок, уровни сообщений, вывод в файл или консоль.
├── trace.c                 # Подробная трассировка выполнения IR-команд, полезна при отладке сложных случаев.
```

### 📁 4. tests/ — Тесты

> Содержит модульные и интеграционные тесты. Проверяют правильность работы отдельных компонентов: лексера, парсера, IR и т.д.

* Юнит-тесты для каждого ключевого модуля (lexer\_test.c, parser\_test.c, semantic\_test.c, ir\_test.c, vm\_test.c и т.д.).
* Тесты для отдельных модулей, используют фреймворк типа `Check`, `Unity`, `CuTest`.
* Интеграционные тесты на примерах ABAP-кода из папки examples/
* Регрессионные тесты для ключевых сценариев (tests/regression/)
* Автоматизированное покрытие тестами и CI

```
tests/
├── lexer_test.c
├── parser_test.c
├── semantic_test.c
├── ir_test.c
├── vm_test.c
├── regression/
│   ├── test_if.abap
│   ├── test_perform.abap
│   ├── test_sql.abap
```

### 📁 5. examples/ — Примеры ABAP-программ

> ABAP-примеры, используемые для тестирования и демонстрации возможностей компилятора. Запускаться через CLI.

```
examples/
├── hello_world.abap        # 
├── nested_if.abap          #
├── loop_select.abap        #
├── classes.abap            #
├── и др. для демонстрации возможностей компилятора
```

### 📁 6. docs/ — Документация

> Документация проекта для разработчиков и пользователей.

```
docs/
├── architecture.md         # Подробное описание архитектуры компилятора, модульные зависимости, контроль потоков.
├── grammar.md              # Описание синтаксиса ABAP в терминах BNF/EBNF. Полезно для расширения парсера и генерации AST.
├── ir_spec.md              # Описание структуры промежуточного представления, команд IR, регистра/стеков и семантики.       
├── build.md                # Инструкция по сборке проекта с использованием Make/CMake, установка зависимостей.
├── optimization.md         # Описание оптимизаций и стратегий.
```

### 📁 7. scripts/ — Скрипты

* Сборка проекта (Makefile, CMakeLists.txt, shell-скрипты)
* Анализ кода (статический анализ, форматирование)
* CI/CD — запуск тестов, деплой

### 📁 8. tools/ — 

├── tools/

### 📁 9. build/ - 

> Каталог для временных файлов сборки, сгенерированных компилятором (например, `.o`, `.d`, `.dep` файлы). Не попадает под контроль версий. Используется системой сборки Make или CMake.

### 📁 10. bin/ - 

> Сюда помещаются собранные исполняемые бинарные файлы (например, `abapc`, `abapc-cli`). Это финальные артефакты, которые можно запускать для компиляции и анализа ABAP-кода.

### 📁 11. lib/ - 

> Внешние библиотеки, используемые в проекте (например, `utf8proc`, `libsqlmock`, `libjit`). Подключаются как подмодули и как скомпилированные `.a`/`.so` файлы.

## 📦 Количество исходников:

* `.c` файлов: **более 160**
* `.h` файлов: \~**20-30**
* Всего: \~**200 файлов** — реалистичный размер для компилятора среднего уровня сложности.
