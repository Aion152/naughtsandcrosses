#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#define BUFFER_WIDTH	800
#define BUFFER_HEIGHT	800

#define DISPLAY_SCALE	1
#define DISPLAY_WIDTH 	(BUFFER_WIDTH  * DISPLAY_SCALE)
#define DISPLAY_HEIGHT 	(BUFFER_HEIGHT * DISPLAY_SCALE)

// DISPLAY
ALLEGRO_DISPLAY* display;
ALLEGRO_BITMAP* buffer;

// MISC VARIABLES
int msX = 0;
int msY = 0;
const double BOARD_PADDING = 160;
const double MB_PADDING = 20;
const double CELL_LENGTH = 40;
const double CELL_PADDING = 10;

// ARRAY OF O AND X
const int DMNSN_SZ = 3;
char boardArray[3][3][3][3];


void must_init(bool test, const char *description)
{
	if(test) return;

	printf("Couldn't initialise %s\n", description);
	exit(1);
}

void mouse_init()
{
  must_init(al_install_mouse(), "mouse");
}

void display_init()
{
  al_set_new_window_title("Naughts and Crosses");
  display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  must_init(display, "display");

  buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
  must_init(buffer, "bitmap buffer");
}

void display_deinit()
{
  al_destroy_bitmap(buffer);
  al_destroy_display(display);
}

void display_pre_draw()
{
  al_set_target_bitmap(buffer);
}

void display_post_draw()
{
  al_set_target_backbuffer(display);
  al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_WIDTH, BUFFER_HEIGHT, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
  al_flip_display();
}

void board_init()
{
  for(int y = 0; y < DMNSN_SZ; y++)
  {
    for(int x = 0; x < DMNSN_SZ; x++)
    {
      for(int y2 = 0; y2 < DMNSN_SZ; y2++)
      {
        for(int x2 = 0; x2 < DMNSN_SZ; x2++)
        {
          if(x2 % 2 == 1)
          {
            boardArray[y][x][y2][x2] = 'x';
          }
          else
          {
            boardArray[y][x][y2][x2] = 'o';
          }
        }
      }
    }
  }
}

void x_draw(int x_0, int y_0)
{
  int x_1 = x_0 + 20;
  int y_1 = y_0 + 20;
  al_draw_line(x_0, y_0, x_1, y_1, al_map_rgb_f(1, 0, 0), 2);
  x_0 += 20;
  x_1 -= 20;
  al_draw_line(x_0, y_0, x_1, y_1, al_map_rgb_f(1, 0, 0), 2);
}

void o_draw(double x_0, double y_0)
{
  double r = (CELL_LENGTH - (2 * CELL_PADDING)) / 2;
  x_0 += r;
  y_0 += r;
  al_draw_circle(x_0, y_0, r, al_map_rgb_f(0, 0, 1), 2);
}

void board_output()
{
  for(int y = 0; y < DMNSN_SZ; y++)
  {
    for(int x = 0; x < DMNSN_SZ; x++)
    {
      for(int y2 = 0; y2 < DMNSN_SZ; y2++)
      {
        for(int x2 = 0; x2 < DMNSN_SZ; x2++)
        {
          double x_0 = ((1 + x) * BOARD_PADDING) + MB_PADDING + (x2 * CELL_LENGTH) + CELL_PADDING;
          double y_0 = ((1 + y) * BOARD_PADDING) + MB_PADDING + (y2 * CELL_LENGTH) + CELL_PADDING;
          if(boardArray[y][x][y2][x2] == 'x')
          {
            x_draw(x_0, y_0);
          }
          else if(boardArray[y][x][y2][x2] == 'o')
          {
            o_draw(x_0, y_0);
          }
        }
      }
    }
  }
}

void board_draw()
{
  double x_0, y_0, x_1, y_1;
  for(int i = 1; i < 3; i++)
  {
    x_0 = x_1 = (1 + i) * BOARD_PADDING;
    y_0 = BOARD_PADDING;
    y_1 = 4 * BOARD_PADDING;

    al_draw_line(x_0, y_0, x_1, y_1, al_map_rgb_f(1, 1, 1), 1);
    al_draw_line(y_0, x_0, y_1, x_1, al_map_rgb_f(1, 1, 1), 1);
  }

  for(int y = 1; y < 4; y++)
  {
    y_0 = y * BOARD_PADDING + MB_PADDING;
    y_1 = y_0 + 3 * CELL_LENGTH;
    for(int x = 1; x < 4; x++)
    {
      x_0 = x_1 = x * BOARD_PADDING + MB_PADDING;
      for(int o = 1; o < 3; o++)
      {
        x_0 = x_1 += CELL_LENGTH;
        al_draw_line(x_0, y_0, x_1, y_1, al_map_rgb_f(1, 1, 1), 1);
        al_draw_line(y_0, x_0, y_1, x_1, al_map_rgb_f(1, 1, 1), 1);
      }
    }
  }
  board_output();
}


int main()
{
  must_init(al_init(), "Allegro");

  ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
  must_init(timer, "timer");

  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  must_init(queue, "queue");

  must_init(al_init_primitives_addon(), "primitives");


  display_init();
  mouse_init();
  board_init();

  al_register_event_source(queue, al_get_display_event_source(display));
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_mouse_event_source());


  srand(time(NULL));

  bool done = false;
  bool redraw = true;
  ALLEGRO_EVENT event;

  al_start_timer(timer);
  while(1)
  {
    al_wait_for_event(queue, &event);

    switch(event.type)
    {
      case ALLEGRO_EVENT_TIMER:
        redraw = true;
        break;
      case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        msX = event.mouse.x;
        msY = event.mouse.y;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;
    }

    if (done)
      break;

    if(redraw && al_is_event_queue_empty(queue))
    {
      display_pre_draw();

      al_clear_to_color(al_map_rgb(0, 0, 0));

      board_draw();

      display_post_draw();
      redraw = false;

      if(done)
      {
        sleep(3);
        break;
      }
    }
  }

  display_deinit();
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  return 0;
}
