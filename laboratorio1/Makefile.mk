.DEFAULT_GOAL := procesoBusqueda
procesoBusqueda: abrirServer mostrarInterfaz
	@echo "Busqueda finalizada"
mostrarInterfaz:
	gcc p1-odProgram.c -o p1-odProgram
	./p1-odProgram
abrirServer:
	gcc servidor.c -o servidor
	./servidor &