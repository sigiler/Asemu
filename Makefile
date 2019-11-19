
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
LDFLAGS  := -lm -L/usr/lib -lSDL2
INCLUDES := -Isrc -I/usr/include -I/usr/local/include

SRC_PATH   := src
BUILD_PATH := build
NAME       := Asemu

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
	$(CXX) -MMD -MF $(basename $@).d $(CXXFLAGS) $(INCLUDES) -o $@ -c $<
-include $(basename $@).d

$(BUILD_PATH)/$(NAME): $(OBJFILES)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) $(OBJFILES) -o $(BUILD_PATH)/$(NAME)

# single step build
simple:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) $(CPPFILES) -o $(BUILD_PATH)/$(NAME)

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
	@echo $(SRC_DIRS)
	@echo $(CPPFILES)
