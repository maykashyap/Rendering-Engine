CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -fPIC -Iinclude -Ivendor
CFLAGS   := -Ivendor
LDFLAGS  := -shared
LIBS     := -lglfw -ldl

SRC_DIR    := src
VENDOR_DIR := vendor
OBJ_DIR    := build/obj
LIB_DIR    := build/lib

TARGET := $(LIB_DIR)/libengine.so

# ── Sources ───────────────────────────────────────────────
# All engine .cpp files
CXX_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

# Vendor C files (glad — compiled as C, not C++)
C_SRCS   := $(VENDOR_DIR)/glad/glad.c

# Object files — mirror source tree under build/obj
CXX_OBJS := $(patsubst $(SRC_DIR)/%.cpp,    $(OBJ_DIR)/src/%.o,    $(CXX_SRCS))
C_OBJS   := $(patsubst $(VENDOR_DIR)/%.c,   $(OBJ_DIR)/vendor/%.o, $(C_SRCS))

ALL_OBJS := $(CXX_OBJS) $(C_OBJS)

# ── Release / Debug toggle ────────────────────────────────
ifeq ($(BUILD), debug)
    CXXFLAGS += -g3 -O0 -DDEBUG
    CFLAGS   += -g3 -O0 -DDEBUG
else
    CXXFLAGS += -O2 -DNDEBUG
    CFLAGS   += -O2 -DNDEBUG
endif

# ── Rules ─────────────────────────────────────────────────
.PHONY: all clean debug

all: $(TARGET)

debug:
	$(MAKE) BUILD=debug

# Link
$(TARGET): $(ALL_OBJS)
	@mkdir -p $(LIB_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# Compile engine .cpp files
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile vendor .c files (glad)
$(OBJ_DIR)/vendor/%.o: $(VENDOR_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/
