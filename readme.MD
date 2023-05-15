## Abid Khursheed | 2021101136

---

## Requirements

- GCC compiler
- Linux OS (preferably Ubuntu)

## Running Shell

From the root directory of project, run `make` followed by `./shell`


## Assumptions
- Discover gives the path of all the subfiles and(or) subdirectories if no argument is given for comparision
- For discover command "arg" is the path to be seached and argument without quotes("") is the file/directory to be searched

- Reasonable assumptions are made for things like length of current path, number of commands, command length, etc. The shell will work perfectly for all "day to day work".

- Every user input will be stored as it is in history, given it is not same as the last input and contains at least one character other than space or tab.

- `&` (used for running process in background) can come anywhere in command.

- "exit" command is added to the shell. (Not given in the pdf).
- IO redirection is handled in the code. 
- ctrl +(D,Z,C) is handled in the code.
