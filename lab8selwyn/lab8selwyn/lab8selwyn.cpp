// Tyler Selwyn
// CPSC 440 - Lab 8

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

const int SCREEN_W = 900;
const int SCREEN_H = 800;
const float SPEED = 3.0f;

int main() {
    al_init();
    al_install_keyboard();
    al_init_image_addon();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // load and scale rocket down to 64x64
    ALLEGRO_BITMAP* rocket_raw = al_load_bitmap("rocket.png");
    ALLEGRO_BITMAP* rocket = al_create_bitmap(64, 64);
    al_set_target_bitmap(rocket);
    al_draw_scaled_bitmap(rocket_raw, 0, 0,
        al_get_bitmap_width(rocket_raw), al_get_bitmap_height(rocket_raw),
        0, 0, 64, 64, 0);
    al_set_target_bitmap(al_get_backbuffer(display));
    al_destroy_bitmap(rocket_raw);

    ALLEGRO_BITMAP* bg = al_load_bitmap("background.jpg");

    if (!rocket || !bg) {
        al_destroy_display(display);
        return -1;
    }

    int rw = al_get_bitmap_width(rocket);
    int rh = al_get_bitmap_height(rocket);
    float cx = rw / 2.0f;
    float cy = rh / 2.0f;

    float rocket_x = SCREEN_W / 2.0f;
    float rocket_y = SCREEN_H / 2.0f;
    float dx = 0, dy = 0;

    // flip and rotation for facing direction
    int flip_flag = 0;
    float base_angle = 0;
    int direction = 0; // 0=right 1=left 2=up 3=down

    bool done = false;
    ALLEGRO_EVENT ev;

    al_start_timer(timer);

    while (!done) {
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            done = true;

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                done = true;

            if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                dx = SPEED; dy = 0;
                direction = 0;
                flip_flag = 0;
                base_angle = 1.57f;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                dx = -SPEED; dy = 0;
                direction = 1;
                flip_flag = ALLEGRO_FLIP_HORIZONTAL;
                base_angle = -1.57f;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                dx = 0; dy = -SPEED;
                direction = 2;
                flip_flag = 0;
                base_angle = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                dx = 0; dy = SPEED;
                direction = 3;
                flip_flag = ALLEGRO_FLIP_VERTICAL;
                base_angle = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                dx = 0; dy = 0;
            }
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            rocket_x += dx;
            rocket_y += dy;
        }

        if (al_is_event_queue_empty(event_queue)) {
            al_draw_scaled_bitmap(bg,
                0, 0, al_get_bitmap_width(bg), al_get_bitmap_height(bg),
                0, 0, SCREEN_W, SCREEN_H, 0);

            al_draw_rotated_bitmap(rocket, cx, cy,
                rocket_x + cx, rocket_y + cy, base_angle, flip_flag);

            al_flip_display();
        }
    }

    al_destroy_bitmap(rocket);
    al_destroy_bitmap(bg);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}