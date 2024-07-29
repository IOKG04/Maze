default:
	mkdir bin/
	gcc -o bin/build src/main.c src/display.c

clean:
	rm -r bin/
