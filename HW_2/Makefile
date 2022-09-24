CC=g++
CFLAGS=-c
LDFLAGS=
OPTIMIZATIONS=-g
SOURCES=main.cpp matmul.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=time_results

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
func.o: matmul.cpp
	$(CC) $(OPTIMIZATIONS) $(CFLAGS) $< -o $@
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf *.o time_results