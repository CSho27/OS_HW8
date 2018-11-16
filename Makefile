all: prob1.o memory_server.o prob2_writer.o prob2_reader.o memory_server3.o prob3_writer.o prob3_reader.o
	gcc -o prob1 prob1.o -lrt -pthread
	gcc -o memory_server memory_server.o -lrt -pthread
	gcc -o prob2_writer prob2_writer.o -lrt -pthread
	gcc -o prob2_reader prob2_reader.o -lrt -pthread
	gcc -o memory_server3 memory_server3.o -lrt -pthread
	gcc -o prob3_writer prob3_writer.o -lrt -pthread
	gcc -o prob3_reader prob3_reader.o -lrt -pthread
	
prob1.o: prob1.c
	gcc -Wall -Werror -g -c prob1.c -fopenmp

memory_server.o: memory_server.c
	gcc -Wall -Werror -g -c memory_server.c -fopenmp
	
prob2_writer.o: prob2_writer.c
	gcc -Wall -Werror -g -c prob2_writer.c -fopenmp
	
prob2_reader.o: prob2_reader.c
	gcc -Wall -Werror -g -c prob2_reader.c -fopenmp

memory_server3.o: memory_server3.c
	gcc -Wall -Werror -g -c memory_server3.c -fopenmp

prob3_writer.o: prob3_writer.c
	gcc -Werror -g -c prob3_writer.c -fopenmp
	
prob3_reader.o: prob3_reader.c
	gcc  -Werror -g -c prob3_reader.c -fopenmp
	
clean:
	rm -f *.o
	rm -f prob1
	rm -f memory_server
	rm -f prob2_writer
	rm -f prob2_reader
	rm -f memory_server3
	rm -f prob3_writer
	rm -f prob3_reader