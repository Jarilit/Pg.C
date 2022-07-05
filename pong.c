// Copyrights 2022 cherellm
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define X_size 83
#define Y_size 27
#define ROCKET1_START_Y 19
#define ROCKET2_START_Y 5
#define ROCKET1_DEFAULT_X 8
#define ROCKET2_DEFAULT_X (X_size - 9)
#define WIN_SCORE 21
#define SPEED 10

int random_int();
int render_score(int x_size, int y_size, int *player_1_score, int *player_2_score);
int render(int x_size, int y_size, int rocket_1_y, int rocket_2_y, int ball_x, int ball_y);
void move_ball(int *ball_x, int *ball_y, int *vector_X, int *vector_Y, int rocket_1_y, int rocket_2_y);
int check_score(int *ball_x, int *ball_y, int *player_1_score, int *player_2_score, int *vector_X);
void move_rocket(int *rocketOne, int *rocketTwo);
void win_message(int winner);

int main(void) {
    int ball_x = 40;
    int ball_y = 13;
    int vector_X = 1;
    int vector_Y = 1;
    int rocket_1_y = 7;
    int rocket_2_y = 14;
    int player_1_score = 0;
    int player_2_score = 0;
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, 1);
    scrollok(stdscr, 1);
    while (1) {                     // Основной цикл обработки событий
        render_score(X_size, Y_size - 1, &player_1_score, &player_2_score);
        render(X_size, Y_size - 1, rocket_1_y, rocket_2_y, ball_x, ball_y);
        usleep(10000 * SPEED);              // Задержка вывода в терминал
        move_rocket(&rocket_1_y, &rocket_2_y);
        move_ball(&ball_x, &ball_y, &vector_X, &vector_Y, rocket_1_y, rocket_2_y);
        clear();
        if (check_score(&ball_x, &ball_y, &player_1_score, &player_2_score, &vector_X)) {
            clear();
            break;
        }
    }
    endwin();
    win_message(check_score(&ball_x, &ball_y, &player_1_score, &player_2_score, &vector_X));
}
// Рендеринг игрового поля
int render(int x_size, int y_size, int rocket_1_y, int rocket_2_y, int ball_x, int ball_y) {
    char wall = '#';
    char line = '|';
    char space = ' ';
    char racket = '#';
    char ball = '*';
    for (int j = 0; j <= y_size ; j++) {
        for (int i = 0; i <= x_size; i++) {
            if (((j == 0 || j == y_size) && (i != x_size)) || (i == 0) || i == x_size-1) {
                printw("%c", wall);
            } else if ((i == ROCKET1_DEFAULT_X && j == rocket_1_y) ||
                       (i == ROCKET2_DEFAULT_X && j == rocket_2_y)) {
                printw("%c", racket);
            } else if ((i == ROCKET1_DEFAULT_X && j == rocket_1_y + 1) ||
                      (i == ROCKET2_DEFAULT_X && j == rocket_2_y + 1)) {
               printw("%c", racket);
           } else if ((i == ROCKET1_DEFAULT_X && j == rocket_1_y + 2) ||
                      (i == ROCKET2_DEFAULT_X && j == rocket_2_y + 2)) {
               printw("%c", racket);
           } else if (i == ball_x && j == ball_y) {
               printw("%c", ball);
           } else if (i == x_size) {
                printw("\n");
            } else if (i == 41) {
                printw("%c", line);
            } else {
                printw("%c", space);
            }
        }
    }
    return 0;
}
// Движение ракеток по оси У
void move_rocket(int *rocketOne, int *rocketTwo) {
    char symbol = getch();
    if (symbol != -1) {
        switch (symbol) {
            case 'a':
                if (*rocketOne - 1 != 0) {
                    *rocketOne -= 1;
                }
                break;
            case 'z':
                if (*rocketOne + 3 != 26) {
                    *rocketOne += 1;
                }
                break;
            case 'k':
                if (*rocketTwo - 1 != 0) {
                    *rocketTwo -= 1;
                }
                break;
            case 'm':
                if (*rocketTwo + 3 != 26) {
                    *rocketTwo += 1;
                }
                break;
        }
    }
}
// Движение и отскок мяча
void move_ball(int *ball_x, int *ball_y, int *vector_X, int *vector_Y, int rocket_1_y, int rocket_2_y) {
    if (*ball_y == 1 || *ball_y == Y_size - 2) {
        *vector_Y = -(*vector_Y);
    }
    if (*ball_x == ROCKET1_DEFAULT_X + 1 && (*ball_y == rocket_1_y ||
                                             *ball_y == rocket_1_y + 1 ||
                                             *ball_y == rocket_1_y + 2)) {
        *vector_X = -(*vector_X);
    }
    if (*ball_x == ROCKET2_DEFAULT_X - 1 && (*ball_y == rocket_2_y ||
                                             *ball_y == rocket_2_y + 1 ||
                                             *ball_y == rocket_2_y + 2)) {
        *vector_X = -(*vector_X);
    }
    *ball_y += *vector_Y;
    *ball_x += *vector_X;
}
// Увеличение счета. Проверка на конец игры
int check_score(int *ball_x, int *ball_y, int *player_1_score, int *player_2_score, int *vector_X) {
    if (*ball_x < 2) {
        *player_2_score += 1;
        *ball_x = 41;
        *ball_y = random_int();
        *vector_X = -(*vector_X);
        return FALSE;
    }
    if (*ball_x > X_size - 4) {
        *player_1_score += 1;
        *ball_x = 41;
        *ball_y = random_int();
        *vector_X = -(*vector_X);
        return FALSE;
    }
    if (*player_1_score == WIN_SCORE) {
        return 1;
    } else if (*player_2_score == WIN_SCORE) {
        return 2;
    }
    return FALSE;
}
// Рендеринг счета игроков
int render_score(int x_size, int y_size, int *player_1_score, int *player_2_score) {
    char space = ' ';

    for (int j = 0; j <= 1; j++) {
        for (int i = 0; i <= x_size; i++) {
            if (((j == 0 || j == y_size) && (i != x_size)) || (i == 0) || i == x_size-1) {
                printw("%c", space);
            } else if (j == 1 && i == 2 && *player_1_score < 10) {
                printw("Player One:  %d", *player_1_score);
                i += 9;
            } else if (j == 1 && i == 2 && *player_1_score >= 10) {
                printw("Player One: %d", *player_1_score);
                i += 9;
            } else if (j == 1 && i == x_size - 20 && *player_2_score < 10) {
                printw("Player Two:  %d", *player_2_score);
                i = x_size -1;
            } else if (j == 1 && i == x_size - 20 && *player_2_score >= 10) {
                printw("Player Two: %d", *player_2_score);
                i = x_size - 2;
            } else if (i == x_size) {
                printw("\n");
            } else {
                printw("%c", space);
            }
        }
    }
    return 0;
}
// Сообщение о победе
int random_int() {
    int rnd;
    srand(time(NULL));
    rnd = rand()%100;
    while (rnd < 1 || rnd > (Y_size - 3)) {
        rnd = rand()%100;
    }
    return rnd;
}

