CC = g++
CFLAGS = -g -c -I../include 
LDFLAGS = -D__linux__
SOURCES = ../src/main.cpp ../src/websocket.cpp ../src/sha1.cpp ../src/base64.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = server

all: $(SOURCES) $(EXECUTABLE) mvobj


$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o ./$@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

mvobj:
	-mv -t . ../src/*.o server

clean:
	rm -rf *o $(EXECUTABLE)
