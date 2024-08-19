CC = gcc

BIN_DIR = bin
OBJ_DIR = obj

# Maze sources and such
MAZE_SRC_DIR = src
MAZE_BIN_DIR = $(BIN_DIR)/Maze
MAZE_OBJ_DIR = $(OBJ_DIR)/Maze
MAZE_TARGET = $(MAZE_BIN_DIR)/build
MAZE_SOURCES = $(MAZE_SRC_DIR)/main.c $(MAZE_SRC_DIR)/chunk.c $(MAZE_SRC_DIR)/raycast.c $(MAZE_SRC_DIR)/display.c
MAZE_OBJECTS = $(patsubst $(MAZE_SRC_DIR)/%.c,$(MAZE_OBJ_DIR)/%.o,$(MAZE_SOURCES))
# other Maze stuff
MAZE_CFLAGS = -Wall -lm

.PHONY: all
all: $(MAZE_TARGET)

# Maze related targets
.PHONY: maze
maze: $(MAZE_TARGET)

$(MAZE_TARGET): $(MAZE_OBJECTS)
	@mkdir -p $(MAZE_BIN_DIR)
	gcc -o $@ $^ $(MAZE_CFLAGS)

$(MAZE_OBJ_DIR)/%.o: $(MAZE_SRC_DIR)/%.c
	@mkdir -p $(MAZE_OBJ_DIR)
	gcc -o $@ $< -c $(MAZE_CFLAGS)

# cleaning related targets
.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