void win_message(int winner) {
        if (winner == 1) {
            printf("\t\t\t█▀█ █░░ ▄▀█ █▄█ █▀▀ █▀█   █▀█ █▄░█ █▀▀\n");
            printf("\t\t\t█▀▀ █▄▄ █▀█ ░█░ ██▄ █▀▄   █▄█ █░▀█ ██▄\n\n");
            printf("\t\t\t/////░██╗░░░░░░░██╗██╗███╗░░██╗██╗/////\n");
            printf("\t\t\t/////░██║░░██╗░░██║██║████╗░██║██║/////\n");
            printf("\t\t\t/////░╚██╗████╗██╔╝██║██╔██╗██║██║/////\n");
            printf("\t\t\t/////░░████╔═████║░██║██║╚████║╚═╝/////\n");
            printf("\t\t\t/////░░╚██╔╝░╚██╔╝░██║██║░╚███║██╗/////\n");
            printf("\t\t\t/////░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚══╝╚═╝/////\n");
        } else {
            printf("\t\t\t█▀█ █░░ ▄▀█ █▄█ █▀▀ █▀█   ▀█▀ █░█░█ █▀█\n");
            printf("\t\t\t█▀▀ █▄▄ █▀█ ░█░ ██▄ █▀▄   ░█░ ▀▄▀▄▀ █▄█\n\n");
            printf("\t\t\t/////░██╗░░░░░░░██╗██╗███╗░░██╗██╗/////\n");
            printf("\t\t\t/////░██║░░██╗░░██║██║████╗░██║██║/////\n");
            printf("\t\t\t/////░╚██╗████╗██╔╝██║██╔██╗██║██║/////\n");
            printf("\t\t\t/////░░████╔═████║░██║██║╚████║╚═╝/////\n");
            printf("\t\t\t/////░░╚██╔╝░╚██╔╝░██║██║░╚███║██╗/////\n");
            printf("\t\t\t/////░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚══╝╚═╝/////\n");
        }
}
