#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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

void place_ship(Player* player, int x, int y, char direction, int ship_size, int ship_type) {
    Ship* ship = &player->ships[0];
    ship->x = x;
    ship->y = y;
    ship->size = ship_size;
    ship->direction = direction;
    ship->tiles = malloc(sizeof(Tile*) * ship_size);

    for (int i = 0; i < ship_size; i++) {
        int x = ship->x + (direction == 'H' ? 0 : i);
        int y = ship->y + (direction == 'V' ? 0 : i);

        ship->tiles[i] = &player->board[x][y];
        player->board[x][y].hasBoat = true;
        player->board[x][y].shipType = ship_type;
        
    }
}

void print_board(Tile board[BOARD_SIZE][BOARD_SIZE]) {
    printf("  0 1 2 3 4 5 6 7 8 9 \n");
    printf(" ┌───────────────────┐\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c│", i + 65);
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].hasBoat) {
                printf("%c", board[i][j].shipType);
            } else {
                printf("-");
            }
            if (j != BOARD_SIZE - 1) {
                printf(" "); 
            }
        }
        printf("│\n");
    }
    printf(" └───────────────────┘\n");
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

int main() {
    Player player1;
    Player player2;

    // initialize the boards
    inititalize_board(player1.board);
    inititalize_board(player2.board);

    // place the ships on the boards
    for (int i = 0; i < NUM_SHIPS_PER_PLAYER; i++) {
        print_board(player1.board);
        int x, y;
        char direction;
        get_user_input(&player1, &x, &y, &direction, ship_sizes[i]);
        place_ship(&player1, x, y, direction, ship_sizes[i], ship_chars[i]);
    }

    for (int i = 0; i < NUM_SHIPS_PER_PLAYER; i++) {
        int x, y;
        char direction;
        ai_placement(&player2, &x, &y, &direction, ship_sizes[i]);
        place_ship(&player2, x, y, direction, ship_sizes[i], ship_chars[i]);
    }
    
    // Print the board
    print_board(player1.board);
    print_board(player2.board);

    return 0;
}