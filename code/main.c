#include <stdbool.h>
#include <stdio.h>

#define HEIGHT 8 
#define LENGTH 8 

#define TOTALTILES HEIGHT*LENGTH

#define PLAYERONE 'B'
#define PLAYERTWO 'W'

typedef struct _validMoves{
    int validPos[64][2], affectedPos[64][3], totalMoves;
} validMoves;


typedef struct _othello {
    int tiles[64][2], tilesLength, totalPlayerOne, totalPlayerTwo;
    char board[HEIGHT][LENGTH];
    validMoves validMoves;
} othello;

othello initializeGame(){
    othello board;

    board.totalPlayerOne = 2;
    board.totalPlayerTwo = 2;

    for(int j = 0; j < LENGTH; j++){
        for(int i = 0; i < HEIGHT; i++){
            if(i == 3 && j == 3){
                board.board[i][j] = PLAYERONE;
            } else if(i == 3 && j == 4){
                board.board[i][j] = PLAYERTWO;
            } else if(i == 4 && j == 3){
                board.board[i][j] = PLAYERTWO;
            } else if(i == 4 && j == 4){
                board.board[i][j] = PLAYERONE;
            } else {
                board.board[i][j] = ' ';
            }
        }
    }

    return board;
}

othello initializeValidMoves(othello game){
    for(int i = 0; i < 64; i++){
      for(int j = 0; j < 3; j++){
            if(j < 2){
                game.validMoves.validPos[i][j] = 0;
            }
            game.validMoves.affectedPos[i][j] = 0;
        }
    }

    return game;
}

othello cleanBoard(othello game){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < LENGTH; j++){
            if(game.board[i][j] == PLAYERONE || game.board[i][j] == PLAYERTWO){
            } else {
                game.board[i][j] = ' ';
            }
        }
    }

    return game;
}

othello findTiles(char currentPlayer, othello game){
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 2; j++){
            game.tiles[i][j] = 0;
        }
    }

    game.tilesLength = 0;

    for(int i = 0; i < LENGTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            if(game.board[j][i] == currentPlayer){

                game.tiles[game.tilesLength][1] = i;
                game.tiles[game.tilesLength][0] = j;

                game.tilesLength++;
            }
        }
    }

    game.tilesLength++;

    return game;
}

othello verifyCurrentPos(int xAxis, int yAxis, int index, int i, char currentPlayer, othello game){
    int directionX[] = {-1, -1, -1, 0, 0, 1, 1, 1},
        directionY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int dirX, dirY;

    dirX = directionX[index];
    dirY = directionY[index];

    if(xAxis < 0 || yAxis < 0 || xAxis > 7 || yAxis > 7){
        return game;
    } else {
        if(game.board[yAxis][xAxis] == ' '){
            game.board[yAxis][xAxis] = 'X';

            printf("\ni(%d, %d) - o(%d, %d) a(%d, %d) s(%d, %d)\n", game.validMoves.totalMoves, i, game.tiles[i][1], game.tiles[i][0], xAxis, yAxis, dirX, dirY);

            game.validMoves.validPos[game.validMoves.totalMoves][0] = yAxis;
            game.validMoves.validPos[game.validMoves.totalMoves][1] = xAxis;

            game.validMoves.validPos[game.validMoves.totalMoves][2] = index;

            game.validMoves.totalMoves++;

            return game;
        } else if(game.board[yAxis][xAxis] == currentPlayer){
            return game;
        } else {
            yAxis += dirY;
            xAxis += dirX;

            return verifyCurrentPos(xAxis, yAxis, index, i, currentPlayer, game);
        }
    }
}

othello verifyMoves(char currentPlayer, othello game) {
    game = findTiles(currentPlayer, game);
    game.validMoves.totalMoves = 0;

    game = initializeValidMoves(game);

    int directionX[] = {-1, -1, -1, 0, 0, 1, 1, 1},
        directionY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(int j = 0; j < game.tilesLength; j++){
        for(int i = 0; i < 8; i++){
            int yAxis = game.tiles[j][0] + directionY[i];
            int xAxis = game.tiles[j][1] + directionX[i];

            if(xAxis < 0 || yAxis < 0 || xAxis > 7 || yAxis > 7 || game.board[yAxis][xAxis] == currentPlayer ||
                game.board[yAxis][xAxis] == 'X' || game.board[yAxis][xAxis] == ' '){
            } else {
                yAxis += directionY[i];
                xAxis += directionX[i];

                game = verifyCurrentPos(xAxis, yAxis, i, j, currentPlayer, game);
            }
        }
    }

    return game;
}

