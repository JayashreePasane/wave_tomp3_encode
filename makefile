
CPP=g++

ifndef CPPFLAGS 
CPPFLAGS  = -MMD -O0 -Wall -g3 -MP -std=c++17
endif

LDFLAGS = -static

OBJS = main.o  wave.o

INC_DIR = -I"C:\MinGW_64\include"

LIBS = -lmp3lame
LIBS += -lpthread
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
	$(Q)$(LDO) $(LDFLAGS) -o waveToMp3Encode $(OBJS) $(LIBS) $(INC_DIR) $(INC)
	@$(E) "  LD " $@

clean:
	-rm ./wave.d
	-rm ./wave.o