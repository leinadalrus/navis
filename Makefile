CC := gcc
CXX := g++
CFLAGS := -Wall -g

BUILD_DIR := ./build
LIBRARY_DIR := ./lib
SOURCE_DIR := ./src

SOURCES := $(shell find $(SOURCE_DIR) -name '*.cpp' -or -name '*.cxx' -or -name '*.cc' -or -name '*.c' -or -name '*.s')
OBJECTS := $(SOURCES:%=$(BUILD_DIR)/%.o)

DEPENDENCIES := $(OBJECTS:=.o=.d)

INCLUDE_DIR := $(shell find $(SOURCE_DIR) -type d)
INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIR))

CPPFLAGS := $(INCLUDE_FLAGS) -MMD -MP
CXXFLAGS := -Wall -g
LDFLAGS := 

TARGET_EXEC := "pale-noel"

$(BUILD_DIR)/%$(TARGET_EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< $@

%.o: %.cpp %.cxx %.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: $(TARGET_EXEC)
default: all

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPENDENCIES)
