#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>

#define BOARD_SIZE 10
#define NUM_SHIPS_PER_PLAYER 5

enum Ships{
    CARRIER,
    BATTLESHIP,
    DESTROYER,
    SUBMARINE,
    PATROLBOAT,
};

const int ship_sizes[5] = {5, 4, 3, 3, 2};
const char ship_chars[5] = {'C', 'B', 'D', 'S', 'P'};

typedef struct {
    bool hasBoat;
    bool isHit;
    char shipType;
} Tile;

typedef struct {
    int x;
    int y;
    int size;
    char direction; // 'H' for horizontal, 'V' for vertical
    Tile** tiles; // Array of pointers to Tile structs
} Ship;

typedef struct {
    Ship ships[NUM_SHIPS_PER_PLAYER];
    Tile board[BOARD_SIZE][BOARD_SIZE];
} Player;

void inititalize_board(Tile board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].hasBoat = false;
            board[i][j].isHit = false;
            board[i][j].shipType = '\0';
        }
    }
}

void place_ship(Player* player, int x, int y, char direction, int ship_type) {
    Ship* ship = &player->ships[ship_type];
void place_ship(Player* player, int x, int y, char direction, int ship_type) {
    Ship* ship = &player->ships[ship_type];
    ship->x = x;
    ship->y = y;
    ship->size = ship_sizes[ship_type];
    ship->size = ship_sizes[ship_type];
    ship->direction = direction;
    ship->tiles = malloc(sizeof(Tile*) * ship_sizes[ship_type]);
    ship->tiles = malloc(sizeof(Tile*) * ship_sizes[ship_type]);

    for (int i = 0; i < ship_sizes[ship_type]; i++) {
    for (int i = 0; i < ship_sizes[ship_type]; i++) {
        int x = ship->x + (direction == 'H' ? 0 : i);
        int y = ship->y + (direction == 'V' ? 0 : i);

        ship->tiles[i] = &player->board[x][y];
        player->board[x][y].hasBoat = true;
        player->board[x][y].shipType = ship_chars[ship_type];
        player->board[x][y].shipType = ship_chars[ship_type];
        
    }
}

void print_board(Tile board[BOARD_SIZE][BOARD_SIZE], Tile enemy_board[BOARD_SIZE][BOARD_SIZE]) {
    printf("    Player 1's board      Player 2's board\n");
    printf("  0 1 2 3 4 5 6 7 8 9   0 1 2 3 4 5 6 7 8 9 \n");
    printf(" ┌───────────────────┐ ┌───────────────────┐\n");
void print_board(Tile board[BOARD_SIZE][BOARD_SIZE], Tile enemy_board[BOARD_SIZE][BOARD_SIZE]) {
    printf("    Player 1's board      Player 2's board\n");
    printf("  0 1 2 3 4 5 6 7 8 9   0 1 2 3 4 5 6 7 8 9 \n");
    printf(" ┌───────────────────┐ ┌───────────────────┐\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c│", i + 65);  
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].isHit) {
                if (board[i][j].hasBoat) {
                    printf("X");
                } else {
                    printf("O");
                }
                } else {
                    if (board[i][j].hasBoat) {
                        printf("%c", board[i][j].shipType);
                    } else {
                        printf("-");
                    }
                }
            if (j != BOARD_SIZE - 1) {
                printf(" "); 
            }
        }
        printf("│%c│", i + 65);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (enemy_board[i][j].isHit) {
                if (enemy_board[i][j].hasBoat) {
                    printf("X");
                } else {
                    printf("O");
                }
                } else {
                    printf("-");
                }
            if (board[i][j].isHit) {
                if (board[i][j].hasBoat) {
                    printf("X");
                } else {
                    printf("O");
                }
                } else {
                    if (board[i][j].hasBoat) {
                        printf("%c", board[i][j].shipType);
                    } else {
                        printf("-");
                    }
                }
            if (j != BOARD_SIZE - 1) {
                printf(" "); 
            }
        }
        printf("│%c│", i + 65);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (enemy_board[i][j].isHit) {
                if (enemy_board[i][j].hasBoat) {
                    printf("X");
                } else {
                    printf("O");
                }
                } else {
                    printf("-");
                }
            if (j != BOARD_SIZE - 1) {
                printf(" "); 
            }
        }
        printf("│\n");
    }
    printf(" └───────────────────┘ └───────────────────┘\n");
    printf("Ships: C = Carrier, B = Battleship, D = Destroyer, S = Submarine, P = Patrolboat\n");
    printf("Legend: X = Hit, O = Miss, - = Empty\n");
    printf(" └───────────────────┘ └───────────────────┘\n");
    printf("Ships: C = Carrier, B = Battleship, D = Destroyer, S = Submarine, P = Patrolboat\n");
    printf("Legend: X = Hit, O = Miss, - = Empty\n");
}

bool validate_placement(Player* player, int x, int y, char direction, int ship_size) {
    
    switch (direction)
    {
    case 'V':
        if (x + ship_size > BOARD_SIZE) {
            return false;
        }
        
        for (int i = 0; i < ship_size; i++) {
            if (player->board[x + i][y].hasBoat) {
                return false;
            }
        }

        break;
    
    case 'H':
        if (y + ship_size > BOARD_SIZE) {
            return false;
        }

        for (int i = 0; i < ship_size; i++) {
            if (player->board[x][y + i].hasBoat) {
                return false;
            }
        }
        
        break;

    default:
        break;
    }

    return true;
}

