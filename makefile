#
# Specifiy the target
all:	predictors

# Specify the object files that the target depends on
# Also specify the object files needed to create the executable
predictors:	project1.o
	g++ project1.o  -o project1

# Specify how the object files should be created from source files
project1.o:	project1.cpp
	gcc -g -c project1.cpp -o project1.o

# Specify the object files and executables that are generated
# and need to be removed to re-compile the whole thing
clean:
	rm -f *.o *~ project1
