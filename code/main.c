#include <stdbool.h>
#include <stdio.h>

int tiles[64][2], validMoves[64][2];

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

void verifyMoves(int turn, char board[8][8]) {
  char currentPlayer = turn % 2 == 0 ? 'X' : 'O';
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
                  validMoves[count][0] = xAxis;
                  validMoves[count][1] = yAxis;

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
}

int main() {
  char board[8][8] = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                      {' ', ' ', ' ', 'X', 'O', ' ', ' ', ' '},
                      {' ', ' ', ' ', 'O', 'X', ' ', ' ', ' '},
                      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

  int linha, coluna, turn = 1;
  bool status;

  while (true) {
    scanf("%d %d", &linha, &coluna);

    linha--;
    coluna--;

    verifyMoves(turn, board);

    /*
    if(status){
            board[x][y] = turn % 2 == 0 ? 'O' : 'X';
    } else {
            printf("Invalid move");
    }
    */

    printf("\n    1 2 3 4 5 6 7 8\n");
    printf("\n   -----------------\n");

    for (int i = 0; i < 8; i++) {
      printf("%d  |", i + 1);
      for (int j = 0; j < 8; j++) {
        printf("%c|", board[i][j]);
      }
      printf("\n   -----------------\n");
    }
  }

  return 0;
}