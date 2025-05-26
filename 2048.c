#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#define SIZE 4
#define CELL_WIDTH 7
#define CELL_HEIGHT 3

int grid[SIZE][SIZE];
int score = 0;

void spawn_tile();
void init_game();
void draw_grid(int start_y, int start_x);
void draw_game(int start_y, int start_x, int key_pressed);
void draw_joystick(int start_y, int start_x, int key_pressed);
int move_left();
int move_right();
int move_up();
int move_down();
int can_move();

void spawn_tile() {
    int empty_cells[SIZE * SIZE][2];
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) {
                empty_cells[count][0] = i;
                empty_cells[count][1] = j;
                count++;
            }
        }
    }
    if (count > 0) {
        int r = rand() % count;
        int i = empty_cells[r][0];
        int j = empty_cells[r][1];
        grid[i][j] = (rand() % 10 == 0) ? 4 : 2;
    }
}

void init_game() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = 0;
    score = 0;
    spawn_tile();
    spawn_tile();
}

void draw_grid(int start_y, int start_x) {
    int i, j, k, l;
    for (i = 0; i <= SIZE; i++) {
        for (j = 0; j <= SIZE; j++) {
            move(start_y + i * CELL_HEIGHT, start_x + j * CELL_WIDTH);
            if (i == 0 && j == 0)
                addch(ACS_ULCORNER);
            else if (i == 0 && j == SIZE)
                addch(ACS_URCORNER);
            else if (i == SIZE && j == 0)
                addch(ACS_LLCORNER);
            else if (i == SIZE && j == SIZE)
                addch(ACS_LRCORNER);
            else if (i == 0)
                addch(ACS_TTEE);
            else if (i == SIZE)
                addch(ACS_BTEE);
            else if (j == 0)
                addch(ACS_LTEE);
            else if (j == SIZE)
                addch(ACS_RTEE);
            else
                addch(ACS_PLUS);

            if (j < SIZE) {
                for (k = 0; k < CELL_WIDTH - 1; k++) {
                    addch(ACS_HLINE);
                }
            }
        }
    }

    for (i = 0; i < SIZE; i++) {
        for (l = 1; l < CELL_HEIGHT; l++) {
            for (j = 0; j <= SIZE; j++) {
                move(start_y + i * CELL_HEIGHT + l, start_x + j * CELL_WIDTH);
                addch(ACS_VLINE);
                if (j < SIZE) {
                    for (k = 0; k < CELL_WIDTH - 1; k++)
                        addch(' ');
                }
            }
        }
    }
}

void draw_game(int start_y, int start_x, int key_pressed) {
    draw_grid(start_y, start_x);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] != 0) {
                char buf[16];
                sprintf(buf, "%d", grid[i][j]);
               

                int cell_y = start_y + i * CELL_HEIGHT;
                int cell_x = start_x + j * CELL_WIDTH;

                // Centrare perfectă
		int center_y = cell_y + (CELL_HEIGHT) / 2 ;
		int center_x = cell_x + (CELL_WIDTH ) / 2 ;

                int color = 1;
                int value = grid[i][j];
                switch (value) {
                 case 2: color = 1; break;
		case 4: color = 2; break;
		case 8: color = 3; break;
		case 16: color = 4; break;
		case 32: color = 5; break;
		case 64: color = 6; break;
		case 128: color = 7; break;
		case 256: color = 8; break;
		case 512: color = 9; break;
		case 1024: color = 10; break;
		case 2048: color = 11; break;
		default: color = 12; break;
                }

                attron(COLOR_PAIR(color) | A_BOLD);
                mvprintw(center_y, center_x, "%s", buf);
                attroff(COLOR_PAIR(color) | A_BOLD);
            }
        }
    }
    mvprintw(start_y + SIZE * CELL_HEIGHT + 1, start_x, "Scor: %d", score);
    draw_joystick(start_y + 2, start_x + SIZE * CELL_WIDTH + 8, key_pressed);
}

void draw_joystick(int start_y, int start_x, int key_pressed) {
    int highlight_up = (key_pressed == KEY_UP);
    int highlight_down = (key_pressed == KEY_DOWN);
    int highlight_left = (key_pressed == KEY_LEFT);
    int highlight_right = (key_pressed == KEY_RIGHT);

    move(start_y, start_x + 4);
    if (highlight_up) attron(A_REVERSE);
    printw("[^]");
    if (highlight_up) attroff(A_REVERSE);

    move(start_y + 1, start_x);
    if (highlight_left) attron(A_REVERSE);
    printw("[<]");
    if (highlight_left) attroff(A_REVERSE);

    move(start_y + 1, start_x + 4);
    if (highlight_down) attron(A_REVERSE);
    printw("[v]");
    if (highlight_down) attroff(A_REVERSE);

    move(start_y + 1, start_x + 8);
    if (highlight_right) attron(A_REVERSE);
    printw("[>]");
    if (highlight_right) attroff(A_REVERSE);
}

