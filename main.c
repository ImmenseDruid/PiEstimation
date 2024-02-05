#include <assert.h>
#include <pthread.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_POINTS 100000
const int SCREEN_SIZE = 1000;
const Vector2 center = (Vector2){0, SCREEN_SIZE};

typedef struct thread_data {
  int thread_id;
  Vector2 *points;
  int start;
  int stop;
} thread_data;

void *func(void *threadargs) {
  thread_data *mydata = (thread_data *)threadargs;
  printf("%i %i %i\n", mydata->thread_id, mydata->start, mydata->stop);
  for (int i = mydata->start; i < mydata->start +  mydata->stop; i++) {
    mydata->points[i] = (Vector2){rand() % SCREEN_SIZE, rand() % SCREEN_SIZE};
    if (Vector2Distance(mydata->points[i], center) > SCREEN_SIZE) {
    } else {
    }
  }

  return NULL;
}

int main(void) {
  InitWindow(SCREEN_SIZE, SCREEN_SIZE, "Estimation of Pi using random numbers");
  SetTargetFPS(30);
  srand(time(NULL));

  Vector2 *points = calloc(NUM_POINTS, sizeof(Vector2));
  int inside = 0;
  int outside = 0;

  const int threadNum = 16;
  pthread_t threads[threadNum];
  thread_data data_array[threadNum];
  void *res;

  int rc = 0;
  for (int i = 0; i < threadNum; i++) {
    data_array[i].thread_id = i;
    data_array[i].points = points;
    data_array[i].start = i * (NUM_POINTS / threadNum);
    data_array[i].stop = (NUM_POINTS / threadNum);
    rc = pthread_create(&threads[i], NULL, func, (void *)&data_array[i]);
    assert(!rc);
  }

  for (int i = 0; i < threadNum; i++) {
    rc = pthread_join(threads[i], NULL);
    assert(!rc);
  }
  printf("Pi is : %f\n", (float)(4.0f * inside) / (NUM_POINTS));

  bool doOnce = true;

  while (!WindowShouldClose()) {

    if (IsKeyPressed(KEY_R)) {
        inside = 0;
      for (int i = 0; i < threadNum; i++) {
        data_array[i].thread_id = i;
        data_array[i].points = points;
        data_array[i].start = i * (NUM_POINTS / threadNum);
        data_array[i].stop = (NUM_POINTS / threadNum);
        rc = pthread_create(&threads[i], NULL, func, (void *)&data_array[i]);
        assert(!rc);
      }

      for (int i = 0; i < threadNum; i++) {
        rc = pthread_join(threads[i], NULL);
        //inside += *((int *)res);
        assert(!rc);
      }


      printf("Pi is : %f\n", (float)(4.0f * inside) / (NUM_POINTS));
    }

    BeginDrawing();

    ClearBackground(BLACK);

    DrawCircleV(center, SCREEN_SIZE, RED);

    for (int i = 0; i < NUM_POINTS; i++) {
      DrawCircleV(points[i], 1, BLUE);
    }

    EndDrawing();
  }


  free(points);
  free(res);


  CloseWindow();
  return 0;
}
