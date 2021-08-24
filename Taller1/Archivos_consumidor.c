#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FileName "data.dat"

void reportar_y_salir(const char* msg) {
    perror(msg);
    exit(-1); 
}

int main() {
  struct flock lock;
  lock.l_type = F_WRLCK;    /* read/write (exclusive) lock */
  lock.l_whence = SEEK_SET; /* base for seek offsets */
  lock.l_start = 0;         /* 1st byte in file */
  lock.l_len = 0;           /* 0 here means 'until EOF' */
  lock.l_pid = getpid();    /* process id */

  int fd; // Descriptor de archivo.
  if ((fd = open(FileName, O_RDONLY)) < 0)  
    reportar_y_salir("open to read failed...");

  // Si el archivo está bloqueado por escritura, se duerme el proceso. 
  fcntl(fd, F_GETLK, &lock); //Pone lock.l_type a F_UNLCK si no hay bloqueo por escritura. 
  //if (lock.l_type != F_UNLCK)
  while (lock.l_type != F_UNLCK)
    printf("Arcihvo aún tiene bloqueo por escritura. Espere por favor");
    sleep(5);

  lock.l_type = F_RDLCK; //Este bloqueo previene cualquier escritura mientras se está leyendo. 
  if (fcntl(fd, F_SETLK, &lock) < 0)
    reportar_y_salir("No se pudo conseguir un bloqueo solo para lectura. ");


  int c; // Buffer para leer los bytes. 
  while (read(fd, &c, 1) > 0)    
    //write(STDOUT_FILENO, &c, 1); // Escribir byte por byte al output standard. 

  // Desbloquear el archivo manualmente. 
  lock.l_type = F_UNLCK;
  if (fcntl(fd, F_SETLK, &lock) < 0)
    reportar_y_salir("Fallo en el desbloqueo manual.");

  printf("El consumidor ha leído el archivo. \n");
  printf("\n");

  close(fd); // Se cierra el archivo de comunicación. 
  remove(FileName); // Se borra el archivo para evitar sobreescritura. 
  
  return 0;
}