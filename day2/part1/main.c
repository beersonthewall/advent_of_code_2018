#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
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

  unsigned int two_count = 0;
  unsigned int three_count = 0;
  char* line = NULL;
  size_t strLen = 0;
  size_t nread = 0;

  while((nread = getline(&line, &strLen, fd)) != -1) {
    int letters[26] = { 0 };
    for(int i = 0; i < nread; i++) {
      letters[line[i]-97] += 1;
    }

    bool two_found = false;
    bool three_found = false;
    for(int i = 0; i < 26; i++) {
      if(!two_found && letters[i] == 2) {
        two_count++;
        two_found = true;
      }
      if(!three_found && letters[i] == 3) {
        three_count++;
        three_found = true;
      }
    }
  }

  printf("2's: %d, 3's: %d, Checksum: %d\n",
         two_count,
         three_count,
         two_count * three_count);
}
