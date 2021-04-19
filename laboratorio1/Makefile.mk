.DEFAULT_GOAL := procesoSuma
procesoSuma: escritura suma limpiar
	@echo "suma finalizada"
escritura:
	gcc ejemplo1.c -o ejemplo1
	./ejemplo1
suma: escritura
	gcc ejemplo2.c -o ejemplo2
	./ejemplo2
limpiar:
	rm out.txt