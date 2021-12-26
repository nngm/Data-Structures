SANFLAGS = -fsanitize=address
CXXFLAGS += -Wall -std=c++11 -pedantic $(SANFLAGS)
LDFLAGS += $(SANFLAGS)

C++ = g++ $(CXXFLAGS)
header = FlatHash.h HierarchyHash.h NRKFlat.h

main: main.cpp $(header)
	$(C++) -o assign3 main.cpp $(header)

clean:
	rm -f assign3