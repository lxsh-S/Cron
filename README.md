# Cron

<img width="730" height="402" alt="2026_10_10_21_26_42_screenshot" src="https://github.com/user-attachments/assets/cbcca6f3-b0c7-4c5f-a163-0b8aaf3f764f" />

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

### Execute Commands

The shell check is the command is built or not. If it isn't the shell uses child processes

- Builtin Commands

Internal commands like (`cd`, `exit`) run in the shell process instead of spawning a child process for the obvious reasons

- External Commands

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
- '|' pipes
- Git commands [quotes support]
- use environment variables with pipes and I/O redirections

A bunch of shell features are still being impemented :P

### CURRENTLY WORKING ON??

A lot here too!!

- Command history -> done
- Persistent command history -> done
- Better prompts (show current directory)
- tab completion
- Better error handling

All of the new thing will be coming on release `0.4`
