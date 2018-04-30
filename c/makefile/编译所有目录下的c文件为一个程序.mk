GCC =gcc 
CC = gcc 
STRIPCC = strip

LIB = -ldl -lm -lpthread -lssh -lopenbsd-compat  -lresolv -lcrypto -ldl -lutil  -lz  -lcrypt -lrt 

LDFLAGS=-L./openssl/lib -L./Common/Ftp/sftp -L./Common/Ftp/sftp/openbsd-compat
INCLUDE = -I./Common/Ftp/sftp/ -I./Common/Ftp/sftp/openbsd-compat/ 
#-I./netsnmplib/include/

CFLAGS=
#-I. `./netsnmplib/bin/net-snmp-config --cflags` 
BUILDAGENTLIBS=
#`./netsnmplib/bin/net-snmp-config --agent-libs`

# -Wall 生成所有警告信息
CFLAG = -Wall -O2 -w
CFLAG = -Wall 
PROJECT = ARM

SAMDIR=./
SAMINCH=$(shell find $(SAMDIR) -name '*.c')
TMPDIR=$(dir $(SAMINCH))
SAMINCDIR=$(sort $(TMPDIR))

SRCS := $(foreach dir,$(SAMINCDIR),$(wildcard $(addprefix  ${dir}*,.c)))
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(PROJECT):$(OBJS)
	@ $(GCC) -o $@ $^ $(CFLAG)  $(LDFLAGS) $(LIB) $(BUILDAGENTLIBS) 
	@ echo "\33[7mProgram Size:\33[0m \a\c"
	@ $(STRIPCC) $(PROJECT)
	@ du -b $(PROJECT)
	
%.o:%.c
	$(GCC) -c $(CFLAG) $(BUILDAGENTLIBS) $(INCLUDE) -o $@ $< 

	
clean:
	rm -f $(OBJS) $(PROJECT)

