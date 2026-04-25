tp: main.c
	gcc -o tp -g -fsanitize=address ./main.c
clear:
	rm -rf tp
run: tp
	./tp $(ROWS) $(COLS) $(THREADS)
