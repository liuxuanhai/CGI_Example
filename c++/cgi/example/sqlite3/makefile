CPP = g++ 

LIB = -lcgicc -lsqlite3
SRCS = $(wildcard *.cpp) 
OBJS = $(SRCS:.cpp=.cgi)
all:$(OBJS)
%.cgi:%.cpp
	$(CPP) -o $@ $< $(LIB)
clean:
	rm -f $(OBJS)
