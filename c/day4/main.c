#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <glib.h>

#define HOURS_SIZE 60

enum ACTION { START, ASLEEP, WAKE };

typedef struct log_t log_t;
struct log_t {
  struct tm* tstmp;
  int guard_id;
  enum ACTION guard_action;
};

typedef struct guard_t guard_t;
struct guard_t {
  int id;
  int minutes_asleep;
};

guard_t* find_max_asleep(log_t** logs, size_t size){

  GHashTable* guard_table = g_hash_table_new(g_int_hash, g_int_equal);

  guard_t* guard = NULL;
  for(int i = 0; i < size; i++){

    if(logs[i]->guard_id != 0){
      guard = (guard_t*) g_hash_table_lookup(guard_table, &logs[i]->guard_id);

      if(!guard){
        // new guard
        guard = malloc(sizeof(guard_t));
        guard->id = logs[i]->guard_id;
        guard->minutes_asleep = 0;
        g_hash_table_insert(guard_table, &guard->id, guard);
      }
    }

    if(logs[i]->guard_action == WAKE && i != 0){
      log_t* last_log = logs[i-1];
      guard->minutes_asleep += logs[i]->tstmp->tm_min - last_log->tstmp->tm_min;
    }
  }

  GHashTableIter itr;
  g_hash_table_iter_init(&itr, guard_table);

  int* guard_id;
  guard_t* current;
  int max_asleep = 0;
  int sleepy_guard_id = 0;
  while(g_hash_table_iter_next(&itr, (void**) &guard_id, (void **) &current)){
    if(current->minutes_asleep > max_asleep){
      sleepy_guard_id = current->id;
      max_asleep = current->minutes_asleep;
    }
  }

  current = g_hash_table_lookup(guard_table, &sleepy_guard_id);
  g_hash_table_destroy(guard_table);
  return current;
}

void destroy_value(void* value){
  free(value);
}

void find_most_asleep_minute(log_t** logs, size_t size){
  GHashTable* guard_table =
    g_hash_table_new_full(g_int_hash, g_int_equal, NULL, destroy_value);

  int* hours = NULL;
  for(int i = 0; i < size; i++){

    if(logs[i]->guard_id != 0){
      hours = (int*) g_hash_table_lookup(guard_table, &(logs[i]->guard_id));

      if(!hours){
        // new guard
        hours = calloc(HOURS_SIZE, sizeof(int));
        g_hash_table_insert(guard_table, &(logs[i]->guard_id), hours);
      }
    }

    if(logs[i]->guard_action == WAKE && i != 0){
      log_t* last_log = logs[i-1];

      int j = last_log->tstmp->tm_min;
      int target = logs[i]->tstmp->tm_min;
      while(j != target){

        hours[j] += 1;

        j++;
        if(j >= 60){
          j = 0;
        }
      }
    }
  }

  GHashTableIter itr;
  g_hash_table_iter_init(&itr, guard_table);
  int* guard_id;
  int max_min = 0;
  int max_val = 0;
  int max_id = 0;
  int* current;

  while(g_hash_table_iter_next(&itr, (void**) &guard_id, (void **) &current)){
    for(int i = 0; i < HOURS_SIZE; i++){
      if(current[i] > max_val){
        max_val = current[i];
        max_id = *guard_id;
        max_min = i;
      }
    }
  }

  printf("Solution 2\n");
  printf("Guard id: %d slept most on minute: %d, with: %d minutes slept."
         " solution: %d\n",
         max_id, max_min, max_val, max_id * max_min);
  g_hash_table_destroy(guard_table);
}

int compare_tm(const void* t1, const void* t2){
  const log_t* l1 = *((log_t**) t1);
  const log_t* l2 = *((log_t**) t2);

  time_t time1 = mktime(l1->tstmp);
  time_t time2 = mktime(l2->tstmp);

  return (int) difftime(time1, time2);
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

  size_t max = 50;
  size_t pos = 0;

  log_t** logs = malloc(max * sizeof(log_t*));

  while((read = getline(&line, &n, fd)) != -1){
    struct tm* tstmp = calloc(1, sizeof(struct tm));
    log_t* log_entry = calloc(1, sizeof(log_t));
    char* guardState;

    // %m is a conversion format that indicates sscanf will
    // malloc the memory for us.
    sscanf(line, "[%d-%d-%d %d:%d] %m[^.]\n",
           &tstmp->tm_year, &tstmp->tm_mon, &tstmp->tm_mday,
           &tstmp->tm_hour, &tstmp->tm_min, &guardState);

    log_entry->tstmp = tstmp;

    // Now we need to parse the guard state.
    if(guardState[0] == 'G'){
      int id = 0;
      sscanf(guardState, "Guard #%d begins shift\n", &id);
      log_entry->guard_id = id;
      log_entry->guard_action = START;
    }
    else if(guardState[0] == 'f'){
      log_entry->guard_action = ASLEEP;
    }
    else if(guardState[0] == 'w'){
      log_entry->guard_action = WAKE;
    }

    if(pos >= max){
      max += max;
      logs = realloc(logs, max * sizeof(log_t*));
    }

    logs[pos] = log_entry;
    pos++;
    free(guardState);
  }

  qsort(logs, pos, sizeof(log_t*), compare_tm);
  guard_t* sleepyGuard = find_max_asleep(logs, pos);

  printf("Solution 1\n");
  printf("Sleepy Guard id: %d\n", sleepyGuard->id);

  // found the sleepiest guard now we want to find the
  // minute he sleeps the most.
  int hour[60] = {0};

  int last_id = logs[0]->guard_id;
  for(int i = 1; i < pos; i++){
    if(logs[i]->guard_id != 0){
      last_id = logs[i]->guard_id;
      continue;
    }

    if(logs[i]->guard_action == WAKE && sleepyGuard->id == last_id){
      // log the minutes
      for(int j = logs[i-1]->tstmp->tm_min; j < logs[i]->tstmp->tm_min; j++){
        hour[j] += 1;
      }
    }
  }

  int most_often = 0;
  for(int i = 0; i < 60; i++){
    if(hour[i] > hour[most_often]){
      most_often = i;
    }
  }

  printf("Most often asleep on minute: %d, so id * min = %d\n",
         most_often, most_often * sleepyGuard->id);

  find_most_asleep_minute(logs, pos);

  for(int i = 0; i < pos; i++){
    free(logs[i]->tstmp);
    free(logs[i]);
  }

  free(logs);
  free(line);
  fclose(fd);
}
