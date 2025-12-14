CC = gcc
CFLAGS = -Wall -Wextra -g -pedantic

TARGET = myshell
PROJECT_NAME = $(TARGET)

SRCDIR = src
HDRDIR = include
BUILD_DIR = .build


SRCS = main.c parse.c execute.c builtin.c signals.c logger.c
OBJS = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))

$(TARGET): $(BUILD_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(HDRDIR) -c $< -o $@

.PHONY: clean
clean:
	@echo "Removing executable and build directory..."
	rm -f $(PROJECT_NAME)
	rm -rf $(BUILD_DIR)