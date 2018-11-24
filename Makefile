
CC=gcc

CFLAGS=-I../src/ `sdl2-config --cflags` `sdl2-config --libs` -lstdc++ -fopenmp -std=c++11 -lm -O3 -g

SRC=../src/utils.cpp ../src/Box.cpp ../src/Camera.cpp ../src/Light.cpp ../src/Mat.cpp \
	../src/Material.cpp ../src/Mesh.cpp ../src/RayTracer.cpp \
	../src/Scene.cpp ../src/Sphere.cpp ../src/Surface.cpp \
	../src/Transform.cpp ../src/Triangle.cpp ../src/Vertex.cpp \
	../src/BillboardTransform.cpp ../src/Billboard.cpp ../src/BVH.cpp 

SRC_OBJ=$(SRC:%.cpp=%.o)

all: hello_world shading universe cube textures earth transforms exercise cube_motion humanoids airplane marble complex city indoors

project: $(SRC_OBJ) project.cpp 
	$(CC) -o $@.out $^ $(CFLAGS)
	
lego: $(SRC_OBJ) src/lego.cpp 
	$(CC) -o $@.out $^ $(CFLAGS)


ifdef DEFINE
%.o: %.cpp
	$(CC) -D${DEFINE} -c -o $@ $^ $(CFLAGS)
else
%.o: %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)
endif

clean:
	rm *.out
	rm ../src/*.o
