#include <iostream>
#include <cmath>
#include <vingraph.h>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <cstdlib>
#include <random>
#include <iostream>

using namespace std;

struct SharedControlData {
  char command;
  bool exit;
  int pic;
};

void *MoveFigure(void *args) {
  int figure_id = *((int *)args);
  tPoint window;
  tPoint f_pos;
  tPoint f_size;

  srand(time(NULL));

  usleep(rand() % 5000000 + 1000000);

  while (true) {
    window = GetDim(0);
    f_pos = GetPos(figure_id);
    f_size = GetDim(figure_id);

  float initial_x = (rand() % int(window.x - f_size.x)) + f_size.x / 2;
  float initial_y = (rand() % int(window.y - f_size.y)) + f_size.y / 2;

  MoveTo(initial_x, initial_y, figure_id);

  float dx = (rand() % 2000 - 1000) / 1000.0;
  float dy = (rand() % 2000 - 1000) / 1000.0;

  if (f_pos.x <= 0 || f_pos.x + f_size.x >= window.x) {
    dx = -dx;
  }
  if (f_pos.y <= 0 || f_pos.y + f_size.y >= window.y) {
    dy = -dy;
  }
  Move(figure_id, dx, dy);

  usleep(10000);
  }
}

void *MoveMainFigure(void *args) {
  struct SharedControlData *control_data = (struct SharedControlData *)args;
  int figure_id = control_data->pic;
  tPoint window;
  tPoint f_pos;
  tPoint f_size;
  float x = 0;
  float y = 0;
  int a = 10;
  int b = 100;
  bool isReverse = false;
  float phi = 0;

  control_data->exit = false;

  while (true) {
    window = GetDim(0);
    f_pos = GetPos(figure_id);
    f_size = GetDim(figure_id);

    char input = control_data->command;

    if (input != '\0') {
      if (input == 'T') {
        isReverse = true;
    } else if (input == 'R') {
      isReverse = false;
    } else if (input == 'Q') {
        b -= 10;
    } else if (input == 'S') {
        b += 10;
    } else if (input == 'E') {
    control_data->exit = true;
    break;
    }
  control_data->command = '\0';
  }

  float offset_x = window.x / 2;
  float offset_y = window.y / 2;

  x = (2 * a * a * cos(2 * phi) + b) * cos(phi);
  y = (2 * a * a * cos(2 * phi) + b) * sin(phi);

  if (isReverse) {
    phi -= 0.05;
  } else {
    phi += 0.05;
  }

    float new_x = offset_x + x - (f_size.x / 2);
    float new_y = offset_y + y - (f_size.y / 2);
    MoveTo(new_x, new_y, figure_id);

    usleep(10000);
  }

// munmap(control_data, sizeof(struct SharedControlData));
return NULL;
}


void *KeyboardInput(void *args) {
  struct SharedControlData *control_data = (struct SharedControlData *)args;

  while (true) {
    char input = InputChar();
    control_data->command = input;
    cout << "Input: " << input << endl;
  }

return NULL;
}


int main() {
  ConnectGraph();
  tPoint window = GetDim(0);
  int line_id = Line(70, 50, 130, 90);
  int rect_id = Rect(210, 40, 60, 60);
  int ellipse_id = Ellipse(350, 40, 60, 60);

  pthread_t thread_id1, thread_id2, thread_id3, thread_id4;

  pthread_create(&thread_id1, 0, MoveFigure, &line_id);
  pthread_create(&thread_id2, 0, MoveFigure, &rect_id);
  pthread_create(&thread_id3, 0, MoveFigure, &ellipse_id);

  int picture = Picture(300, 300);

  Ellipse(150, 200, 50, 60, RGB(255, 0, 0), picture);

  tPoint body1[] = {
    {150, 150},
    {140, 140},
    {160, 140}
    };
  int body1_id = Polygon(body1, 3, RGB(0, 255, 0), picture);

  tPoint body[] = {
    {130, 140},
    {120, 130},
    {140, 130}
    };
  int body_id = Polygon(body, 3, RGB(0, 255, 0), picture);

  tPoint robot[] = {
    {120, 170},
    {120, 190},
    {150, 190},
    {190, 170}
    };
  int robot_id = Polygon(robot, 4, RGB(255, 0, 255), picture);

  Show(picture);
  int main_figure = picture;


struct SharedControlData *control_data = (struct SharedControlData *)mmap(NULL, sizeof(struct SharedControlData), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


control_data->command = '\0';
control_data->exit = false;
control_data->pic = picture;


pthread_t keyboardThread;
pthread_create(&keyboardThread, 0, KeyboardInput, control_data);

pthread_create(&thread_id4, 0, MoveMainFigure, control_data);

pthread_join(keyboardThread, NULL);
CloseGraph();
return 0;
}