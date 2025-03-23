CXX = g++
CXXFLAGS = -Wall -Wextra -g
LDFLAGS = -lncurses

SRC = src/main.cpp src/logic/EditorInputs.cpp src/render/RenderEditor.cpp
TARGET = bin/neonote

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) -o $(TARGET) $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
