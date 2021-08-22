for i in $(ls ./Archivos);
do    

    nombreArchivo="${i%.*}"
    metrica="${nombreArchivo: -2:2}"
    valor="${nombreArchivo//[K,M,b]/""}"

    # Hacer los experimentos para el tamaño de archivo
    echo -e "----------------- Tamaño: $valor$metrica -----------------"
    
    ################################## Pipe ##################################
    echo Pipe

    runtimesum=0
    for j in {1..50};
    do
        # Capturar el inicio
        start=`date +%s.%N`

        gcc tuberias.c -o tuberias
        ./tuberias $i $valor $metrica

        # Capturar el final
        end=`date +%s.%N`

        runtime=$(echo "$end - $start" | bc -l)
        runtimesum=$(echo "$runtimesum + $runtime" | bc -l)
    done

    echo -e "Tiempo medio: $(echo "$runtimesum / 50" | bc -l) \n"

    ################################## Mkfifo ##################################
    echo Mkfifo

    runtimesum=0
    for j in {1..50};
    do
        # Capturar el inicio
        start=`date +%s.%N`

        gcc Mkfifo_productor.c -o Mkfifo_productor
        ./Mkfifo_productor $i $valor $metrica &

        gcc Mkfifo_consumidor.c -o Mkfifo_consumidor
        ./Mkfifo_consumidor $i $valor $metrica

        # Capturar el final
        end=`date +%s.%N`

        runtime=$(echo "$end - $start" | bc -l)
        runtimesum=$(echo "$runtimesum + $runtime" | bc -l)
    done

    echo -e "Tiempo medio: $(echo "$runtimesum / 50" | bc -l) \n"

    ################################## Archivos ##################################
    echo Archivos

    runtimesum=0
    for j in {1..50};
    do
        # Capturar el inicio
        start=`date +%s.%N`

        gcc Archivos_productor.c -o Archivos_productor
        ./Archivos_productor $i $valor $metrica &

        gcc Archivos_consumidor.c -o Archivos_consumidor
        ./Archivos_consumidor $i $valor $metrica

        # Capturar el final
        end=`date +%s.%N`

        runtime=$(echo "$end - $start" | bc -l)
        runtimesum=$(echo "$runtimesum + $runtime" | bc -l)
    done

    echo -e "Tiempo medio: $(echo "$runtimesum / 50" | bc -l) \n"

    ################################## Memoria C ##################################
    
done