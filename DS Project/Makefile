CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC_DIR  = DS\ Project
SOURCES  = $(SRC_DIR)/main.cpp \
           $(SRC_DIR)/Utils.cpp \
           $(SRC_DIR)/Train.cpp \
           $(SRC_DIR)/Coach.cpp \
           $(SRC_DIR)/Seat.cpp \
           $(SRC_DIR)/Network.cpp \
           $(SRC_DIR)/Logger.cpp \
           $(SRC_DIR)/UndoRedo.cpp \
           $(SRC_DIR)/FIleIO.cpp

TARGET   = IronNexus.exe

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q $(TARGET) 2>nul || rm -f $(TARGET)
	del /Q /S data\*.dat 2>nul || rm -f data/*.dat

.PHONY: all run clean
