CXX = g++
CXXFLAGS = -Wall -Wextra -g
LDFLAGS = -lncurses

SRC = src/main.cpp src/logic/TerminalEditor.cpp src/render/RenderEditor.cpp src/render/RenderMenu.cpp
TARGET = bin/neonote

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(dir $(TARGET))
	$(CXX) -o $(TARGET) $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
