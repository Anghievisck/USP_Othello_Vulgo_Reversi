#include <stdbool.h>
#include <stdio.h>

#define PlayerOne 'B'
#define PlayerTwo 'W'

int tiles[64][2], validMoves[64][6];

int findTiles(char currentPlayer, char board[8][8]) {
    int count = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == currentPlayer) {
                tiles[count][0] = i;
                tiles[count][1] = j;

                printf("x: %d y: %d\n", tiles[count][0] + 1, tiles[count][1] + 1);

                count++;
            }
        }
    }

    count++;

    return count;
}

int verifyMoves(char currentPlayer, char board[8][8]) {
    int tilesLength = findTiles(currentPlayer, board), count = 0;

    int directionX[] = {-1, -1, -1, 0, 0, 1, 1, 1},
        directionY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int j = 0; j < tilesLength; j++) {
        for (int i = 0; i < 8; i++) {
            int xAxis = tiles[j][0] + directionX[i];
            int yAxis = tiles[j][1] + directionY[i];

            while (true) {
                if (xAxis < 0 || yAxis < 0 || xAxis > 7 || yAxis > 7) {
                    break;
                } else {
                    if (board[xAxis][yAxis] == currentPlayer) {
                        break;
                    } else if (board[xAxis][yAxis] == ' ') {
                        break;
                    } else {
                        xAxis += directionX[i];
                        yAxis += directionY[i];
                        while (true) {
                            if (xAxis < 0 || yAxis < 0 || xAxis > 7 || yAxis > 7) {
                                break;
                            } else {
                                if (board[xAxis][yAxis] == ' ') {
                                    board[xAxis][yAxis] = 'X';

                                    validMoves[count][0] = xAxis;
                                    validMoves[count][1] = yAxis;

                                    validMoves[count][2] = tiles[j][0];
                                    validMoves[count][3] = tiles[j][1];

                                    validMoves[count][4] = directionX[i];
                                    validMoves[count][5] = directionY[i];

                                    count++;

                                    break;
                                } else if (board[xAxis][yAxis] == currentPlayer) {
                                    break;
                                } else {
                                    xAxis += directionX[i];
                                    yAxis += directionY[i];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return count;
}

void updateBoard(int posX, int posY, int originPos, char board[8][8], char currentPlayer){
    board[posX][posY] = currentPlayer;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j] == PlayerOne || board[i][j] == PlayerTwo){
            } else {
                board[i][j] = ' ';
            }
        }
    }

    for(int i = validMoves[originPos][2]; i < posX; i += validMoves[originPos][4]){
        for (int j = validMoves[originPos][3]; j < posY; j += validMoves[originPos][5]){
            board[i][j] = currentPlayer;
        }
    }
}

int main() {
    char board[8][8] = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', PlayerOne, PlayerTwo, ' ', ' ', ' '},
        {' ', ' ', ' ', PlayerTwo, PlayerOne, ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };

    int posX, posY, totalMoves, turn = 1;
    bool status = false, finished = true;

    while (true) {
        finished = true;
        char currentPlayer = turn % 2 == 0 ? PlayerTwo : PlayerOne;
        
        totalMoves = verifyMoves(turn, board);

        printf("\n    1 2 3 4 5 6 7 8\n");
        printf("\n   -----------------\n");

        for (int i = 0; i < 8; i++) {
            printf("%d  |", i + 1);
            for (int j = 0; j < 8; j++) {
                printf("%c|", board[i][j]);
            }
                printf("\n   -----------------\n");
        }

        if(totalMoves != 0){
            while(true){
                printf("Jogadas válidas:\n")
                for (int i = 0; i < totalMoves; i++){
                    printf("%d %d - ", validMoves[i][0], validMoves[i][1]);
                }

                printf("\n>>> ")

                scanf("%d %d", &posX, &posY);

                posX--;
                posY--;
                
                for(int i = 0; i < totalMoves; i++){
                    if(posX == validMoves[i][0] && posY == validMoves[i][1]){
                        updateBoard(posX, posY, i, board, currentPlayer);
                        turn++;
                        status = true;
                    }
                }

                if(status){
                    break;
                } else {
                    printf("Por favor, digite uma jogada válida");
                }
            }
        } else {
            printf("Sem jogadas possíveis, pulando a vez...\n");
        }

        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(board[i][j] == ' '){
                    finished = false;
                }
            }
        }

        if(finished){
            break;
        }
    }

    return 0;
}