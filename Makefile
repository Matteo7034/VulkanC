# Compilatore e flag
CC      := gcc
CFLAGS  := -std=c11 -O2 -Wall -Wextra -Iinclude
LDFLAGS := -lglfw -lvulkan -ldl -lm -lpthread -lX11 -lXrandr -lXi

# Tool per gli shader
GLSLC   := glslc

# Cartelle e target
SRC_DIR   := src
BUILD_DIR := build
TARGET    := VulkanTest

# Sorgenti (.c) e Oggetti (.o)
SRC := $(SRC_DIR)/main.c $(SRC_DIR)/app.c $(SRC_DIR)/init.c
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

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

# Compilazione dei singoli file .c in .o dentro build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia dei file generati
clean:
	rm -rvf $(BUILD_DIR) $(TARGET) shaders/*.spv

# Esecuzione del programma
run: $(TARGET) shaders
	./$(TARGET)

.PHONY: all clean run shaders
