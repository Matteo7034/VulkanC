# Compilatore e flag
CC      := gcc
CFLAGS  := -std=c11 -O2 -Wall -Wextra -Iinclude
LDFLAGS := -lglfw -lvulkan -ldl -lm -lpthread -lX11 -lXrandr -lXi

# Tool per gli shader
GLSLC   := glslc

# Cartelle e target
SRC_DIR   := src
SRC_UTILS := utils
BUILD_DIR := build
TARGET    := VulkanTest

# Sorgenti (.c) divise per cartella
SRC_ENGINE := $(SRC_DIR)/main.c $(SRC_DIR)/engine.c $(SRC_DIR)/init.c
SRC_UTILS_FILES := $(SRC_UTILS)/debug.c

# Oggetti (.o) mappati correttamente in build/
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_ENGINE)) \
       $(patsubst $(SRC_UTILS)/%.c, $(BUILD_DIR)/%.o, $(SRC_UTILS_FILES))

# Shader
VERT_SOURCES := $(wildcard shaders/*.vert)
FRAG_SOURCES := $(wildcard shaders/*.frag)
SPV_OBJS     := shaders/vert.spv shaders/frag.spv

# Regola principale
all: $(TARGET) shaders

# Regola per la compilazione degli shader GLSL -> SPIR-V
shaders: $(SPV_OBJS)

shaders/vert.spv: shaders/shader.vert
	@mkdir -p shaders
	$(GLSLC) $< -o $@

shaders/frag.spv: shaders/shader.frag
	@mkdir -p shaders
	$(GLSLC) $< -o $@

# Link dell'eseguibile finale
$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Regola per i file .c dentro src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nuova regola per i file .c dentro utils/
$(BUILD_DIR)/%.o: $(SRC_UTILS)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia dei file generati
clean:
	rm -rvf $(BUILD_DIR) $(TARGET) shaders/*.spv

# Esecuzione del programma
run: $(TARGET) shaders
	./$(TARGET)

.PHONY: all clean run shaders
