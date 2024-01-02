default: compile

clean:
	del prog.exe

compile: main.cpp
	g++ -std=c++20 main.cpp -static -static-libgcc -o prog.exe

optimize: main.cpp
	g++ -std=c++20 main.cpp -O3 -static -static-libgcc -o prog.exe 