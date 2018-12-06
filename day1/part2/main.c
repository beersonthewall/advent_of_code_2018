#include <stdio.h>
#include <stdbool.h>
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
  size_t hist_sz = 20;
  int hist_pos = 0;
  int* freq_hist = malloc(hist_sz * sizeof(int));

  while((num_read = getline(&line, &len, fd)) != -1) {

    if(hist_pos >= hist_sz - 1) {
      hist_sz += hist_sz;
      freq_hist = realloc(freq_hist, hist_sz * sizeof(int));
    }

    freq_hist[hist_pos] = freq;
    hist_pos++;

    if(line[0] == '+') {
      freq += atoi(line + 1);
    }
    else if(line[0] == '-') {
      freq -= atoi(line + 1);
    }
  }

  freq_hist[hist_pos] = freq;
  printf("lst 2: %d, %d\n", freq_hist[hist_pos - 1], freq_hist[hist_pos]);
  printf("fst 2: %d, %d\n", freq_hist[0], freq_hist[1]);

  // find the first repeat frequency.
  bool repeat = false;
  int count = 1;
  while(!repeat) {
    int change = freq_hist[count] - freq_hist[count - 1];
    freq = freq + change;
    count++;

    // TODO: a hash table would be way faster.
    for(int i = 0; i < hist_pos; i++) {
      if(freq == freq_hist[i]) {
        printf("repeat: %d\n", freq);
        repeat = true;
      }
    }

   if(hist_pos == hist_sz - 1) {
      hist_sz += hist_sz;
      freq_hist = realloc(freq_hist, hist_sz * sizeof(int));
    }

   printf("new: %d\n", freq);
   printf("change: %d\n", change);
    freq_hist[hist_pos] = freq;
    hist_pos++;
  }

  printf("Frequency: %d\n", freq);
  free(freq_hist);
  fclose(fd);
  return 0;
}
