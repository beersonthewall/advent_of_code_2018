#include <stdio.h>
#include <stdlib.h>

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

  size_t num_read = 0;
  size_t len = 0;
  char* line = NULL;
  int freq = 0;

  while((num_read = getline(&line, &len, fd)) != -1) {
    if(line[0] == '+') {
      freq += atoi(line + 1);
    }
    else if(line[0] == '-') {
      freq -= atoi(line + 1);
    }
  }

  printf("Frequency: %d\n", freq);
  return 0;
}
