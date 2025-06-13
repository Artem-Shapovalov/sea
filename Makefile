SOURCES += main.cpp
SOURCES += helpers.cpp
SOURCES += app.cpp
SOURCES += builder.cpp

OBJECTS += main.o
OBJECTS += helpers.o
OBJECTS += app.o
OBJECTS += builder.o

CXXFLAGS = -I. -Wall -pedantic -std=c++17 -g3 -ggdb -DVERSION="\"sub-build\""
CXX = g++

all: sea

install: sea
	cp sea /usr/bin

uninstall:
	rm /usr/bin/sea

sea: _sea
	rm -rf *.o sea
	./_sea --build

_sea: $(OBJECTS)
	$(CXX) $(OBJECTS) -o _sea

main.o: main.cpp app.hpp builder.hpp helpers.hpp
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS)

main.cpp:

helpers.o: helpers.hpp helpers.cpp
	$(CXX) -c helpers.cpp -o helpers.o $(CXXFLAGS)

helpers.cpp:

helpers.hpp:

app.o: app.hpp app.cpp helpers.hpp
	$(CXX) -c app.cpp -o app.o $(CXXFLAGS)

app.cpp:

app.hpp:

builder.o: builder.hpp builder.cpp helpers.hpp
	$(CXX) -c builder.cpp -o builder.o $(CXXFLAGS)

builder.cpp:

builder.hpp:

clean:
	rm -rf .sea _sea _sea.a _sea.so sea $(OBJECTS)
