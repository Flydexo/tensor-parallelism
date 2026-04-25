tp.out: main.c
	gcc -o tp.out -g -fsanitize=address ./main.c
clear:
	rm -rf tp
run: tp
	./tp $(ROWS) $(COLS) $(THREADS)
