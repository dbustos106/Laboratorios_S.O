busqueda: correrPrograma & correrServer
correrPrograma:
	gcc p1-odProgram.c -o p1-odProgram
	./p1-odProgram
correrServer:
	gcc servidor.c -o servidor
	./servidor