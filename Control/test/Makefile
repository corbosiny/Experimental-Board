SRC=test.c ../pid.c ../abc.c
SRC_H=../pid.h ../pid.c ../abc.h ../abc.c test.c

test: $(SRC_H)
	gcc $(SRC) -o test

clean:
	rm test *~
