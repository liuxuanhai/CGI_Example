GCC = gcc
CC = gcc

LIB = -ldl -lm -lzlog -lpthread 

BUILDLIBS =

CFLAG = -Wall 
PROJECT = Daemon

SAMDIR=./
SAMINCH=$(shell find $(SAMDIR) -name '*.c')
TMPDIR=$(dir $(SAMINCH))
SAMINCDIR=$(sort $(TMPDIR))

SRCS := $(foreach dir,$(SAMINCDIR),$(wildcard $(addprefix  ${dir}*,.c)))
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(PROJECT):$(OBJS)
	@ $(GCC) -o $@ $^ $(CFLAG) $(LIB) $(BUILDLIBS) 
	@ du -b $(PROJECT)
	@ du -h $(PROJECT)
	
%.o:%.c
	$(GCC) -c $(CFLAG) $(BUILDLIBS) $(INCLUDE) -o $@ $< 

	
clean:
	rm -f $(OBJS) $(PROJECT)

