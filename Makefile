all:
	gcc -g atiny*.c main.c src/mqttpacket/*.c  -I ./ -I ./src/mqttpacket -o atiny -DATINY_DEBUG

clean:
	rm atiny
