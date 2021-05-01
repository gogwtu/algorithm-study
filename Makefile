sorts: sorts.o generate_data.o
	gcc -o sorts sorts.o generate_data.o -lrt
sorts.o: sorts.c generate_data.h
	gcc -c sorts.c
generate_data.o: generate_data.c generate_data.h
	gcc -c generate_data.c
clean:
	rm -f *.o
