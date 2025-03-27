# Compiler
CXX = g++

# Compilation flags
CXXFLAGS = -Wall -std=c++17 `pkg-config --cflags opencv4`

# libraries
LDFLAGS = `pkg-config --libs opencv4`

# Name of the executable
EXEC = mdch_april

# Source files and objects
SRC = main.cpp \
      process_image/process_image.cpp \
      image/image.cpp \
      stb/stb_wrapper.cpp \
      buddy_system/buddy_allocator.cpp \
      measure_memorytime/measure_memorytime.cpp \
	  utils/utils.cpp

OBJ = main.o \
      process_image/process_image.o \
      image/image.o \
      stb/stb_wrapper.o \
      buddy_system/buddy_allocator.o \
      measure_memorytime/measure_memorytime.o \
	  utils/utils.o

# Compiling the executable
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

# Rules for compiling objects
main.o: main.cpp process_image/process_image.h image/image.h utils/utils.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

process_image.o: process_image/process_image.cpp process_image/process_image.h image/image.h
	$(CXX) $(CXXFLAGS) -c process_image/process_image.cpp -o process_image/process_image.o

image.o: image/image.cpp image/image.h
	$(CXX) $(CXXFLAGS) -c image/image.cpp -o image/image.o

stb_wrapper.o: stb/stb_wrapper.cpp stb/stb_image.h stb/stb_image_write.h
	$(CXX) $(CXXFLAGS) -c stb/stb_wrapper.cpp -o stb/stb_wrapper.o

buddy_allocator.o: buddy_system/buddy_allocator.cpp buddy_system/buddy_allocator.h
	$(CXX) $(CXXFLAGS) -c buddy_system/buddy_allocator.cpp -o buddy_system/buddy_allocator.o

measure_memorytime.o: measure_memorytime/measure_memorytime.cpp measure_memorytime/measure_memorytime.h
	$(CXX) $(CXXFLAGS) -c measure_memorytime/measure_memorytime.cpp -o measure_memorytime.o

utils.o: utils/utils.cpp utils/utils.h
	$(CXX) $(CXXFLAGS) -c utils/utils.cpp -o utils.o

# Cleaning
clean:
	rm -f $(OBJ) $(EXEC)

# ---------------------------------------------
# Execution with different parameters [Examples]

run-buddy:
	./$(EXEC) example_images/cosmic-tornado.png example_images/cosmic-tornado_output_with_buddy.png -angle 30 -scalar 1.5 -buddy

run-no-buddy:
	./$(EXEC) example_images/cosmic-tornado.png example_images/cosmic-tornado_output_no_buddy.png -angle 45 -scalar 1.5

run-no-all:
	./$(EXEC) example_images/cosmic-tornado.png example_images/cosmic-tornado_output_no_all.png -angle 0 -scalar 1.0

run-buddy-no-rotate:
	./$(EXEC) example_images/cosmic-tornado.png example_images/cosmic-tornado_output_no_rotate.png -angle 0 -scalar 0.5 -buddy

run-buddy-no-scale:
	./$(EXEC) example_images/cosmic-tornado.png example_images/cosmic-tornado_output_no_scale.png -angle 45 -scalar 1.0 -buddy
