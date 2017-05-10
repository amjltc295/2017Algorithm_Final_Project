AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2

all : libgraphlab.a
	@echo -n ""

clean :
	rm *.a *.o

tm_usage.o : tm_usage.cpp tm_usage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

