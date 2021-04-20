correrPrograma, correrServer
correrPrograma:
	gcc servidor.c -o servidor
	./servidor
correrServer:
	gcc p1-odProgram.c -o p1-odProgram
	./p1-odProgram
	