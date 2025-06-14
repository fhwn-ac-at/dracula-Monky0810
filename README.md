# Snakes & Ladders Simulator

A Markov-Chain Monte-Carlo Simulator for the "Snakes & Ladders" game

---

## Project structure

- pfusch
    - Makefile 
    - READMe.md
    - .gitignore 
    - board.txt -> s & l board config
    - src/ -> all .c and .h files
        - board.c
        - board.h
        - cli.c 
        - cli.h
        - die.c
        - die.h
        - main.c
        - sim.c
        - sim.h 
        - stats.c
        - stats.h

Each part of the program lives in a `.c/.h` pair.
The **Makefile** then compiles under `-std=c17 -Wall -Werror -Isrc` into a single executable named **pfusch**.

---

## Building the solution

In the terminal from the project root simply run `make` which then compiles every `src/*.c` into `src/*.o`. These object files will be linked into the **pfusch** executable. 

---

## Running the executable

In the terminal write `./pfusch -c <board_file>` to do a standard simulation run of the board. The `-c <board_file>` is a minimum requirement otherwise the executable does not have a board to simulate.

There are optional flags which can be added to the standard command which are listed below:

| Flag | Description                                                          | Default    |
| ---- | -------------------------------------------------------------------- | ---------- |
| `-d` | Number of die faces                                                  | 6          |
| `-p` | Comma-separated face probabilities (must supply exactly `-d` values) | uniform    |
| `-i` | Number of games to simulate                                          | 10000      |
| `-s` | Max rolls per game before abort                                      | 10000      |
| `-e` | Win by exceeding last square                                         | on         |
| `-x` | Must land exactly on last square                                     | off        |
| `-S` | RNG seed                                                             | time(null) |

---

## Board config file

The board is defined by a simple text file. Blank lines are allowed. A template for writing your own board is shown below. If the template shown in the *README.md* is not sufficient enough there are five pre configured `board.txt` files which can be looked at to inspire a custom board. These five boards can also be used for the simulation.  

- The first line for the board conifg file must be two integers to define the board size
- Then each ladder needs to be defined using the format *L start end* and must always go up using numbers greater or equal to 1 but smaller than the highest number of the board (10x10 -> smaller than 100)
- Then each snake needs to be defined using the format *S start end* and must always go down using numbers greater or equal to 1 but always smaller than the highest number of the board(10x10 -> smaller than 100)

*Board size*
10 10

*Snakes and Ladders configuration*

|Type  | Start | End |
|------|-------|-----|
| L    | 1     | 38  |
| L    | 4     | 14  |
| L    | 9     | 31  |
| L    | 21    | 42  |
| L    | 28    | 84  |
| L    | 36    | 44  |
| L    | 51    | 67  |
| L    | 71    | 91  |
| L    | 80    | 100 |
| S    | 16    | 6   |
| S    | 47    | 26  |
| S    | 49    | 11  |
| S    | 56    | 53  |
| S    | 62    | 19  |
| S    | 64    | 60  |
| S    | 87    | 24  |
| S    | 93    | 73  |
| S    | 95    | 75  |
| S    | 98    | 78  |


---

## Output

The output after running the executable as explained above will display:

- average rolls to win: *number*
- shortest game (number of rolls): *rolled numbers*
- jump traversal counts: *The last piece of information explains how many times each snake/ladder was used and its percentage.* 
