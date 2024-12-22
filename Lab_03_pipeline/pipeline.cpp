#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
   // Pipes para comunicación padre-hijo
   int pipe_to_parent[2], pipe_to_child[2];
   pid_t pid; // Proceed ID
   FILE *file_hijo, *file_padre;
   char buffer[256];
   int line_count = 0;

   pipe(pipe_to_parent); // Crear pipe para comunicación padre-hijo
   pipe(pipe_to_child);  // Crear pipe para comunicación hijo-padre

   // Creando procesos
   pid = fork();

   // Proceso child
   if (pid == 0) {                          
      close(pipe_to_parent[0]); // Close read to pipe_to_parent
      close(pipe_to_child[1]);  // Close write to pipe_to_child

      if ((file_hijo = fopen("Child_file.txt", "r")) == NULL) {
         perror("Error abriendo archivo del hijo");
         exit(EXIT_FAILURE);
      }

      // Leer el archivo y enviar datos al padre
      while (fgets(buffer, sizeof(buffer), file_hijo) != NULL) {
         if (strcmp(buffer, "EOM\n") == 0) break; // Fin de mensaje
         write(pipe_to_parent[1], buffer, strlen(buffer)); // Enviar línea al padre
      }

      fclose(file_hijo);        // Close file
      close(pipe_to_parent[1]); // Close write to pipe_to_parent

      // Leer numero de lineas
      read(pipe_to_child[0], &line_count, sizeof(line_count));

      // Escribir cantidad de lineas
      if ((file_hijo = fopen("Child_counter.txt", "w")) == NULL) {
         perror("Error abriendo archivo num_hijo.txt");
         exit(EXIT_FAILURE);
      }
      fprintf(file_hijo, "Líneas en el documento padre: %d\n", line_count);
      fclose(file_hijo);

      close(pipe_to_child[0]); // Clore read to pipe_to_child
      exit(0);
   }

   // Proceso parent
   else {           
      close(pipe_to_parent[1]); // Close write to pipe_to_parent
      close(pipe_to_child[0]);  // Close read to pipe_to_child

      if ((file_padre = fopen("Parent_file.txt", "w")) == NULL) {
         perror("Error abriendo archivo del padre");
         exit(EXIT_FAILURE);
      }

      // Recibir datos del hijo y escribir en archivo
      while (read(pipe_to_parent[0], buffer, sizeof(buffer) - 1) > 0) {
         buffer[sizeof(buffer) - 1] = '\0'; // Verificar que el buffer tenga un final
         char *line = strtok(buffer, "\n"); // Separar por saltos de línea

         while (line != NULL) {
            fputs(line, file_padre);
            fputs("\n", file_padre);   // Asegurarse de que haya una nueva línea
            line_count++;              // Incrementar contador por cada línea
            line = strtok(NULL, "\n"); // Leer siguiente línea
         }
      }

      fclose(file_padre);      
      close(pipe_to_parent[0]); // Close read to pipe_to_parent

      // Sending lines to child
      write(pipe_to_child[1], &line_count, sizeof(line_count));
      close(pipe_to_child[1]); // Close write to pipe_to_child
   }

   return 0;
}