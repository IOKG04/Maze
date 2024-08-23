CC = gcc

BIN_DIR = bin
OBJ_DIR = obj

# Maze sources and such
MAZE_SRC_DIR = src
MAZE_BIN_DIR = $(BIN_DIR)/Maze
MAZE_OBJ_DIR = $(OBJ_DIR)/Maze
MAZE_TARGET = $(MAZE_BIN_DIR)/build
MAZE_SOURCES = $(wildcard $(MAZE_SRC_DIR)/*.c)
MAZE_HEADERS = $(wildcard $(MAZE_SRC_DIR)/*.h)
MAZE_OBJECTS = $(patsubst $(MAZE_SRC_DIR)/%.c,$(MAZE_OBJ_DIR)/%.o,$(MAZE_SOURCES))
# other Maze stuff
MAZE_CFLAGS = -Wall -lm

# MaDiEn sources and such
MDE_SRC_DIR = madien_vALPHA.0.0
MDE_BIN_DIR = $(BIN_DIR)/MaDiEn
MDE_OBJ_DIR = $(OBJ_DIR)/MaDiEn
MDE_TARGET = $(MDE_BIN_DIR)/madien.a
MDE_SOURCES = $(wildcard $(MDE_SRC_DIR)/*.c)
MDE_HEADERS = $(wildcard $(MDE_SRC_DIR)/*.h)
MDE_OBJECTS = $(patsubst $(MDE_SRC_DIR)/%.c,$(MDE_OBJ_DIR)/%.o,$(MDE_SOURCES))
MDE_HEADERS_MAZE_DIR = $(MAZE_SRC_DIR)/madien
MDE_HEADERS_MAZE = $(patsubst $(MDE_SRC_DIR)/%.h,$(MDE_HEADERS_MAZE_DIR)/%.h,$(MDE_HEADERS))
# other MaDiEn stuff
MDE_CFLAGS = -Wall -O2

.PHONY: all
all: $(MDE_TARGET) $(MAZE_TARGET)

# Maze related targets
.PHONY: maze
maze: $(MAZE_TARGET)

$(MAZE_TARGET): $(MDE_HEADERS_MAZE) $(MAZE_OBJECTS) $(MDE_TARGET)
	@mkdir -p $(MAZE_BIN_DIR)
	gcc -o $@ $(filter-out %.h,$^) $(MAZE_CFLAGS)

$(MAZE_OBJ_DIR)/%.o: $(MAZE_SRC_DIR)/%.c $(MAZE_HEADERS)
	@mkdir -p $(MAZE_OBJ_DIR)
	gcc -o $@ $< -c $(MAZE_CFLAGS)

# MaDiEn related targets
.PHONY: madien
madien: $(MDE_TARGET)

$(MDE_TARGET): $(MDE_OBJECTS)
	@mkdir -p $(MDE_BIN_DIR)
	ar rcs $@ $^

$(MDE_OBJ_DIR)/%.o: $(MDE_SRC_DIR)/%.c $(MDE_HEADERS)
	@mkdir -p $(MDE_OBJ_DIR)
	gcc -o $@ $< -c $(MDE_CFLAGS)

.PHONY: madien_headers
madien_headers: $(MDE_HEADERS_MAZE)

$(MDE_HEADERS_MAZE_DIR)/%.h: $(MDE_SRC_DIR)/%.h
	@mkdir -p $(MDE_HEADERS_MAZE_DIR)
	cp $< $@

# cleaning related targets
.PHONY: clean
clean: clean_mde_headers
	rm -rf $(BIN_DIR) $(OBJ_DIR)

.PHONY: clean_mde_headers
clean_mde_headers:
	rm -rf $(MDE_HEADERS_MAZE_DIR)
