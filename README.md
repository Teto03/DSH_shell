# dsh — a simple Unix shell in C

A minimal Unix shell written in C to understand how a shell works under the hood: command lookup in a configurable search path, output redirection and pipes, with process management through POSIX system calls.

## Features

- **Command execution** from a configurable search path (`setpath`)
- **Output redirection** with `>` and `>>`
- **Pipes** with `|`

## Code structure

| Function | Role |
|---|---|
| `prompt` | shows the prompt and reads user input |
| `set_path` / `path_lookup` | set the search path and resolve commands |
| `rel2abs` | converts relative paths to absolute |
| `do_redir` | output redirection |
| `do_pipe` | pipes |
| `do_exec` | command execution |
| `panic` | error handling |

## Build and run

Linux only (uses `sys/wait.h`).

```bash
gcc -o dsh dsh.c
./dsh
```

## Notes

Educational project: it does not implement all the features of a full shell and is not meant for production use.
