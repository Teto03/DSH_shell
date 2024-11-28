dsh.C  A Simple Shell in C
This project is a straightforward implementation of a shell in the C programming language. The code includes basic functionalities such as executing commands, managing paths, redirecting output, and using pipes.

Features
Command Execution: The shell can execute commands specified within the path.
Path Management: It is possible to set the command search path using the setpath command.
Output Redirection: The shell supports output redirection using the symbols > and >>.
Pipes: The shell supports the use of pipes with the | symbol.
Code Structure
The code is organized into several functions, each handling a specific functionality of the shell:

panic: Prints an error message and terminates execution.
prompt: Displays the shell prompt and reads user input.
set_path: Sets the search path for commands.
path_lookup: Searches for a command within the specified path.
rel2abs: Converts a relative path into an absolute path.
do_redir: Handles output redirection.
do_pipe: Manages the use of pipes.
do_exec: Executes a command.
main: The main function that governs the shell's lifecycle.

How to Compile and Run
To compile the code, use the following gcc command:
  $ gcc -o dsh dsh.c  

To execute the shell, use the command:
   $ ./dsh  

Notes
This project serves as an example of how to implement a shell in C. It does not include all the features of a full-fledged shell and is not intended for production use.

IMPORTANT: Due to the use of the sys/wait library, the code must be executed in a LINUX environment. It will not function on Windows.
