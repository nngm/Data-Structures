SANFLAGS = -fsanitize=address
CXXFLAGS += -Wall -std=c++11 -pedantic $(SANFLAGS)
LDFLAGS += $(SANFLAGS)

C++ = g++ $(CXXFLAGS)

main: main.cpp stack.h queue.h to_postfix.h tree.h
	g++ -o assign2 main.cpp stack.h queue.h to_postfix.h tree.h

clean:
	rm -f assign2