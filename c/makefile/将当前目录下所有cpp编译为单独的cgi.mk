CPP = g++ 

LIB = -lcgicc -lsqlite3 -ljson_linux-gcc-5.4.0_libmt
SRCS = $(wildcard *.cpp) 
OBJS = $(SRCS:.cpp=.cgi)
all:$(OBJS)
%.cgi:%.cpp
	$(CPP) -o $@ $< $(LIB)
clean:
	rm -f $(OBJS)
