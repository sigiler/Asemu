
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
LDFLAGS  := -lm -L/usr/lib -lSDL2
INCLUDES := -Isrc -I/usr/include -I/usr/local/include

SRC_PATH   := src src/core src/frontend src/libs src/tests src/utils
BUILD_PATH := build

NAME := Asemu

CPPFILES := $(foreach dirname, $(SRC_PATH), $(wildcard $(dirname)/*.cpp))
HPPFILES := $(foreach dirname, $(SRC_PATH), $(wildcard $(dirname)/*.hpp))
OBJFILES := $(CPPFILES:%.cpp=$(BUILD_PATH)/%.o)

.PHONY: all clean debug release

# incremental build
all: $(BUILD_PATH)/$(NAME)

$(BUILD_PATH)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -MMD -MF $@.d $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
-include $@.d

$(BUILD_PATH)/$(NAME): $(OBJFILES)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) $(OBJFILES) -o $(BUILD_PATH)/$(NAME)

# simple step build
#all: $(BUILD_PATH)/$(NAME)
#$(BUILD_PATH)/$(NAME): $(CPPFILES)
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) $(CPPFILES) -o $(BUILD_PATH)/$@

debug: CXXFLAGS += -g -Og -DDEBUG
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rv build/*
