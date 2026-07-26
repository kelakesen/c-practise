semu:sEMU2.c
	gcc sEMU2.c -o semu
file_wr:filewr.c
	gcc filewr.c -o file_wr
ex16:ex16.c
	gcc ex16.c -o ex16
ex17:ex17.c
	gcc ex17.c -o ex17
clean:
	rm semu ex16 ex17
