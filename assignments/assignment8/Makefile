# Simple Makefile for 6.837 curve editing assignment
# for g++ on linux platforms

# ===============================================================

SRCS 	 	= src/main.C \
		  src/matrix.C \
		  src/glCanvas.C \
		  src/spline_parser.C \
		  src/triangle_mesh.C \
		  src/spline.C \
		  src/curve.C \
		  src/surface.C


# ===============================================================
# NOTE:  On athena, the Mesa GL libraries are currently located in the
#        mesa_v501 locker.  Type "add mesa_v501" to access these files.

LIBS            = -lm -lGL -lGLU -lglut 
# INCLUDE 	= -I/mit/mesa_v501/Mesa-5.0.1/include/
CC		= g++
EXE	  	= curve_editor
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

