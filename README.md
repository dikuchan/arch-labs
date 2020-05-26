# Architecture for High-Performance Computing

## Build

Выполненные семестровые задания расположены в директории с соответствующим номером.

```bash
$ cd {semester}/homework
$ make
```

Способ задания входных данных изменяется через флаги компиляции.

## Usage

### Semester I

Запущенная без аргументов программа выводит результаты вычислений каждого из заданий.

```bash
$ ./1
```

Можно получить справку при указании параметра.

```bash
$ ./1 -i
```

А также сравнение результатов и их вывод.

```bash
$ ./1 -o
```

### Semester II

Задания по MPI расположены в одноименной директории.
Каждому заданию будет присовен номер после сборки через _Makefile_.

```bash
$ make
$ mpirun 1
$ Device 2 (pure) out of 2
$ Device 1 (pure) out of 2
```
