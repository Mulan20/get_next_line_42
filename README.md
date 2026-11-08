Here is a GitHub README for **get_next_line** under 42 Bangkok, including the bonus part.

---

# get_next_line — 42 Bangkok

> Read a line from a file descriptor. One line at a time. No matter the buffer size.

## About

This repository contains my **get_next_line** project completed at **42 Bangkok**, including the **bonus** part. The goal is to write a function that returns a single line read from a file descriptor, and can be called repeatedly to read the entire file line by line — regardless of the `BUFFER_SIZE` used at compile time.

The function prototype is:

```c
char *get_next_line(int fd);
```

It returns the line that was read (including the trailing `\n` if present), or `NULL` if there is nothing left to read or an error occurs.

## Project Structure

```
.
├── Makefile
├── get_next_line.h
├── get_next_line.c
├── get_next_line_utils.c
├── get_next_line_bonus.h
├── get_next_line_bonus.c
├── get_next_line_utils_bonus.c
└── README.md
```

The `_bonus` files implement the multi-file-descriptor version. The `Makefile` can compile either the mandatory or bonus part depending on the target.

## Environment & Compilation

**Requirements:**
- Unix-like system (Linux / macOS)
- `cc` or `gcc`
- `make` (optional, depending on your setup)

**Compile the mandatory part:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c your_main.c -o gnl
```

**Compile the bonus part:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c your_main_bonus.c -o gnl_bonus
```

`BUFFER_SIZE` can be any positive integer — `1`, `42`, `9999`, etc. The function must work correctly for all of them.

**42 Norm check:**

```bash
norminette
```

## How It Works

The core idea is to keep a **static buffer** (or a static array of buffers in the bonus) that persists between calls to `get_next_line`. Each call:

1. **Checks the static buffer** for a newline character (`\n`).
2. **If a newline exists**, extract everything up to and including the newline as the line to return, and keep the remainder in the static buffer for the next call.
3. **If no newline exists**, read from the file descriptor using `read()` in chunks of `BUFFER_SIZE`, appending to the static buffer until a newline is found or `read` returns `0` (EOF).
4. **Returns the extracted line**, or `NULL` if there is nothing left to read.

### Key Design Points

- **Static variable:** The buffer must be `static` so its contents survive between function calls.
- **No global variables:** The 42 Norm forbids globals, so the buffer must be a static local variable.
- **Memory management:** Every allocation must be freed appropriately. Leaks are checked by the Moulinette and francinette.
- **`read()` return value:** Handle `-1` (error), `0` (EOF), and positive values correctly.

## Bonus Part — Multiple File Descriptors

The bonus requires `get_next_line` to manage **multiple file descriptors simultaneously** without mixing up their buffers. For example:

```c
int fd1 = open("file1.txt", O_RDONLY);
int fd2 = open("file2.txt", O_RDONLY);

char *line1 = get_next_line(fd1);
char *line2 = get_next_line(fd2);
char *line3 = get_next_line(fd1);  // continues where line1 left off
```

To achieve this, the static buffer is replaced by a **static array of buffers**, indexed by file descriptor:

```c
static char *buffer[OPEN_MAX];
```

`OPEN_MAX` is the maximum number of file descriptors a process can have open simultaneously (typically 1024 on Linux). Each `fd` gets its own independent buffer, so interleaved reads from different files work correctly.

### Bonus Checklist

- [x] `get_next_line` works with multiple file descriptors
- [x] Buffers do not interfere with each other
- [x] No leaks when reading from many files
- [x] Works with `BUFFER_SIZE=1` and large buffer sizes
- [x] Handles `fd` values that are opened and closed at different times

## Testing

Common ways to test `get_next_line`:

1. **Basic test:** Read a small text file line by line and compare with `cat`.
2. **Buffer size stress test:** Compile with `BUFFER_SIZE=1`, `BUFFER_SIZE=42`, `BUFFER_SIZE=9999` and verify identical output.
3. **Edge cases:**
   - Empty file
   - File with no trailing newline
   - File with only newlines
   - Very long lines
   - `fd = -1` or invalid file descriptor
4. **Bonus test:** Open multiple files, interleave `get_next_line` calls, verify each file's lines come out correctly.
5. **Leak check:** Run with `valgrind` or francinette to confirm no memory leaks.

**Francinette** is the most commonly used tester by 42 students and covers both mandatory and bonus parts.

## What I Learned

- **Static variables:** How to preserve state between function calls without using globals.
- **`read()` system call:** Understanding file descriptors, buffer sizes, and the difference between `0`, `-1`, and positive return values.
- **Dynamic memory:** Building strings incrementally with `malloc`, `free`, and `ft_strjoin`-style helpers, while avoiding leaks.
- **Multiple file descriptors (bonus):** Managing an array of static buffers indexed by `fd`, and keeping them independent.
- **The 42 Norm:** No `for` loops, functions ≤ 25 lines, no globals, consistent naming.
- **Defensive programming:** Always check `malloc` return values, always free what you allocate, always handle `NULL` returns.

## Notes

This project is a personal learning record. It is not guaranteed to pass the Moulinette or francinette. If you are also at 42 Bangkok, try implementing it yourself first — `get_next_line` is one of the best projects for understanding memory and file I/O deeply.

## References

- `man 2 read`
- `man 3 malloc` / `man 3 free`
- 42 Bangkok official get_next_line subject (Intra)
- 42 Norm documentation
- Francinette tester (widely used by 42 students)

