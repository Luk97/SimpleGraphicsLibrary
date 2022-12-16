rule:	examples/*.c sgl.c
	gcc -o checkerboard sgl.c examples/*.c -I.