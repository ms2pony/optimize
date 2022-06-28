
app: test1.o test3.o
	gcc test1.o test3.o -o app

test3.o: test3.asm
	nasm -g -f elf64 $< -o $@

test1.o: test1.c
	gcc -g -c $<

clean:
	rm -rf a.out app *.o

