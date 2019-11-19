
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
LDFLAGS  := -L/usr/lib -L/usr/lib/ -lm -lSDL2
LDLIBS   := 
INCLUDES := -Isrc -I/usr/include -I/usr/local/include
NAME_EXT := 
#`sdl2-config --libs`
#`sdl2-config --cflags`
#`sdl2-config --static-libs`
#LIBRARY_PATH, LD_LIBRARY_PATH

#-I/opt/local/x86_64-w64-mingw32/include/SDL2 -Dmain=SDL_main
#-L/opt/local/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows
#-L/opt/local/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -static-libgcc

PLAT     := w64-mingw32
ARCH     := x86_64

CROSS    := $(PLAT)-$(ARCH)-
#CXX     = $(CROSS)g++
#CROSSCXX = $(CROSS)g++

NAME_EXT := 
CROSSCXX  := $(CXX)

#CROSSCXX  := $(ARCH)-$(PLAT)-$(CXX)

#NAME_EXT := .exe
#CROSSCXX := x86_64-w64-mingw32-g++
#CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -Dmain=SDL_main
# the mistery of linker libs order mattering...
#-L/usr/lib/gcc/x86_64-w64-mingw32/ -lmingw32 -lSDL2main
#LDFLAGS  := -L/usr/x86_64-w64-mingw32/lib/ -L/home/gal/x86_64-w64-mingw32/lib/ -lmingw32 -lSDL2main -lSDL2 -mwindows -lm -lstdc++
#INCLUDES := -Isrc -I/usr/x86_64-w64-mingw32/include/ -I/home/gal/x86_64-w64-mingw32/include/

#TODO copy these dlls
#SDL2.dll
#libgcc_s_seh-1.dll
#libstdc++-6.dll
#/usr/lib/gcc/x86_64-w64-mingw32/8.3-win32/
#from sdl

SRC_PATH   := src
BUILD_PATH := build
TARGET     := Asemu
NAME       := Asemu
NAME       := $(NAME)$(NAME_EXT)

SRC_DIRS := $(sort $(dir $(wildcard $(SRC_PATH)/*/)))
CPPFILES := $(foreach dirname, $(SRC_DIRS), $(wildcard $(dirname)*.cpp))
HPPFILES := $(foreach dirname, $(SRC_DIRS), $(wildcard $(dirname)*.hpp))
OBJFILES := $(CPPFILES:%.cpp=$(BUILD_PATH)/%.o)

.PHONY: all clean debug release
.PHONY: simple test

# incremental build
all: $(BUILD_PATH)/$(NAME)

$(BUILD_PATH)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CROSSCXX) -MMD -MF $(basename $@).d $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
-include $(basename $@).d

$(BUILD_PATH)/$(NAME): $(OBJFILES)
	@mkdir -p $(@D)
	$(CROSSCXX) $(CXXFLAGS) $(INCLUDES) $(OBJFILES) -o $(BUILD_PATH)/$(NAME) $(LDFLAGS)

# single step build
simple:
	$(CROSSCXX) $(CXXFLAGS) $(INCLUDES) $(CPPFILES) -o $(BUILD_PATH)/$(NAME) $(LDFLAGS)

# builds with options
debug: CXXFLAGS += -g -Og -DDEBUG
debug: clean all

release: CXXFLAGS += -O2
release: clean all

# utilities
clean:
	-@rm -rvf $(BUILD_PATH)/*

.PHONY: distclean
distclean:
	rm -r build/*
	rm -r .build/*
	rm log.txt

test:
	@echo $(CROSSCXX)
	@echo $(SRC_DIRS)
	@echo $(CPPFILES)
