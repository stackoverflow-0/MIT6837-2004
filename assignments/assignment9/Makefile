# Simple Makefile for 6.837 particle system assignment
# for g++ on linux platforms

# ===============================================================

SRCS 	 	= src/main.C \
		  src/matrix.C \
		  src/glCanvas.C \
		  src/system.C \
		  src/particle.C \
		  src/parser.C  \
		  src/perlin_noise.C \
		  src/generator.C

# ===============================================================
# NOTE:  On athena, the Mesa GL libraries are currently located in the
#        mesa_v501 locker.  Type "add mesa_v501" to access these files.

LIBS            = -lm -lGL -lGLU -lglut
CC		= g++
EXE	  	= particle_system
OBJS =  $(SRCS:.C=.o)

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE)

depend:
	$(CC) $(INCLUDE) -E -M $(SRCS) > Makefile.depend

clean: 
	-rm -f *~ *bak src/*.o  core.* Makefile.depend $(EXE) 

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) $(INCLUDE) -o $@ $(OBJS) $(LIBS)

.C.o:
	$(CC) $(INCLUDE) $< -c -o $@

# ===============================================================

-include Makefile.depend