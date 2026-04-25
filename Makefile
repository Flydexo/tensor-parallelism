tp.out: main.c
	gcc -o tp.out -g -fsanitize=address ./main.c
clear:
	rm -rf tp.out
run: tp.out
	./tp.out $(ROWS) $(COLS) $(THREADS)
