AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2

all : libDPT_balance_color.a
	@echo -n ""

clean :
	rm *.a *.o

libDPT_balance_color.a : tm_usage.o
	$(AR) $(ARFLAGS) $@ $^ 

tm_usage.o : tm_usage.cpp tm_usage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

