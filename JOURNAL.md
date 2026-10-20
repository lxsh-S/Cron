# JOURNAL

I'm starting a journal to document my learnings and progress from `20/10/26 (dd/mm/yy)` :)
(I was building before this but didnt journal :( )

# 20/10/26

Today I mostly fixed the errors that came with my current implementation of the tokenizer to tackle quotes in git commands. Operators such as `>, <, >>, |` are recognized as text by the tokenizer with was very needed to prevent errors

I faced errors with:

```
echo "hello \"LAKSH\"" - no escaped quotes support
echo "hello world - unmatched quotes
```

## Details

### Escape handling

- example -> `echo "hello \"LAKSH\""`
  before, `corn` used to treat any quote as the end of the text, but today I have changed the else statement to recognize `\` and so the next character after it to be treated as a litereal text.

I know this isn't the best implementation because suppose if I put something other than quote `cron` will still treat it as text. But I didnt account for that edge cause only because I have never needed to use an Operator with backslash and I think this shouldnt cause any problems.

### Mismatched quotes

- example -> `echo "hello world`
  Here, as you see we we havent closed the string but `cron` still outputs `hello world` in the terminal while other shells support multiline input after you press enter.

Because we have a `quote` variable that checks if we are inside a text or not the fix was easy. We simply check if we are still inside the quoted text when the input is passed, if we are we return `-1` and we don't execute the command then we print an unmatched quote error from "shell.c".
