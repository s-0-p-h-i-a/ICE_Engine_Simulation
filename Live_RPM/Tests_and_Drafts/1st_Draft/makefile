# -----------------------------
# Toolchain
# -----------------------------
MCU      := atmega328p        # Arduino Uno MCU
F_CPU    := 16000000UL
CC       := avr-gcc
CXX      := avr-g++
OBJCOPY  := avr-objcopy

# -----------------------------
# Compiler flags
# -----------------------------
CFLAGS   := -Wall -Wextra -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=c11
CXXFLAGS := -Wall -Wextra -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=c++17

# Linker flags
LDFLAGS  := -mmcu=$(MCU)

# -----------------------------
# Sources
# -----------------------------
INO_FILE := cylinder_and_flywheel.ino            # Replace with your .ino file name
CPP_SRCS := $(wildcard *.cpp) $(INO_FILE:.ino=.cpp)  # Include other .cpp files + converted .ino
C_SRCS   := $(wildcard *.c)
OBJS     := $(C_SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)

# -----------------------------
# Target
# -----------------------------
TARGET := main.elf
HEX    := main.hex

# -----------------------------
# Rules
# -----------------------------

# Convert .ino to .cpp (Arduino CLI does this automatically)
$(INO_FILE:.ino=.cpp): $(INO_FILE)
	cp $< $@  # Simple copy; in bigger sketches you could add preprocessing if needed

all: $(HEX)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Convert ELF to HEX for uploading
%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET) $(HEX) $(INO_FILE:.ino=.cpp)