void get_user_input(Player* player, int* x, int* y, char* direction, int ship_size) {
    do {
        char c;
        int i;
        char dir;

        do {
            printf("Enter A letter and a number to place the start of your ship (e.g. A0): ");
            if (scanf(" %c%d", &c, &i) != 2 || c < 'A' || c > 'J' || i < 0 || i > 9) {
                printf("Invalid input. Please try again.\n");
                while (getchar() != '\n'); // Clear input buffer
            } else {
                break; // Exit the loop if input is valid
            }
        } while (1);

        do {
            printf("Enter direction (H or V) for the ship: ");
            if (scanf(" %c", &dir) != 1 || (dir != 'H' && dir != 'V')) {
                printf("Invalid input. Please try again.\n");
                while (getchar() != '\n'); // Clear input buffer
            } else {
                break; // Exit the loop if input is valid
            }
        } while (1);

        if (validate_placement(player, c - 'A', i, dir, ship_size)) {
            *x = c - 'A'; // Convert letter to integer
            *y = i;
            *direction = dir;
            break;
        } else {
            printf("Invalid placement. Please try again.\n");
        }

    } while (1);
}

void ai_placement(Player* player, int* x, int* y, char* direction, int ship_size) {
    srand(time(0));

    do {
        *x = rand() % BOARD_SIZE;
        *y = rand() % BOARD_SIZE;
        *direction = rand() % 2 == 0 ? 'H' : 'V';
    } while (!validate_placement(player, *x, *y, *direction, ship_size));
}

void do_shot(Player* player, int x, int y) {
    if (player->board[x][y].hasBoat) {
        printf("Hit!\n");
    } else {
        printf("Miss!\n");
    }
    player->board[x][y].isHit = true;
}

void get_user_shot(Player* player) {
    do {
        char x;
        int y;

        printf("Enter A letter and a number to shoot (e.g. A0): ");
        if (scanf(" %c%d", &x, &y) != 2 || x < 'A' || x > 'J' || y < 0 || y > 9) {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
        } else {
            do_shot(player, x - 'A', y);
            break; // Exit the loop if input is valid
        }
    } while (1);
}

void get_ai_shot(Player* player) {
    srand(time(0));
    int x, y;
    x = rand() % BOARD_SIZE;
    y = rand() % BOARD_SIZE;

    do_shot(player, x, y);
}

bool check_game_over(Player* player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (!player->board[i][j].isHit && player->board[i][j].hasBoat) {
                return false;
            }
        }
    }
    return true;
}

int check_if_ships_sunk(Player* player) {
    for (int i = 0; i < NUM_SHIPS_PER_PLAYER; i++) {
        for (int j = 0; j < ship_sizes[i]; j++) {
            if (!player->ships[i].tiles[j]->isHit) {
                break;
            }
            return i;
        }
    }
    return -1;
}

void do_shot(Player* player, int x, int y) {
    if (player->board[x][y].hasBoat) {
        printf("Hit!\n");
    } else {
        printf("Miss!\n");
    }
    player->board[x][y].isHit = true;
}

void get_user_shot(Player* player) {
    do {
        char x;
        int y;

        printf("Enter A letter and a number to shoot (e.g. A0): ");
        if (scanf(" %c%d", &x, &y) != 2 || x < 'A' || x > 'J' || y < 0 || y > 9) {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
        } else {
            do_shot(player, x - 'A', y);
            break; // Exit the loop if input is valid
        }
    } while (1);
}

void get_ai_shot(Player* player) {
    srand(time(0));
    int x, y;
    x = rand() % BOARD_SIZE;
    y = rand() % BOARD_SIZE;

    do_shot(player, x, y);
}

bool check_game_over(Player* player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (!player->board[i][j].isHit && player->board[i][j].hasBoat) {
                return false;
            }
        }
    }
    return true;
}

int check_if_ships_sunk(Player* player) {
    for (int i = 0; i < NUM_SHIPS_PER_PLAYER; i++) {
        for (int j = 0; j < ship_sizes[i]; j++) {
            if (!player->ships[i].tiles[j]->isHit) {
                break;
            }
            return i;
        }
    }
    return -1;
}

int main() {
    Player player1;
    Player player2;

    // initialize the boards
    inititalize_board(player1.board);
    inititalize_board(player2.board);

    // place the ships on the boards
    for (int i = 0; i < NUM_SHIPS_PER_PLAYER; i++) {
        print_board(player1.board, player2.board);
        print_board(player1.board, player2.board);
        int x, y;
        char direction;
        get_user_input(&player1, &x, &y, &direction, ship_sizes[i]);
        place_ship(&player1, x, y, direction, i);
        system("cls");
    }

    for (int i = 0; i < NUM_SHIPS_PER_PLAYER; i++) {
        int x, y;
        char direction;
        ai_placement(&player2, &x, &y, &direction, ship_sizes[i]);
        place_ship(&player2, x, y, direction, i);
    }
    
    // Game loop
    while (true) {
        // Player 1's turn
        print_board(player1.board, player2.board);
        get_user_shot(&player2);
        system("cls");
        int sunk_ship = check_if_ships_sunk(&player2);
        if (sunk_ship != -1) {
            printf("Ship %c sunk!\n", ship_chars[sunk_ship]);
        }
        if (check_game_over(&player2)) {
            printf("Player 1 wins!\n");
            break;
        }
        // Player 2's turn
        get_ai_shot(&player1);
        sunk_ship = check_if_ships_sunk(&player1);
        if (sunk_ship != -1) {
            printf("Ship %c sunk!\n", ship_chars[sunk_ship]);
        }
        if (check_game_over(&player1)) {
            printf("Player 2 wins!\n");
            break;
        }
    }
    return 0;
}