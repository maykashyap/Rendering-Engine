CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -DGLFW_INCLUDE_NONE
CFLAGS   :=
LDFLAGS  :=
LIBS     := -lglfw -ldl -lGL

# ── Directories ───────────────────────────────────────────────────────────
ENGINE_SRC  := engine/src
ENGINE_INC  := engine/include
VENDOR_DIR  := engine/vendor
PLAYGROUND  := playground
OBJ_DIR     := engine/build/obj
BIN_DIR     := bin

# ── Target naming ─────────────────────────────────────────────────────────
# Base name of the app — override on the command line if needed.
#   make APP=my_game
APP := playground_app

# Optional version suffix — appended to every binary name when provided.
#   make VERSION=1.0.0
#   make debug VERSION=1.0.0
VERSION        :=
VERSION_SUFFIX := $(if $(VERSION),_$(VERSION),)

# Build mode suffix + flags
ifeq ($(BUILD), debug)
    MODE_SUFFIX  := _debug
    CXXFLAGS     += -g3 -O0 -DDEBUG
    CFLAGS       += -g3 -O0 -DDEBUG

else ifeq ($(BUILD), asan)
    MODE_SUFFIX  := _asan
    # -fno-omit-frame-pointer keeps frame pointers intact so asan can
    # produce full call stacks in its error output.
    CXXFLAGS     += -g3 -O0 -DDEBUG -fsanitize=address,undefined \
                    -fno-omit-frame-pointer
    CFLAGS       += -g3 -O0        -fsanitize=address,undefined \
                    -fno-omit-frame-pointer
    LDFLAGS      += -fsanitize=address,undefined

else
    MODE_SUFFIX  := _release
    CXXFLAGS     += -O2 -DNDEBUG
    CFLAGS       += -O2 -DNDEBUG
endif

# Final binary path — bin/<app>_<mode>[_<version>]
# Examples:
#   bin/playground_app_release
#   bin/playground_app_debug_1.0.0
#   bin/playground_app_asan
TARGET := $(BIN_DIR)/$(APP)$(MODE_SUFFIX)$(VERSION_SUFFIX)

# ── Include paths ─────────────────────────────────────────────────────────
INCLUDES := \
    -I$(ENGINE_INC)                  \
    -I$(VENDOR_DIR)                  \
    -I$(ENGINE_SRC)/backends/glfw    \
    -I$(ENGINE_SRC)/backends/opengl

# ── Source discovery ──────────────────────────────────────────────────────
ENGINE_CXX_SRCS     := $(shell find $(ENGINE_SRC) -name '*.cpp')
PLAYGROUND_CXX_SRCS := $(shell find $(PLAYGROUND) -name '*.cpp')
VENDOR_C_SRCS       := $(VENDOR_DIR)/glad/glad.c

# ── Object paths — mirror source tree under OBJ_DIR ──────────────────────
ENGINE_OBJS     := $(patsubst $(ENGINE_SRC)/%.cpp,  \
                       $(OBJ_DIR)/engine/src/%.o,   \
                       $(ENGINE_CXX_SRCS))

PLAYGROUND_OBJS := $(patsubst $(PLAYGROUND)/%.cpp,  \
                       $(OBJ_DIR)/playground/%.o,   \
                       $(PLAYGROUND_CXX_SRCS))

VENDOR_OBJS     := $(patsubst $(VENDOR_DIR)/%.c,       \
                       $(OBJ_DIR)/engine/vendor/%.o,   \
                       $(VENDOR_C_SRCS))

ALL_OBJS := $(ENGINE_OBJS) $(PLAYGROUND_OBJS) $(VENDOR_OBJS)

# ── Dependency tracking ───────────────────────────────────────────────────
# -MMD -MP emits a .d file per .o listing its header dependencies.
# Changing any included header triggers recompilation of only affected files.
DEPFLAGS := -MMD -MP
-include $(ALL_OBJS:.o=.d)

# ── Targets ───────────────────────────────────────────────────────────────
.PHONY: all debug asan run run-debug run-asan clean clean-bin list

all: $(TARGET)

debug:
	$(MAKE) BUILD=debug VERSION=$(VERSION)

asan:
	$(MAKE) BUILD=asan VERSION=$(VERSION)

# run targets — each builds its mode then executes the correct binary
run: all
	./$(TARGET)

run-debug: debug
	./$(BIN_DIR)/$(APP)_debug$(VERSION_SUFFIX)

run-asan: asan
	./$(BIN_DIR)/$(APP)_asan$(VERSION_SUFFIX)

# List all binaries currently in bin/
list:
	@echo "Binaries in $(BIN_DIR)/:"
	@ls -lh $(BIN_DIR)/ 2>/dev/null || echo "  (empty)"

# ── Link ──────────────────────────────────────────────────────────────────
$(TARGET): $(ALL_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "Built: $@"

# ── Compile rules ─────────────────────────────────────────────────────────

# Engine C++ sources
$(OBJ_DIR)/engine/src/%.o: $(ENGINE_SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Playground C++ sources
$(OBJ_DIR)/playground/%.o: $(PLAYGROUND)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Vendor C sources (glad)
$(OBJ_DIR)/engine/vendor/%.o: $(VENDOR_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# ── Clean ─────────────────────────────────────────────────────────────────
# clean     — wipe all object files (forces full recompile, keeps binaries)
# clean-bin — wipe all built binaries in bin/
# Both together for a full reset: make clean clean-bin
clean:
	rm -rf $(OBJ_DIR)

clean-bin:
	rm -rf $(BIN_DIR)
