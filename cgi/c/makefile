CPP = gcc

LIB = -lcgicc -lsqlite3
SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c=.cgi)
all:$(OBJS)
%.cgi:%.c
	$(CPP) -o $@ $< $(LIB)
clean:
	rm -f $(OBJS)
