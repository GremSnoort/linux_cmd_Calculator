CC = g++
CFLAGS = -std=c++11 -Wall
OFLAGS = -std=c++11

#incs
INCLUDES = $(PWD)

#srcs
CXX_SRCS += calcmain.cpp\
			Processor.cpp\
			Level.cpp

#objs
CXX_OBJFILES := $(CXX_SRCS:.cpp=.o)

#app
CALC_APP = calc.run

all: $(CALC_APP)
	rm -rf *.o

$(CALC_APP): $(CXX_OBJFILES)
	$(CC) $(OFLAGS) $^ -o $(CALC_APP)

$(CXX_OBJFILES): %.o: %.cpp
	$(info Compiling $*.cpp ...)
	$(CC) $(CFLAGS) -I$(INCLUDES) -o $@ -c $<
	
clean:
	rm -rf *.o $(CALC_APP)
	