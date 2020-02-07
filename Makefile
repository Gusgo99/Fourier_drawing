# Flags do make
MAKEFLAGS	:=	-j12

NOMEPROJ	:=	fourier_drawing

# Arquivos usados
HEADERS		:= 	complex.hpp fourier.hpp \
				display.hpp image.hpp screen_view.hpp \
				request.hpp screen_change_request.hpp resize_request.hpp \
				edit_mode.hpp draw_mode.hpp DIP_mode.hpp \
				image_processing_tool.hpp modifying_image_processing_tool.hpp
SOURCES		:= main.cpp $(patsubst %.hpp, %.cpp, $(HEADERS))

# Compilador e padrao da linguagem
CXX			:=	c++
STD			:=	c++17

# Diretorios
INCDIR		:=	include
SRCDIR		:=	src
BINDIR		:=	bin
OBJDIR		:=	objects
DEPDIR		:=	dependencies

# Flags
CFLAGS		:=	-Wall -Wextra -Wshadow -Wcast-align -Wunused -pedantic  -O0 -DDEBUG -g
CXXFLAGS	:=	-Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual
LINKFLAGS	:=	-LC:/libraries/lib -g $(SDLFLAGS)
SDLFLAGS	:=	-lmingw32 -lSDL2main -lSDL2
INCHEADERS	:=	-iquote$(INCDIR)
LIBRARIES	:=	-IC:/libraries/include

##################################################################################################################################################
# Debug

# Atualiza executavel de debug a partir dos object files
$(BINDIR)/debug/$(NOMEPROJ)_debug.exe: $(patsubst %.cpp, $(OBJDIR)/debug/%.o, $(SOURCES))
	@ echo Linking
	@ $(CXX) $^ -o $@ $(LINKFLAGS) $(SDLFLAGS)

# recebe dependencias
-include $(patsubst %.cpp, $(DEPDIR)/%.d, $(SOURCES))

# Atualiza object files de debug
$(OBJDIR)/debug/main.o:	$(SRCDIR)/main.cpp
	@ echo Compiling $<
	@ $(CXX) -std=$(STD) $< -c -o $@ $(INCHEADERS) $(CFLAGS) $(CXXFLAGS) $(LIBRARIES)
	
$(OBJDIR)/debug/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.hpp
	@ echo Compiling $<
	@ $(CXX) -std=$(STD) $< -c -o $@ $(INCHEADERS) $(CFLAGS) $(CXXFLAGS) $(LIBRARIES)

##################################################################################################################################################
# Prod

# Atualiza executavel de optimized a partir dos object files
$(BINDIR)/optimized/$(NOMEPROJ).exe: $(patsubst %.cpp, $(OBJDIR)/optimized/%.o, $(SOURCES))
	@ echo Linking
	@ $(CXX) $^ -o $@ -LC:/libraries/lib $(SDLFLAGS) -mwindows

# Atualiza object files de optimized
$(OBJDIR)/optimized/main.o:	$(SRCDIR)/main.cpp
	@ echo Compiling $<
	@ $(CXX) -std=$(STD) $< -c -o $@ $(INCHEADERS) $(LIBRARIES) -O3
	
$(OBJDIR)/optimized/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.hpp
	@ echo Compiling $<
	@ $(CXX) -std=$(STD) $< -c -o $@ $(INCHEADERS) $(LIBRARIES) -O3

.PHONY: clean cleaner optim setup exec exec_optimized

# too ugly, don't look too closely
$(DEPDIR)/%.d: $(SRCDIR)/%.cpp $(patsubst %.hpp, $(INCDIR)/%.hpp, $(HEADERS))
	@ echo | set /p="$(OBJDIR)/debug/" > $@ | set ERRORLEVEL = 0
	@ $(CXX) $< -DDEBUG -MM $(INCHEADERS) >> $@
	@ echo | set /p="$(OBJDIR)/optimized/" >> $@ | set ERRORLEVEL = 0
	@ $(CXX) $< -MM $(INCHEADERS) >> $@

# Limpa objetc files (somente windows)
clean:
	@ del .\objects\debug\*.o
	@ del .\objects\optimized\*.o
	@ del .\$(DEPDIR)\*.d

# Limpa tudo (objetos, binarios) (somente windows)
cleaner: clean
	@ del .\bin\debug\*.exe
	@ del .\bin\optimized\*.exe
	@ del .\*.log

# Compila arquivos sem otimizacao.
debug: $(BINDIR)/debug/$(NOMEPROJ)_debug.exe

# Compila arquivos com otimizacao para runtime.
optim: $(BINDIR)/optimized/$(NOMEPROJ).exe

# Executa arquivos gerados para debug e gera log
exec:
	@ cls
	@ $(BINDIR)/debug/$(NOMEPROJ)_debug.exe

exec_optim:
	@ cls
	@ $(BINDIR)/optimized/$(NOMEPROJ).exe

setup:
	@ - mkdir $(OBJDIR)\debug
	@ - mkdir $(OBJDIR)\optimized
	@ - mkdir $(BINDIR)\debug
	@ - mkdir $(BINDIR)\optimized
	@ - mkdir $(SRCDIR)
	@ - mkdir $(INCDIR)
	@ - mkdir $(DEPDIR)
	@ - mkdir resources\
