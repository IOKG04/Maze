# Maze

> A raycasting engine I wanna a) finish and b) turn into a small procedually generated maze "game"

## How to build

If you are building the project for the first time,
```bash
make
```
should work just fine.

If you wish to build the project again, you will have to either run
```bash
make clean
make
```
or run
```bash
make call
```
which is short for `clean all`, which in turn is short for `clean and make all`.  
It runs `make clean` and `make` at once is what im saying. Idk why I called it `call` and not something that makes more sense. Idk how makefiles work btw, any help to make it better would be appreciated :3

## Features

- [x] Raycasting engine
  - [x] *Working* raycasting engine
    - [x] **Really** *working* raycasting engine
- [x] Level generation
  - [ ] *Better* level generation
- [ ] GUI functions
  - [ ] Main menu
- [ ] Gameplay
