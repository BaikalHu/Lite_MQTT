all:
	gcc -g atiny*.c main.c -I ./ -o atiny -DATINY_DEBUG

clean:
	rm atiny
