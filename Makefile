default:
	mkdir bin/
	gcc -o bin/build src/main.c

clean:
	rm -r bin/
