#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool compare(char* l1, char* l2){
  size_t s = strlen(l1);
  if(s != strlen(l2)){
    // all inputs same size
    return false;
  }

  int diffCt = 0;
  for(int i = 0; i < s; i++){
    if(l1[i] != l2[i]){
      diffCt++;
      if(diffCt >= 2){
        return false;
      }
    }
  }

  return true;
}

void find_off_by_one(char** list_of_strings, size_t s){
  for(int i = 0; i < s; i++){
    for(int j = i+1; j < s; j++){
      if(compare(list_of_strings[i], list_of_strings[j])){
        printf("first: %s\nsecond:: %s\n",
               list_of_strings[i],
               list_of_strings[j]);
        size_t len = strlen(list_of_strings[i]);
        for(int k = 0; k < len; k++){
          if(list_of_strings[i][k] == list_of_strings[j][k]){
            printf("%c", list_of_strings[i][k]);
          }
        }
        printf("\n");
        return;
      }
    }
  }
}

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

  size_t max = 25;
  size_t pos = 0;
  char** list_of_strings = malloc(max * sizeof(char*));

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

    if(pos == max){
      max += max;
      list_of_strings = realloc(list_of_strings, max * sizeof(char*));
    }

    // malloc new space for the line since getline will wipe the memory.
    list_of_strings[pos] = malloc((nread +1) * sizeof(char));
    strcpy(list_of_strings[pos], line);
    pos++;
  }

    // part2 answer
  find_off_by_one(list_of_strings, pos);

  // free list_of_strings
  for(int i = 0; i < pos; i++){
    free(list_of_strings[i]);
  }
  free(list_of_strings);

  // part1 answer.
  printf("2's: %d, 3's: %d, Checksum: %d\n",
         two_count,
         three_count,
         two_count * three_count);
}
