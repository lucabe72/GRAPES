ifndef BASE
BASE = ../..
else
vpath %.c $(BASE)/src/$(notdir $(CURDIR))
endif
CFGDIR ?= ..

OBJS = input-stream.o output-stream.o

ifdef FFDIR
OBJS += input-stream-avf.o output-stream-avf.o
endif

all: libchunkiser.a

include $(BASE)/src/utils.mak
ifdef FFDIR
CPPFLAGS += -I$(FFDIR)
CPPFLAGS += -DAVF
endif