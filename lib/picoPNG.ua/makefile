######################## MACROS ###########################
define COMPILE
$(4) : $(2)
	$(1) -c $(2) $(3) -o $(4) $(5)
endef

define TO
$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1)))
endef
##########################################################

LIB	      := libpicoPNG.a
CC 		  := g++ -std=c++17
CFLAGS    := -Wall
ifdef DEBUG:
	CFLAGS+=-g
else
	CFLAGS+=-O3
endif
ARCH      := -arch x86_64 -arch i386
MKDIR     := mkdir -p
SRC       := src
OBJ       := obj
SUBDIRS   	:= $(shell find $(SRC) -type d)
OBJSUBDIR 	:= $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))
ALLCPPS	  	:= $(shell find $(SRC) -type f -iname *.cpp)
ALLOBJS	    := $(foreach F,$(ALLCPPS),$(call TO,$(F)))
ARFLAGS		:= -rcs
INCLUDE   	:= -I $(SRC)

.PHONY:  info clean cleanall

$(LIB) : $(OBJSUBDIR) $(ALLOBJS) 
	ar $(ARFLAGS) $(LIB) $(ALLOBJS)
	ranlib $(LIB)

$(foreach F,$(ALLCPPS),$(eval $(call COMPILE,$(CC),$(F),$(INCLUDE),$(call TO,$(F)),$(CFLAGS))))

info:
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIR))
	$(info $(ALLCPPS))
	$(info $(ALLOBJS))

$(OBJSUBDIR):
	$(MKDIR) $(OBJSUBDIR)

clean:
	rm -r $(OBJ)

cleanall:
	rm -r $(OBJ)
	rm $(LIB)
