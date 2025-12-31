# *************************************************************************************
# FICHIER MAKEFILE (VERSION FINALE, CORRIGÉE POUR WINDOWS/MINGW)
# *************************************************************************************

# --- Variables Globales ---
TARGET_EXE = smart_parking_demo.exe

# Dossiers
SRCDIR = src
DEMO_SRCDIR = demos
INCDIR = include
OBJDIR = obj

# Fichiers sources
SRC_FILES = $(wildcard $(SRCDIR)/*.cpp)
DEMO_SRC_FILE = $(DEMO_SRCDIR)/main.cpp

# Fichiers objets
OBJ_FILES = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC_FILES))

# --- Dépendances Raylib ---

# VOTRE CHEMIN RAYLIB : C:\raylib\raylib-5.5_win64_mingw-w64
RAYLIB_PATH := C:/raylib/raylib-5.5_win64_mingw-w64

# Drapeaux de compilation (CFLAGS)
CFLAGS = -Wall -Wextra -std=c++17 -O2 
CFLAGS += -I$(INCDIR)
CFLAGS += -I$(RAYLIB_PATH)/include 

# Drapeaux de liaison (LDFLAGS)
# CORRECTION CRITIQUE : Ajout des librairies systemes Windows nécessaires a Raylib (winmm, gdi32, opengl32, ws2_32)
LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib -lm -ldl -lpthread -lwinmm -lgdi32 -lopengl32 -lws2_32

# Compilateur
CXX = g++

# --- Règles de Construction Générales ---

.PHONY: all demo clean

all: demo 

demo: $(TARGET_EXE)

# *************************************************************************************
# RÈGLES DE COMPILATION
# *************************************************************************************

$(TARGET_EXE): $(OBJ_FILES) $(DEMO_SRC_FILE)
	$(CXX) $(CFLAGS) $(DEMO_SRC_FILE) $(OBJ_FILES) -o $(TARGET_EXE) $(LDFLAGS)
	@echo "--- Executable Demo cree : $(TARGET_EXE) ---"

# Règle pour créer le dossier obj s'il n'existe pas
$(OBJDIR):	
	@if	not	exist	$(OBJDIR)	mkdir	$(OBJDIR)

# Règle pour compiler les fichiers source en objets
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR) 
	$(CXX) $(CFLAGS) -c $< -o $@

# *************************************************************************************
# RÈGLE DE NETTOYAGE
# *************************************************************************************

clean:
	@echo "Nettoyage des fichiers objets et executables..."
	@if exist $(OBJDIR) del /q $(OBJDIR)\*.o
	@if exist $(TARGET_EXE) del /q $(TARGET_EXE)
	@if exist $(OBJDIR) rd $(OBJDIR)
	@echo "Nettoyage termine."
test: src/tests.cpp src/Game.cpp
	g++ src/tests.cpp src/Game.cpp -Iinclude -lraylib -lopengl32 -lgdi32 -lwinmm -o unit_tests
	./unit_tests