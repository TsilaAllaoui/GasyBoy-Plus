.SUFFIX:

CXX = g++
CFLAGS = -std=c++11 -Wall

ifeq ($(OS), Windows_NT)
	OBJ = *.o
	LIB = "C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32\lib"
	HEADER = "C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32\include"
	SDL = -lmingw32 -lSDL2main -lSDL2 -lopengl32
	CLEAN_SCREEN = cls
	DEL_OBJ = del *.o
	DEL_BIN = del *.exe
else
	CXX = g++-10
	OBJ = obj/*
	LIB = .
	HEADER = /usr/include/SDL2/
	SDL = -lSDL2 -lSDL2_ttf
	MOVE = mkdir -p obj/ && mv *.o obj/
	DEL_OBJ = rm -f *.o
	DEL_BIN = rm -f gb
endif

IMGUI = imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_sdl.cpp imgui/imgui_sdl.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui.cpp

SRC = register.cpp cpu.cpp debugger.cpp gameboy.cpp gpu.cpp instruction_viewer.cpp machine.cpp main.cpp memory_viewer.cpp mmu.cpp register_viewer.cpp stack_viewer.cpp timer.cpp utilities.cpp $(IMGUI)

all:
	@$(CLEAN_SCREEN)

	@echo ... Compiling project, Please wait a moment ...
	@echo ***************************************
	$(CXX) $(CFLAGS) -I$(HEADER) -L$(LIB) -o gb $(SRC) $(SDL) 
	@echo ***************************************
	@$(CLEAN_SCREEN)

link : obj
	@echo ... Linking files ...
	@echo *********************
	$(CXX) $(OBJ) -L$(LIB) $(SDL) -o gb
	@echo *********************
	@$(CLEAN_SCREEN)


obj : $(SRC)
	@$(CLEAN_SCREEN)

	@echo ... Compiling, Please wait a moment ...
	@echo ***************************************
	$(CXX) $(CFLAGS) -I$(HEADER) -c $?
	@echo ***************************************

clean : 
	@$(DEL_OBJ)
	@$(DEL_BIN)