othello updateBoard(int posX, int posY, int originalPos, char currentPlayer, othello game){
    int currentX = game.tiles[originalPos][1],
        currentY = game.tiles[originalPos][0];

    printf("I recebido: %d", originalPos);
    int directionX[] = {-1, -1, -1, 0, 0, 1, 1, 1},
        directionY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int i = game.validMoves.validPos[originalPos][2];
    printf("\nindex: %d", game.validMoves.validPos[originalPos][2]);

    game.board[posY][posX] = currentPlayer;

    game = cleanBoard(game);

    printf("\ncurrentX: %d currentY: %d\nposX: %d posY: %d\n", currentX, currentY, posX, posY);

    game.board[currentY][currentX] = currentPlayer;

    currentX += directionX[i];
    currentY += directionY[i];

    printf("\ncurrentX: %d currentY: %d\nposX: %d posY: %d\n", currentX, currentY, posX, posY);

    game.board[currentY][currentX] = currentPlayer;

    return game;
}

othello getScore(othello game){
    game.totalPlayerOne = 0; 
    game.totalPlayerTwo = 0;

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < LENGTH; j++){
            if(game.board[i][j] == PLAYERONE){
                game.totalPlayerOne++;
            } else if(game.board[i][j] == PLAYERTWO){
                game.totalPlayerTwo++;
            }
        }
    }

    return game;
}

void printBoard(othello game){
    printf("Jogador 1: %d\nJogador 2: %d\n\n", game.totalPlayerOne, game.totalPlayerTwo);

    printf("\n    1 2 3 4 5 6 7 8\n");
    printf("\n   -----------------\n");

    for(int i = 0; i < HEIGHT; i++){
        printf("%d  |", i + 1);
        for(int j = 0; j < LENGTH; j++){
            printf("%c|", game.board[i][j]);
        }
            printf("\n   -----------------\n");
    }
}

void showWinner(othello game){
    printBoard(game);

    if(game.totalPlayerOne > game.totalPlayerTwo){
        printf("Jogador 1 ganhou!!!");
    } else if(game.totalPlayerTwo > game.totalPlayerOne){
        printf("Jogador 2 ganhou!!!");
    } else {
        printf("Empate!!!");
    }
}

int main() {
    int posX, posY, turn = 1;
    bool status = false, finished = true;

    othello game = initializeGame();

    while(true){
        finished = true;
        char currentPlayer = turn % 2 == 0 ? PLAYERTWO : PLAYERONE;

        game = verifyMoves(currentPlayer, game);
        printBoard(game);

        if(game.validMoves.totalMoves != 0){
            while(true){
                status = false;

                printf("Jogadas validas:\n| ");
                for(int i = 0; i < game.validMoves.totalMoves; i++){
                    printf("x: %d y: %d | ", game.validMoves.validPos[i][1]+1, game.validMoves.validPos[i][0]+1);
                }

                printf("\n>>> ");

                scanf("%d %d", &posX, &posY);

                posX--;
                posY--;
                
                for(int i = 0; i < game.validMoves.totalMoves; i++){
                    if(posX == game.validMoves.validPos[i][1] && posY == game.validMoves.validPos[i][0]){
                        printf("\nI passado: %d\n", i);
                        game = updateBoard(posX, posY, i, currentPlayer, game);
                        game = getScore(game);

                        status = true;
                    }
                }

                if(status){
                    turn++;
                    break;
                } else {
                    printf("Por favor, digite uma jogada valida\n\n");
                }
            }
        } else {
            printf("Sem jogadas possiveis, pulando a vez...\n");
            turn++;
        }

        for(int i = 0; i < HEIGHT; i++){
            for(int j = 0; j < LENGTH; j++){
                if(game.board[i][j] == ' ' || game.board[i][j] == 'X'){
                    finished = false;
                    break;
                }
            }
        }

        if(finished){
            showWinner(game);
            break;
        }
    }

    return 0;
}