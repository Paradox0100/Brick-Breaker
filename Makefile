build:
	g++ -o program src/*.cpp -lSDL2 -lSDL2_ttf
run:
	./program
clean:
	rm -f program
