# Simple Makefile for 6.837 raycasting assignment
# for g++ on linux platforms

# ===============================================================
# update this list as you add new C++ source files

SRCS 	 	= src/main.C \
		  src/matrix.C \
		  src/image.C \
		  src/scene_parser.C \
		  src/object3d.C \
		  src/sphere.C \
		  src/triangle.C \
		  src/plane.C \
	 	  src/transform.C \
		  src/group.C \
		  src/material.C \
		  src/raytracer.C \
		  src/light.C \
		  src/camera.C \
		  src/rayTree.C \
		  src/grid.C \
		  src/boundingbox.C \
		  src/glCanvas.C \
		  src/marchinfo.C \
		  src/raytracing_stats.C \
		  src/perlin_noise.C \
		  src/film.C \
		  src/filter.C

# ===============================================================
# NOTE:  On athena, the Mesa GL libraries are currently located in the
#        mesa_v501 locker.  Type "add mesa_v501" to access these files.

LIBS            = -lm -lGL -lGLU -lglut \
		#   -Xlinker -rpath -Xlinker /mit/mesa_v501/lib/ -L/mit/mesa_v501/lib/
# INCLUDE 	= -I/mit/mesa_v501/Mesa-5.0.1/include/
CC		= g++
EXE	  	= raytracer
OBJS =  $(SRCS:.C=.o)

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE)

depend:
	$(CC)  -E -M $(SRCS) > Makefile.depend

clean: 
	-rm -f *~ *bak src/*.o  core.* Makefile.depend $(EXE) 

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC)  -o $@ $(OBJS) $(LIBS)

.C.o:
	$(CC)  $< -c -o $@

.cpp.o:
	$(CC) $< -c -o $@
# ===============================================================

-include Makefile.depend
