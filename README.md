# Cron

<img width="852" height="334" alt="2026_10_06_17_50_02_screenshot" src="https://github.com/user-attachments/assets/36978579-5ebe-4bd2-91c4-e62827e0cffe" />

A Small, Simple and Lightweight shell written in C.

> Currently working on adding new features, the shell is very bare bones

## OVERVIEW

### Core Loop

At the core, `Cron` runs a Read-Eval_print Loop (REPL): Like other shells, it prints a prompt, reads a line of the input given by the user, parser it into tokens and executes the command as a built in or by spawning a child process

### Parser

Currently `Cron's` parser tokenizes the user input into the following `token types`:

1. TOKEN_WORD - ex. 'WORKS'
2. TOKEN_PIPE - ex. '|'
3. TOKEN_REDIR_IN - ex. '<'
4. TOKEN_REDIR_OUT - ex. '>'
5. TOKEN_REDIR_APPEND - ex. '>>'

And then executes the command

## Execute Commands

The shell check is the command is built or not. If it isn't the shell uses child processes

### Builtin Commands

Internal commands like (`cd`, `exit`) run in the shell process instead of spawning a child process for the obvious reasons

### External Commands

Commands like I/O redirection require child processes to run, so the shells forks a child and the child calls `execvp()` to execute the commands while `Cron` calls `waitpid()` to wait for the command execution to be complete

## BUILD

```
make
```

Uses `GCC` and build `Object files`

## USAGE

```
./build/cron
```

## WHAT WORKS CURRENTLY??

### Basic commands

- ls
- cmatrix,
- pipes.sh and more commands like this
- cat
- echo
- cd
- help

### Pipelines

- ls | grep .c
- echo hello cron! > test.txt and can also append now`>>`
- cat < somefile.txt
- read/print environment variables `echo $HOME`

## WHAT DOESN'T WORK??

A LOT :(

Currently cron only supports:

- basic commands
- 2-command Pipelines
- '>' output redirection
- '>>' output redirection
- '<' to read files using cat/Input

A bunch of shell features are still being impemented :P

### CURRENTLY WORKING ON??

- Make git commands work
