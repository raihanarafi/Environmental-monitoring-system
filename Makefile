CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lcurl -ljson-c

SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
EXECUTABLE = $(BIN_DIR)/weather_monitor

all: directories $(EXECUTABLE)

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./$(EXECUTABLE)

.PHONY: all clean run

