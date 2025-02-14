# A basic makefile. Once the project grows this will have to be improved

build/main: src/*.c
	gcc src/*.c -o build/main
