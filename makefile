CPP=g++

ifndef CPPFLAGS 
CPPFLAGS  = -MMD -O0 -Wall -g3 -MP
endif

LDFLAGS = -static

OBJS = main.o

INC_DIR = -I"C:\MinGW\include"

LIBS = -lmp3lame
LIBS += -lpthread -lm
LIBDIR += -Llame_lib
LIBS += $(LIBDIR)

ifndef LDO
LDO=$(CPP)
endif

Q=@
E=echo
ifeq ($(V), 1)
Q=
E=true
endif
ifeq ($(QUIET), 1)
Q=@
E=true
endif


BINALL=waveToMp3Encode
ALL = $(BINALL)

all: $(ALL)

%.o: %.c
	$(Q)$(CPP) -c -o $@ $(CPPFLAGS ) $<
	@$(E) "  CPP " $<

waveToMp3Encode: $(OBJS)
	$(Q)$(LDO) $(LDFLAGS) -o waveToMp3Encode $(OBJS) $(LIBS) $(INC_DIR)
	@$(E) "  LD " $@

clean:
	rm -f *.o
	rm -f *.d