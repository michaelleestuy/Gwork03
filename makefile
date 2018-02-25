run:
	gcc matrix.c -o test.out
	rm -f basic.ppm basic.png
	./test.out
	convert basic.ppm basic.png
