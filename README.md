# Cron

<img width="852" height="334" alt="2026_10_06_17_50_02_screenshot" src="https://github.com/user-attachments/assets/36978579-5ebe-4bd2-91c4-e62827e0cffe" />

A Small, Simple and Lightweight shell written in C.
> Currently working on the tokenizer and the shell is in unusable state!
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
- cmatrix
- pipes.sh
- cat
- echo

### Pipelines

- ls | grep .c
- echo hello cron! > test.txt and can also append now`>>`
- cat < somefile.txt

## WHAT DOESNT WORK??

A LOT :(

Currently cron only supports:

- basic commands
- 2-command Pipelines
- '>' output redirection
- '>>' output redirection
- '<' to read files using cat/Input
  A bunch of shell features are still being impemented :P

### CURRENTLY WORKING ON??

- A proper tokenizer for complex commands!
