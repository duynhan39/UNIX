all: main.o
	chmod +x *
	@echo Compiled matmultiply
main.o:
	g++ -pthread matmultiply.cpp -o matmultiply
clean: 
	rm -f matmultiply
	rm -f matmultiply.exe
