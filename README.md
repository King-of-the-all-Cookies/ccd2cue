# English

# CCD2CUE - CCD to CUE File Converter

## Description

**ccd2cue** - Utility for converting CD/DVD image files in CCD (CloneCD) format to CUE files.

## Features

- Converts CCD files to CUE format
- Automatically detects image files (.img, .bin, .iso)
- Supports various track types (data and audio)
- Correctly handles timestamps and track indices
- No external dependencies (except the C standard library)

## Installation and Compilation

### Requirements
- GCC compiler
- C standard library
- POSIX-compliant operating system

### Compilation

```bash
gcc -o ccd2cue ccd2cue.c
```

## Usage

### Basic Syntax

```bash
ccd2cue -c <file.ccd>
```

### Command Line Options

| Option | Description |
|----------|----------|
| `-c <file>` | Required parameter. Specifies the path to the input CCD file. |

### Usage Examples

```bash
# Convert file example.ccd
./ccd2cue -c example.ccd

# Convert a file in a different directory
./ccd2cue -c /path/to/disc.ccd
```

## Input and Output Files

### Input Files
- **CCD File**: A file with the `.ccd` extension, containing metadata about the disc structure
- **Image File**: Automatically detected by extensions:
- `.img`
- `.bin`
- `.iso`

### Output Files
- **CUE File**: Created with the same name as the CCD file, but with the `.cue` extension

### Example File Structure

```
project_directory/
├── disc.ccd # Input File
├── disc.img # Image file (auto-detection)
└── disc.cue # Output file (created by the program)
```

## Supported Track Formats

The program supports two main track types:

| Track Type | Description | CUE Format |
|-----------|-----------|-----------|
| Data | Computer Data Tracks | `MODE1/2352` |
| Audio | Audio Tracks | `AUDIO` |

## Time Stamp Processing

The program automatically adjusts time stamps:
- Subtracts 2 seconds from the time of each track
- Correctly handles transitions after one minute
- Maintains frame accuracy

## Return Codes

| Code | Value |
|-----|----------|
| `0` | Successful completion |
| `1` | Error: No CCD file specified |
| `2` | Error: Unable to read CCD file |
| `3` | Error: Image file not found |

-----

# Русский

# CCD2CUE - Конвертер CCD в CUE файлы

## Описание

**ccd2cue** - Утилита для преобразования файлов образов CD/DVD в формате CCD (CloneCD) в CUE-файлы. 

## Особенности

- Конвертирует CCD-файлы в CUE-формат
- Автоматически определяет файлы образов (.img, .bin, .iso)
- Поддерживает различные типы треков (данные и аудио)
- Корректно обрабатывает временные метки и индексы треков
- Не требует внешних зависимостей (кроме стандартной библиотеки C)

## Установка и компиляция

### Требования
- Компилятор GCC
- Стандартная библиотека C
- POSIX-совместимая операционная система

### Компиляция

```bash
gcc -o ccd2cue ccd2cue.c
```

## Использование

### Базовый синтаксис

```bash
ccd2cue -c <файл.ccd>
```

### Параметры командной строки

| Параметр | Описание |
|----------|----------|
| `-c <файл>` | Обязательный параметр. Указывает путь к входному CCD-файлу |

### Примеры использования

```bash
# Конвертировать файл example.ccd
./ccd2cue -c example.ccd

# Конвертировать файл в другом каталоге
./ccd2cue -c /path/to/disc.ccd
```

## Входные и выходные файлы

### Входные файлы
- **CCD-файл**: Файл с расширением `.ccd`, содержащий метаданные о структуре диска
- **Файл образа**: Автоматически определяется по расширениям:
  - `.img`
  - `.bin` 
  - `.iso`

### Выходные файлы
- **CUE-файл**: Создается с тем же именем, что и CCD-файл, но с расширением `.cue`

### Пример структуры файлов

```
директория_проекта/
├── disc.ccd      # Входной файл
├── disc.img      # Файл образа (автоопределение)
└── disc.cue      # Выходной файл (создается программой)
```

## Форматы поддерживаемых треков

Программа поддерживает два основных типа треков:

| Тип трека | Описание | CUE-формат |
|-----------|----------|------------|
| Данные | Треки с компьютерными данными | `MODE1/2352` |
| Аудио | Аудио треки | `AUDIO` |

## Обработка временных меток

Программа автоматически корректирует временные метки:
- Вычитает 2 секунды из времени каждого трека
- Корректно обрабатывает переходы через минуту
- Сохраняет точность до кадров (frames)

## Коды возврата

| Код | Значение |
|-----|----------|
| `0` | Успешное выполнение |
| `1` | Ошибка: не указан CCD-файл |
| `2` | Ошибка: не удалось прочитать CCD-файл |
| `3` | Ошибка: не найден файл образа |

