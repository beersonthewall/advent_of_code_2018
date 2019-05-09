#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int fabric[1000][1000] = { 0 };

typedef struct claim_t claim_t;
struct claim_t {
  int id;
  int x;
  int y;
  int width;
  int height;
};

void claim(claim_t* claim){
  int x = claim->x;
  int y = claim->y;
  int width_end = x + claim->width;
  int height_end = y + claim->height;

  for(int i = x; i < width_end; i++){
    for(int j = y; j < height_end; j++){
      fabric[i][j] = fabric[i][j] + 1;
    }
  }
}

bool claim_overlaps(claim_t* claim){
  int x = claim->x;
  int y = claim->y;
  int width_end = x + claim->width;
  int height_end = y + claim->height;

  for(int i = x; i < width_end; i++){
    for(int j = y; j < height_end; j++){
      if(fabric[i][j] > 1){
        return true;
      }
    }
  }
  return false;
}

int find_non_overlap(claim_t** claims, int size){
  int id = 0;
  for(int i = 0; i < size; i++){
    if(!claim_overlaps(claims[i])){
      id = claims[i]->id;
      break;
    }
  }
  return id;
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

  char* line = NULL;
  size_t n = 0;
  size_t read = 0;

  size_t max = 25;
  size_t pos = 0;
  claim_t** claims = malloc(max * sizeof(claim_t*));

  while((read = getline(&line, &n, fd)) != -1){
    if(pos >= max){
      max += max;
      claims = realloc(claims, max * sizeof(claim_t*));
    }

    claim_t* claim = malloc(sizeof(claim_t));
    sscanf(line, "#%d @ %d,%d: %dx%d\n", &claim->id, &claim->x,
           &claim->y, &claim->width, &claim->height);
    claims[pos] = claim;
    pos++;
  }

  for(int i = 0; i < pos; i++){
    claim(claims[i]);
  }

  long contestedSq = 0;
  for(int i = 0; i < 1000; i++){
    for(int j = 0; j < 1000; j++){
      if(fabric[i][j] > 1){
        contestedSq++;
      }
    }
  }

  printf("Contested: %ld\n", contestedSq);
  printf("Claim id: %d does not overlap any other claims\n",
         find_non_overlap(claims, pos));
  for(int i = 0; i < pos; i++){
    free(claims[i]);
  }

  free(line);
  free(claims);
  fclose(fd);
}
