rule:	examples/*.c sgl.c
	gcc -o test sgl.c examples/*.c -I.