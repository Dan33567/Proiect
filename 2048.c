#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define SIZE 4
#define CELL_WIDTH 7   // latime celula
#define CELL_HEIGHT 3  // inaltime celula

int grid[SIZE][SIZE];
int score = 0; // scor global

// Prototipurile functii
void spawn_tile();
void init_game();
void draw_grid(int start_y, int start_x);
void draw_game(int start_y, int start_x);
int move_left();
int move_right();
int move_up();
int move_down();
int can_move();

// --- Functia spawn_tile ---
// Adauga un nou tile (2 sau 4) intr-o celula goala.
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
        grid[i][j] = (rand() % 10 == 0) ? 4 : 2;  // 10% șansă pentru 4, altfel 2
    }
}

// --- Functia init_game ---
// Initializează jocul: seteaza toate celulele la 0,
// reseteaza scorul si plasează primele doua tile-uri.
void init_game() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = 0;
    score = 0;
    spawn_tile();
    spawn_tile();
}

// --- Functia draw_grid ---
// Deseneaza grila de baza
void draw_grid(int start_y, int start_x) {
    int i, j, k, l;
    // Desenam liniile orizontale și intersectiile
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

            // Completați linia orizontală între intersecții
            if (j < SIZE) {
                for (k = 0; k < CELL_WIDTH - 1; k++) {
                    addch(ACS_HLINE);
                }
            }
        }
    }

    // Desenăm liniile verticale și completăm interiorul celulelor cu spații
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

// --- Functia draw_game ---
// Deseneaza starea actuala a jocului: grila si numerele din celule.
// Afiseaza ai scorul in dreapta grilei.
void draw_game(int start_y, int start_x) {
    draw_grid(start_y, start_x);
    // Afisarea numerelor in celule
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] != 0) {
                char buf[16];
                sprintf(buf, "%d", grid[i][j]);
                int len = strlen(buf);
                // Centrarea numarului in celula
                int y = start_y + i * CELL_HEIGHT + CELL_HEIGHT / 2;
                int x = start_x + j * CELL_WIDTH + (CELL_WIDTH - len) / 2;
                mvprintw(y, x, "%s", buf);
            }
        }
    }
    // afiseaza scorul în dreapta grilei.
    // Calculam pozitia astfel incat scorul sa fie pe acelasi rand.
    mvprintw(start_y, start_x + SIZE * CELL_WIDTH + 2, "Scor: %d", score);
}

// --- Functia move_left ---
// Mutt tile-urile spre stanga și combina-le conform regulilor jocului.
// Actualizează si scorul la combinare.
int move_left() {
    int moved = 0;
    for (int i = 0; i < SIZE; i++) {
        int target = 0;
        int lastMerged = -1;
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] != 0) {
                if (target > 0 && grid[i][target - 1] == grid[i][j] && lastMerged != target - 1) {
                    grid[i][target - 1] *= 2;         // Combină tile-uri
                    score += grid[i][target - 1];       // Adaugă la scor valoarea nouă
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

// --- Functia move_right ---
// Muta tile-urile spre dreapta și combina-le.
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

// --- Functia move_up ---
// Muta tile-urile in sus si le combina
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

// --- Functia move_down ---
// Muta tile-urile în jos si combina-le.
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

// --- Functia can_move ---
// Verifica daca mai exista cel putin o mutare posibila.
int can_move() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0)
                return 1;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if ((j < SIZE - 1 && grid[i][j] == grid[i][j + 1]) ||
                (i < SIZE - 1 && grid[i][j] == grid[i + 1][j]))
                return 1;
        }
    }
    return 0;
}

// --- Functia main ---

int main() {
    srand(time(NULL)); 
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);   // Permite captarea tastelor speciale (de ex. sageti)

    init_game();

    int ch;
    int moved;
    while (1) {
        clear();
        draw_game(1, 2);   // Desenează jocul începând de la poziția (1,2)
        refresh();

        if (!can_move()) {
            mvprintw(0, 2, "Game Over! Apasa orice tasta pentru a iesi.");
            refresh();
            getch();
            break;
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
        // Dacă s-a produs o mutare, se generează un nou tile.
        if (moved)
            spawn_tile();
    }

    endwin();
    return 0;
}
