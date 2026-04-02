# 🐚 Tinsh (Minimal Unix Shell)

A lightweight, heavily documented, and highly functional Unix Shell implementation written entirely in C. 
It supports natively built-in commands, external commands execution via standard Unix processes (`fork` + `execvp`), and a custom Bash-like dynamic colored prompt!

***

## ✨ Features

- **Bash-like Dynamic Prompt:** Automatically detects your current user (`whoami`) and directory (`pwd`), displaying a beautiful colorful prompt: `user@myshell:/cwd$ `.
- **External Commands:** Seamlessly executes standard Linux commands (`ls`, `grep`, `pwd`, `mkdir`, etc.) by searching through your system's `$PATH`.
- **Built-in Commands:** Contains native commands designed to affect the internal system state directly:
  - `cd [dir]` - Change working directory (defaults to `$HOME`)
  - `pwd` - Print absolute working directory path
  - `echo [args]` - Print arguments to standard output
  - `whoami` - Print the name of the current logged-in user
  - `help` - Show the interactive dashboard/help screen
  - `exit [code]` - Terminate the shell properly
- **Signal Handling:** Implements smart signal handling (like `Ctrl+C` / `SIGINT`) which aborts the current typing line instead of crashing the shell itself.
- **Quote Parsing:** Parses single and double-quoted strings accurately, supporting spaces without splitting them into different arguments.

***

## 🚀 How to Run

If you are on Windows, ensure you have **WSL (Windows Subsystem for Linux)**, `gcc` and `make` packages installed. (Hint: you can install compiling packages via `sudo apt install build-essential`).

1. Open your terminal (PowerShell, CMD, or modern VS Code Terminal)
2. Enter your WSL Linux environment natively:
   ```bash
   wsl
   ```
3. Navigate into the source codes directory:
   ```bash
   cd myshell
   ```
4. Compile the project instantly using the provided `Makefile`:
   ```bash
   make
   ```
5. Run your new shiny shell:
   ```bash
   ./myshell
   ```

## 🛠️ Architecture & Methods
- **Language:** Written completely in Raw C (C11 Standard).
- **Core APIs:** Relies tightly on strict POSIX interfaces such as `fork()`, `execvp()`, `waitpid()`, `chdir()`, `getcwd()`, and `signal()`.
