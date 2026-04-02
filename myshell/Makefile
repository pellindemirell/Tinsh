CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -Wformat=2 -Wshadow \
           -D_POSIX_C_SOURCE=200809L
DBGFLAGS = $(CFLAGS) -g -O0 -DDEBUG
SRCS    = main.c parser.c builtins.c executor.c
OBJS    = $(SRCS:.c=.o)
TARGET  = myshell
.PHONY: all
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build successful → ./$(TARGET)"
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
.PHONY: debug
debug: CFLAGS = $(DBGFLAGS)
debug: clean $(TARGET)
	@echo "Debug build complete — run with gdb ./$(TARGET)"
.PHONY: run
run: all
	./$(TARGET)
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned up object files and binary."
main.o:     main.c parser.h builtins.h executor.h
parser.o:   parser.c parser.h
builtins.o: builtins.c builtins.h
executor.o: executor.c executor.h
