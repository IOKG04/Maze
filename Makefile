all:
	mkdir bin/
	gcc -o bin/build src/main.c src/display.c src/raycast.c -lm

clean:
	rm -r bin/

call: clean all
