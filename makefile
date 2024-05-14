CC = gcc
CFLAGS = -lpthread
TARGET = principal
NAME = MM_ejecutable
ARGS = 5 2

run: $(TARGET)
	./$(NAME) $(ARGS)

$(TARGET): $(TARGET).c
	$(CC) $(TARGET).c -o $(NAME) $(CFLAGS)