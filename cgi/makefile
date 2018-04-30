CPP = g++ 

LIB = -lcgicc
SRCS = $(wildcard *.cpp) 
OBJS = $(SRCS:.cpp=.cgi)
all:$(OBJS)
%.cgi:%.cpp
	$(CPP) -o $@ $< $(LIB)
clean:
	rm -f $(OBJS)
