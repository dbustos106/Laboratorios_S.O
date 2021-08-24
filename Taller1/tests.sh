for i in $(ls ./Archivos);
do    

    nombreArchivo="${i%.*}"
    metrica="${nombreArchivo: -2:2}"
    valor="${nombreArchivo//[K,M,b]/""}"

    # Hacer los experimentos para el tamaño de archivo
    echo -e "----------------- Tamaño: $valor$metrica -----------------"
    
    echo Pipe
    gcc tuberias.c -o tuberias
    ./tuberias $i $valor $metrica

    wait $!

    echo Mkfifo
    gcc Mkfifo_productor.c -o Mkfifo_productor
    ./Mkfifo_productor $i $valor $metrica &
    gcc Mkfifo_consumidor.c -o Mkfifo_consumidor
    ./Mkfifo_consumidor $i $valor $metrica

    wait $!

    echo Memcom
    gcc Memcom.c -o Memcom
    ./Memcom $i $valor $metrica

    wait $!

    echo Ficheros
    gcc ficheros.c -o ficheros
    ./ficheros $i $valor $metrica

    wait $!

    echo Sockets
    gcc Sockets_productor.c -lpthread -lm -o Sockets_productor
    ./Sockets_productor $i $valor $metrica &
    gcc Sockets_consumidor.c -lpthread -lm -o Sockets_consumidor
    ./Sockets_consumidor $i $valor $metrica

    wait $!
    
done