#include <stdio.h>
#include <stdlib.h>

/*
 * Template i'll be using for reading file input.
 */
int main(int argc, char* argv[argc+1]){
  if(argc != 2) {
    printf("Enter filename as program args.\n");
    exit(1);
  }

  FILE* fd = fopen(argv[1], "r");
  if(fd == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char* line = NULL;
  size_t n = 0;
  size_t read = 0;

  while((read = getline(&line, &n, fd)) != -1){
  }

  free(line);
  fclose(fd);
}
