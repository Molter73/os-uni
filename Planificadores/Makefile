# Compilador y opciones de compilación
CC = gcc
CFLAGS = -I./modules

# Nombres de archivos y carpetas
TARGET = planificador
SRC_DIR = .
MODULES_DIR = ./modules
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(MODULES_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(wildcard $(MODULES_DIR)/*.h)

# Reglas
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(SRC_DIR)/*.o $(MODULES_DIR)/*.o $(TARGET)

.PHONY: clean
