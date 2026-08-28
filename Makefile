CFLAGES := -Wall -g -00



semu:sEMU2.c
	gcc sEMU2.c -o semu
file_wr:filewr.c
	gcc filewr.c -o file_wr
ex16:ex16.c
	gcc ex16.c -o ex16
ex17:ex17.c
	gcc ex17.c -o ex17
ex18:ex18.c
	gcc ex18.c -o ex18
ex18_q2:ex18_q2.c
	gcc ex18_q2.c -o ex18_q2
game:object.c ex19.c
	gcc -c object.c -o object.o
	gcc -c ex19.c -o ex19.o
	gcc object.o ex19.o -o game
game2:object.c ex19_q.c
	gcc -c object.c -o object.o
	gcc -c ex19_q.c -o ex19_q.o
	gcc object.o ex19_q.o -o game2
mini:minirvEMU.c
	gcc minirvEMU.c -o mini
clean:
	rm -f semu ex16 ex17 ex18 ex18_q2 object.o ex19.o game game2 ex19_q.o mini
