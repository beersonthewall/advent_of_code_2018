#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct point_t point_t;
struct point_t {
  int x;
  int y;
  bool inf;
};

// Manhattan distance
int dist(int x, int y, int a, int b){
  return abs(x - a) + abs(y - b);
}

int get_closest(int x, int y, point_t** points, size_t pts_sz){
  int min_dist = dist(x, y, points[0]->x, points[0]->y);
  int min_pt = 0;
  bool tie = false;

  for(int i = 1; i < pts_sz; i++){
    if(dist(x, y, points[i]->x, points[i]->y) == min_dist){
      tie = true;
    }
    if(dist(x, y, points[i]->x, points[i]->y) < min_dist){
      min_pt = i;
      min_dist = dist(x, y, points[i]->x, points[i]->y);
      tie = false;
    }
  }

  return tie? -2 : min_pt;
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
  size_t size = 0;
  point_t** points = malloc(sizeof(point_t*) * max);

  while((read = getline(&line, &n, fd)) != -1){
    point_t* pt = malloc(sizeof(point_t));
    sscanf(line, "%d, %d\n", &pt->x, &pt->y);

    if(size == max){
      max = max*2;
      points = realloc(points, sizeof(point_t*) * max);
    }

    points[size] = pt;
    size++;
  }

  // we have all the points adjust to exact size.
  size--;
  points = realloc(points, sizeof(point_t*) * size);

  size_t x_max = 1000;
  size_t y_max = 1000;

  int** coords = malloc(sizeof(int*) * x_max);
  for(int i = 0; i < x_max; i++){
    coords[i] = calloc(sizeof(int), y_max);
  }

  for(int i = 0; i < x_max; i++){
    for(int j = 0; j < y_max; j++){
      coords[i][j] = -1;
    }
  }

  for(int i = 0; i < size; i++){
    coords[points[i]->x][points[i]->y] = i;
  }

  for(int i = 0; i < x_max; i++){
    for(int j = 0; j < y_max; j++){
      coords[i][j] = get_closest(i, j, points, size);
    }
  }

  for(int i = 0; i < x_max; i++){
    if(coords[i][0] >= 0){
      points[coords[i][0]]->inf = true;
    }

    if(coords[i][y_max-1] >= 0){
      points[coords[i][y_max-1]]-> inf = true;
    }

    if(coords[0][i] >= 0){
      points[coords[0][i]]->inf = true;
    }

    if(coords[x_max-1][i] >= 0){
      points[coords[x_max-1][i]]->inf = true;
    }
  }

  int max_size = 0;
  for(int i = 0; i < size; i++){
    if(!points[i]->inf){
      int area = 0;
      for(int j = 0; j < x_max; j++){
        for(int k = 0; k < y_max; k++){
          if(coords[j][k] == i){
            area++;
          }
        }
      }
      if(area > max_size){
        max_size = area;
      }
    }
  }

  size_t safe_area_size = 0;
  for(int i = 0; i < x_max; i++){
    for(int j = 0; j < y_max; j++){

      int sum = 0;
      for(int k = 0; k < size; k++){
        sum += dist(i, j, points[k]->x, points[k]->y);
      }

      if(sum < 10000){
        safe_area_size++;
      }

    }
  }

  printf("Largest non-infinite area: %d\n", max_size);
  printf("Safe arean size: %zu\n", safe_area_size);

  for(int i = 0; i < x_max; i++){
    free(coords[i]);
  }

  free(coords);
  free(line);
  fclose(fd);
}
