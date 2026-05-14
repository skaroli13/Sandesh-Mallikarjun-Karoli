CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = flight-delay-analysis
SRCS = main.cpp HashTable.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
