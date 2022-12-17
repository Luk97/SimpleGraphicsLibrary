test: test.c sgl.c
	gcc -o test sgl.c test.c -I.

checkerboard:	examples/checkerboard.c sgl.c
	gcc -o checkerboard sgl.c examples/checkerboard.c -I.

japanflag: examples/japanflag.c sgl.c
	gcc -o japanflag sgl.c examples/japanflag.c -I.

