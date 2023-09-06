# Репозиторий к семинарам по курсу "Алгоритмы и Алгоритмические Языки"

Курс читается для потока **"С"** 1-го курса ФКН ВШЭ.

## Установка ПО

**Увы, но приведённые решения были протестированы только для Linux.**

### Linux

#### Компилятор GCC
Для установки стоит выполнить:
```
❯ sudo apt install gcc
```

Проверка работоспособности:
```
❯ gcc: fatal error: no input files
compilation terminated.
```

#### Утилита для сборки проектов
Для установки стоит выполнить:
```
❯ sudo apt install make
```

Проверка работоспособности:
```
❯ make
make: *** No targets specified and no makefile found.  Stop.
```

### Mac OS

В качестве пакетного менеджера можно использовать [Homebrew](https://docs.brew.sh/Homebrew-on-Linux).

#### Компилятор GCC
Для установки стоит выполнить:
```
❯ brew install gcc
```

Проверка работоспособности:
```
❯ gcc: fatal error: no input files
compilation terminated.
```

#### Утилита для сборки проектов
Для установки стоит выполнить:
```
❯ brew install make
```

Проверка работоспособности:
```
❯ make
make: *** No targets specified and no makefile found.  Stop.
```

### Windows

Рекомендации:
- Можно применять [Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/install) и пользоваться командной строкой так же, как и на UNIX-совместимой операционной системе.
- Можно установить [VS Code](https://code.visualstudio.com/) и компилировать код встроенным компилятором IDE.
