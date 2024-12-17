CXX = g++
CXXFLAGS = -std=c++17 -Wall 

# List of source files
SRCS = main.cpp 

# Output executable
TARGET = hardwareVault

# Build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean target
clean:
	rm -f $(TARGET) $(BENCHMARK_TARGET)