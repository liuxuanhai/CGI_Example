GCC = g++
CC = g++

LIB = 

BUILDLIBS =

CFLAG = -Wall 
PROJECT = VerCenter

SAMDIR=./
SAMINCH=$(shell find $(SAMDIR) -name '*.cpp')
TMPDIR=$(dir $(SAMINCH))
SAMINCDIR=$(sort $(TMPDIR))

SRCS := $(foreach dir,$(SAMINCDIR),$(wildcard $(addprefix  ${dir}*,.cpp)))
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

$(PROJECT):$(OBJS)
	@ $(GCC) -o $@ $^ $(CFLAG) $(LIB) $(BUILDLIBS) 
	@ du -b $(PROJECT)
	@ du -h $(PROJECT)
	
%.o:%.c
	$(GCC) -c $(CFLAG) $(BUILDLIBS) $(INCLUDE) -o $@ $< 

	
clean:
	rm -f $(OBJS) $(PROJECT)

