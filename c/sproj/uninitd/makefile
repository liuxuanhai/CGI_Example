GCC = gcc
CC = gcc

LIB = -ldl -lm -lpthread 

CFLAGS=
#-I. `./netsnmplib/bin/net-snmp-config --cflags` 
BUILDAGENTLIBS=
#=`./netsnmplib/bin/net-snmp-config --agent-libs`

# -Wall 生成所有警告信息
#CFLAG = -Wall -O2 -w
CFLAG = -Wall 
PROJECT = LinuxPlatServer

SAMDIR=./
SAMINCH=$(shell find $(SAMDIR) -name '*.c')
TMPDIR=$(dir $(SAMINCH))
SAMINCDIR=$(sort $(TMPDIR))

SRCS := $(foreach dir,$(SAMINCDIR),$(wildcard $(addprefix  ${dir}*,.c)))
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(PROJECT):$(OBJS)
	@ $(GCC) -o $@ $^ $(CFLAG) $(LIB) $(BUILDAGENTLIBS) 
	@ echo "\33[7mProgram Size:\33[0m \a\c"
	@ du -b $(PROJECT)
	@ du -h $(PROJECT)
	
%.o:%.c
	$(GCC) -c $(CFLAG) $(BUILDAGENTLIBS) $(INCLUDE) -o $@ $< 

	
clean:
	rm -f $(OBJS) $(PROJECT)

