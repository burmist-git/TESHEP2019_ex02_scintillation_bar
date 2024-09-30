name := lhcBrich
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

#CPPFLAGS += -I/home/burmist/root-6.04.00/include/
CPPFLAGS += -I/usr/include/root
#CPPFLAGS += -I$(ROOTSYS)/include
#EXTRALIBS = $(shell root-config --glibs)
EXTRALIBS = -L/usr/lib/x86_64-linux-gnu -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lMatrix -lMathCore -lThread -pthread -lm -ldl -rdynamic


include $(G4INSTALL)/config/binmake.gmk