int move_left() {
    int moved = 0;
    for (int i = 0; i < SIZE; i++) {
        int target = 0;
        int lastMerged = -1;
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] != 0) {
                if (target > 0 && grid[i][target - 1] == grid[i][j] && lastMerged != target - 1) {
                    grid[i][target - 1] *= 2;
                    score += grid[i][target - 1];
                    grid[i][j] = 0;
                    lastMerged = target - 1;
                    moved = 1;
                } else {
                    if (j != target) {
                        grid[i][target] = grid[i][j];
                        grid[i][j] = 0;
                        moved = 1;
                    }
                    target++;
                }
            }
        }
    }
    return moved;
}

int move_right() {
    int moved = 0;
    for (int i = 0; i < SIZE; i++) {
        int target = SIZE - 1;
        int lastMerged = -1;
        for (int j = SIZE - 1; j >= 0; j--) {
            if (grid[i][j] != 0) {
                if (target < SIZE - 1 && grid[i][target + 1] == grid[i][j] && lastMerged != target + 1) {
                    grid[i][target + 1] *= 2;
                    score += grid[i][target + 1];
                    grid[i][j] = 0;
                    lastMerged = target + 1;
                    moved = 1;
                } else {
                    if (j != target) {
                        grid[i][target] = grid[i][j];
                        grid[i][j] = 0;
                        moved = 1;
                    }
                    target--;
                }
            }
        }
    }
    return moved;
}

int move_up() {
    int moved = 0;
    for (int j = 0; j < SIZE; j++) {
        int target = 0;
        int lastMerged = -1;
        for (int i = 0; i < SIZE; i++) {
            if (grid[i][j] != 0) {
                if (target > 0 && grid[target - 1][j] == grid[i][j] && lastMerged != target - 1) {
                    grid[target - 1][j] *= 2;
                    score += grid[target - 1][j];
                    grid[i][j] = 0;
                    lastMerged = target - 1;
                    moved = 1;
                } else {
                    if (i != target) {
                        grid[target][j] = grid[i][j];
                        grid[i][j] = 0;
                        moved = 1;
                    }
                    target++;
                }
            }
        }
    }
    return moved;
}

int move_down() {
    int moved = 0;
    for (int j = 0; j < SIZE; j++) {
        int target = SIZE - 1;
        int lastMerged = -1;
        for (int i = SIZE - 1; i >= 0; i--) {
            if (grid[i][j] != 0) {
                if (target < SIZE - 1 && grid[target + 1][j] == grid[i][j] && lastMerged != target + 1) {
                    grid[target + 1][j] *= 2;
                    score += grid[target + 1][j];
                    grid[i][j] = 0;
                    lastMerged = target + 1;
                    moved = 1;
                } else {
                    if (i != target) {
                        grid[target][j] = grid[i][j];
                        grid[i][j] = 0;
                        moved = 1;
                    }
                    target--;
                }
            }
        }
    }
    return moved;
}

int can_move() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (grid[i][j] == 0)
                return 1;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if ((j < SIZE - 1 && grid[i][j] == grid[i][j + 1]) ||
                (i < SIZE - 1 && grid[i][j] == grid[i + 1][j]))
                return 1;

    return 0;
}

int main() {
    srand(time(NULL));
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);     // 2
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);    // 4
    init_pair(3, COLOR_GREEN, COLOR_BLACK);     // 8
    init_pair(4, COLOR_CYAN, COLOR_BLACK);      // 16
    init_pair(5, COLOR_BLUE, COLOR_BLACK);      // 32
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);   // 64
    init_pair(7, COLOR_RED, COLOR_BLACK);       // 128
    init_pair(8, COLOR_WHITE, COLOR_RED);       // 256
    init_pair(9, COLOR_BLACK, COLOR_YELLOW);    // 512
    init_pair(10, COLOR_BLACK, COLOR_GREEN);    // 1024
    init_pair(11, COLOR_BLACK, COLOR_CYAN);     // 2048
    init_pair(12, COLOR_BLACK, COLOR_MAGENTA);  // >2048
    init_game();

    int ch = 0;
    int moved;

    while (1) {
        clear();

        int term_height, term_width;
        getmaxyx(stdscr, term_height, term_width);
        int grid_width = CELL_WIDTH * SIZE;
        int grid_height = CELL_HEIGHT * SIZE;
        int start_x = (term_width - grid_width) / 2;
        int start_y = (term_height - grid_height) / 2;

        draw_game(start_y, start_x, ch);
        refresh();

        if (!can_move()) {
            mvprintw(0, 2, "Game Over! Apasa Q pentru a iesi sau R pentru restart.");
            refresh();
            while (1) {
                int end_ch = getch();
                if (end_ch == 'q' || end_ch == 'Q') {
                    endwin();
                    return 0;
                } else if (end_ch == 'r' || end_ch == 'R') {
                    init_game();
                    ch = 0;
                    break;
                }
            }
            continue;
        }

        ch = getch();
        moved = 0;
        switch (ch) {
            case KEY_LEFT:
                moved = move_left();
                break;
            case KEY_RIGHT:
                moved = move_right();
                break;
            case KEY_UP:
                moved = move_up();
                break;
            case KEY_DOWN:
                moved = move_down();
                break;
            case 'q':
            case 'Q':
                endwin();
                return 0;
        }
        if (moved) {
            usleep(40000);
            spawn_tile();
        }
    }

    endwin();
    return 0;
}
