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
all: $(TARGET)

debug: CFLAGS += -g
debug: $(TARGET)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(SRC_DIR)/*.o $(MODULES_DIR)/*.o $(TARGET)
	rm -rf $(SRC_DIR)/outputs/
