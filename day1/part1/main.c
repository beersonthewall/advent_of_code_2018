#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[argc+1]) {
  if(argc != 2) {
    printf("Enter filename as program args.\n");
    exit(1);
  }

  FILE* fd = fopen(argv[1], "r");
  if(fd == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  char* input = NULL;
  if(fseek(fd, 0L, SEEK_END)) {
    printf("Seek error.\n");
    exit(1);
  }

  long sz = ftell(fd);
  if(sz == -1) {
    printf("error\n");
    exit(1);
  }

  input = malloc(sizeof(char) * (sz + 1));
  fseek(fd, 0L, SEEK_SET);

  long newLen = fread(input, sizeof(char), sz, fd);
  if(ferror(fd) != 0){
    printf("Error in read\n");
    exit(1);
  }
  else {
    input[newLen++] = '\0';
  }
  fclose(fd);

  

  char* line = strtok(input, "\n");
  long total = 0;
  while(line != NULL) {

    if(strstr(line, "+")){
      total += atol(line+1);
    }
    else if(strstr(line, "-")){
      total -= atol(line+1);
    }

    line = strtok(NULL, "\n");
  }

  printf("Total: %ld\n", total);

  free(input);
  return 0;
}
