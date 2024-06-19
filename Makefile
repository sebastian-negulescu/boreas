CC = clang

INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj

VERSION = -std=c2x
OPTIMIZATION = -O3
debug: OPTIMIZATION = -O0

CFLAGS = $(VERSION) $(OPTIMIZATION) -Wall -Wextra -Wpedantic -I$(INC_DIR)
LDFLAGS = -lm

# Source files and include dirs
SOURCES := $(wildcard $(SRC_DIR)/*.c)
# Create .o and .d files for every .c
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SOURCES))

debug: CFLAGS += -g
debug: boreas

clean:
	rm -f boreas
	rm -rf $(OBJ_DIR)

boreas: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPENDS)

