######################## MACROS ###########################
define COMPILE
$(4) : $(2)
	$(1) -c $(2) $(3) -o $(4) $(5)
endef

define TO
$(patsubst %.c,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1)))
endef
##########################################################

LIB	      := libtinyPTC.a
C 		  := gcc
CFLAGS    := -Wall
ifdef DEBUG:
	CFLAGS+=-g
else
	CFLAGS+=-O3
endif
ARCH	  := -arch x86_64 -arch i386
MKDIR     := mkdir -p
SRC       := src
OBJ       := obj
SUBDIRS   	:= $(shell find $(SRC) -type d)
OBJSUBDIR 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))
ALLCS	  	:= $(shell find $(SRC) -type f -iname *.c)
ALLOBJS	    := $(foreach F,$(ALLCS),$(call TO,$(F)))
ARFLAGS		:= -rcs
INCLUDE   	:= -I $(SRC)  -I/usr/include

.PHONY:  info clean cleanall

$(LIB) : $(OBJSUBDIR) $(ALLOBJS) 
	ar $(ARFLAGS) $(LIB) $(ALLOBJS)
	ranlib $(LIB)

$(foreach F,$(ALLCS),$(eval $(call COMPILE,$(C),$(F),$(INCLUDE),$(call TO,$(F)),$(CFLAGS))))

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIR))
	$(info $(ALLCS))
	$(info $(ALLOBJS))

$(OBJSUBDIR):
	$(MKDIR) $(OBJSUBDIR)

clean:
	rm -r $(OBJ)
	
cleanall:
	rm -r $(OBJ)
	rm $(LIB)