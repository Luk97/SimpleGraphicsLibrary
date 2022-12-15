rule:	main.c sgl.c
	gcc -o main main.c sgl.c -I. && ./main
	xdg-open test.ppm