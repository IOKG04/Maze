all:
	mkdir bin/
	gcc -o bin/build src/main.c src/display.c src/raycast.c

clean:
	rm -r bin/

call: clean all
