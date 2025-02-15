# Compiler
CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra -O2 -luser32 -mwindows

# Trouver tous les fichiers .c dans le dossier courant
SRC = $(wildcard *.c)

# Générer les noms de sortie en remplaçant .c par .exe
EXE = $(SRC:.c=.exe)

# Règle par défaut : compiler tous les .c en .exe
all: $(EXE)

# Règle générique pour compiler chaque .c en .exe
%.exe: %.c
	$(CC) $(CFLAGS) $< -o $@

# Nettoyer les fichiers générés
clean:
	rm -f *.exe
