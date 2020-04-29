all: array thread1 thread2

array: array.c
		gcc array.c -o array
thread1: thread1.c
		gcc thread1.c -o thread1 -lpthread
thread2: thread2.c
		gcc thread2.c -o thread2 -lpthread
clean:
		rm array; rm thread1; rm thread2; result.txt