#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool find_match(char* polymer, size_t size, int* x, int* y){
  for(int i = 1; i < size; i++){
    if(tolower(polymer[i]) == tolower(polymer[i-1])){
      if((islower(polymer[i]) && isupper(polymer[i-1])) ||
         (isupper(polymer[i]) && islower(polymer[i-1]))){
        *x = i-1;
        *y = i;
        return true;
      }
    }
  }
  return false;
}

void reduce(char* polymer){
  bool clean_pass = false;
  size_t size = strlen(polymer);

  while(!clean_pass){

    int x;
    int y;
    if(find_match(polymer, size, &x, &y)){
      clean_pass = false;

      memmove(&polymer[x], &polymer[x+1], size - x);
      size = strlen(polymer);
      memmove(&polymer[x], &polymer[x+1], size - x);
      size = strlen(polymer);
    }
    else {
      clean_pass = true;
    }

  }
}

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

  fseek(fd, 0, SEEK_END);
  size_t size = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  char* polymer = malloc(size);
  fread(polymer, size, 1, fd);

  // just overwrite the ending newline.
  polymer[size-1] = 0;

  reduce(polymer);

  printf("Polymer size: %ld\n", strlen(polymer));
  printf("%s\n", polymer);

  free(polymer);
  fclose(fd);
}
