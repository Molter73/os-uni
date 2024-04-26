#Makefile
# Define el compilador que se usará para compilar los programas.
CC = gcc

# Flags de compilación:
# -I./simulation añade el directorio simulation a la lista de directorios a buscar para los archivos de inclusión.
# -Wall habilita todos los mensajes de advertencia del compilador.
CFLAGS = -I./simulation -Wall

# Busca todos los archivos fuente en el directorio simulation y los almacena en la variable SRCS.
SRCS = $(wildcard simulation/*.c)

# Genera los nombres de los archivos objeto de los archivos fuente encontrados, cambiando la extensión .c por .o.
OBJS = $(SRCS:.c=.o)

# La regla 'all' que es el objetivo por defecto que make intentará construir cuando no se especifique un objetivo.
# Depende del objetivo 'main', lo que significa que 'main' debe ser construido cuando se ejecute 'make' o 'make all'.
all: main

# Regla para construir el ejecutable 'main'.
# Depende de main.o y todos los archivos objeto definidos en OBJS.
# $(CC) -o $@ $^ compila y enlaza los archivos objeto para formar el ejecutable final llamado 'main'.
main: main.o $(OBJS)
	$(CC) -o $@ $^

# Regla para compilar main.c a main.o.
# $(CC) $(CFLAGS) -c main.c compila el archivo main.c con los flags especificados y produce el archivo objeto main.o.
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Regla genérica para convertir cualquier archivo .c a un archivo .o.
# $< es el nombre del archivo fuente, y $@ es el nombre del archivo objetivo.
# Esto permite compilar cualquier archivo .c en el directorio simulation a su correspondiente archivo .o.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla 'clean' para limpiar el proyecto borrando todos los archivos compilados y el ejecutable.
# rm -f elimina los archivos listados de forma segura (sin preguntar).
clean:
	rm -f simulation/*.o main.o main

# Indica que 'all' y 'clean' no son archivos, sino nombres de reglas.
.PHONY: all clean
