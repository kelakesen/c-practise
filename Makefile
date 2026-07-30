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
clean:
	rm semu ex16 ex17 ex18 ex18_q2
