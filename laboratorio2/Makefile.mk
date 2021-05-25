.DEFAULT_GOAL := procesoBusqueda
procesoBusqueda: abrirServer mostrarInterfaz
	@echo "Busqueda finalizada"
mostrarInterfaz:
	cd frontend; \
	gcc p1-odProgram.c -o p1-odProgram; \
	./p1-odProgram 127.0.0.1
abrirServer:
	cd backend; \
	gcc servidor.c -lpthread -lm -o servidor; \
	./servidor & 