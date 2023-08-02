######################## MACROS ###########################
define COMPILE
$(5) : $(3)
	$(1) $(2) -c $(3) $(4) -o $(5)
endef

define TO
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef
##########################################################

GAME	  := app
C 	  	  := gcc
CC        := g++
CFLAGS	  := -std=c++20 -Wall -pedantic

ifdef DEBUG
	CFLAGS+=-O1 -g
else ifdef SANITIZE
	CFLAGS+=-fsanitize=address -fno-omit-frame-pointer -O1 -g
else
	CFLAGS+=
endif


ARCH      := -arch x86_64 -arch i386
MKDIR     := mkdir -p
SRC       := src
OBJ       := obj
LIB 	  := lib
SUBDIRS   	:= $(shell find $(SRC) -type d)
OBJSUBDIR 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))
ALLCPPS   	:= $(shell find $(SRC) -type f -iname *.cpp)
#ALLCS	  	:= $(shell find $(SRC) -type f -iname *.c)
ALLOBJS	    := $(foreach F,$(ALLCPPS),$(call TO,$(F)))
#ALLCPPOBJS   := $(patsubst %.cpp,%.o,$(ALLCPPS))
INCLUDE   	:= -I $(SRC) -I $(LIB) -I/usr/include
STCKLIBS    := $(shell find $(LIB) -type f -iname *.a)
LIBS	  	:= $(STCKLIBS) -lX11

.PHONY:  info clean cleanall libs libs-clean libs-cleanall

$(GAME) : $(OBJSUBDIR) $(ALLOBJS)
	$(CC) $(ALLOBJS) -o $(GAME) $(LIBS)
#	$(CC) $(patsubst $(SRC)%,$(OBJ)%,$(ALLCPPOBJS)) -o $(GAME)

#$(foreach F,$(ALLCS),$(eval $(call COMPILE,$(C),$(F),$(INCLUDE),$(call TO,$(F)),$(CFLAGS))))
$(foreach F,$(ALLCPPS),$(eval $(call COMPILE,$(CC),$(CFLAGS),$(F),$(INCLUDE),$(call TO,$(F)))))

#%.o : %.cpp
#	$(CC) -c $^ $(INCLUDE) -o $(patsubst $(SRC)%,$(OBJ)%,$@) $(CFLAGS)

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIR))
	$(info $(ALLCPPS))
#	$(info $(ALLCS))
	$(info $(ALLOBJS))
	$(info $(STCKLIBS))

$(OBJSUBDIR):
	$(MKDIR) $(OBJSUBDIR)

clean:
	rm -r $(OBJ)

cleanall:
	rm -r $(OBJ)
	rm $(GAME)

libs:
	$(MAKE) -C $(LIB)

libs-clean:
	$(MAKE) -C $(LIB) clean

libs-cleanall:
	$(MAKE) -C $(LIB) cleanall
