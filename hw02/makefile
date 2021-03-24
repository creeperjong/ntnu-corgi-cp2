all:mixed.o bignum.o
	gcc hw0201.c mixed.o -o hw0201
	gcc hw0202.c -o hw0202
	gcc hw0203.c -o hw0203
	gcc hw0204.c bignum.o -o hw0204
mixed:
	gcc -c mixed.c -o mixed.o
bignum:
	gcc -c bignum.c -o bignum.o
clean:
	rm -rf *.o hw0201 hw0202 hw0203 hw0204