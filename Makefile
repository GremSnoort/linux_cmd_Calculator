CC = g++
CFLAGS = -std=c++11 -Wall
OFLAGS = -std=c++11

#incs
INCLUDES = $(PWD)

#srcs
CXX_SRCS += Level.cpp\
		Processor.cpp\
		calcmain.cpp

#objs
CXX_OBJFILES := $(CXX_SRCS:.cpp=.o)

#app
CALC_APP   = calc.run
SIMPLE_APP = simple.run

all: $(CALC_APP)
	rm -rf *.o

simple: $(SIMPLE_APP)
	rm -rf *.o

$(CALC_APP): $(CXX_OBJFILES)
	$(CC) $(OFLAGS) $^ -o $(CALC_APP)

$(SIMPLE_APP):
	$(CC) $(CFLAGS) -c simple_calc.cpp
	$(CC) $(OFLAGS) simple_calc.o -o $(SIMPLE_APP)

$(CXX_OBJFILES): %.o: %.cpp
	$(info Compiling $*.cpp ...)
	$(CC) $(CFLAGS) -I$(INCLUDES) -o $@ -c $<
	
clean:
	rm -rf *.o $(CALC_APP) $(SIMPLE_APP)
	
