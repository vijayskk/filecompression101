CC=g++
CFLAGS = --std=c++11
EXEC=run
MAIN=src/main.cpp

main:$(MAIN)
	$(CC) $? $(CFLAGS) -o build/$(EXEC) 

run:
	build/$(EXEC)

clear:
	rm build/* 

install:
	cp build/$(EXEC) /usr/local/bin/vzip

uninstall:
	rm -f /usr/local/bin/vzip
