#include <stdio.h>
#include <stdbool.h>

bool verifyMove(int turn, char board[8][8]){
    int tiles[64][2], count = 0;
    if (turn % 2 == 0){
        for (int i = 0; i < 8; i++) { 
            for (int j = 0; j < 8; j++) {  
                if(board[i][j] == 'O'){
                    tiles[count][0] = i;
                    tiles[count][1] = j;

                    printf("x: %d y: %d\n", tiles[count][0] + 1, tiles[count][1] + 1);

                    count ++;
                }
            }
        }
    } else if(turn % 2 != 0) {
        for (int i = 0; i < 8; i++) { 
            for (int j = 0; j < 8; j++) {  
                if(board[i][j] == 'X'){
                    tiles[count][0] = i;
                    tiles[count][1] = j;

                    printf("x: %d y: %d\n", tiles[count][0] + 1, tiles[count][1] + 1);

                    count ++;
                }
            }
        }
    }
    return false;
}

int main(){
    char board[8][8] = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'X', 'O', ' ', ' ', ' '},
        {' ', ' ', ' ', 'O', 'X', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };

    int x, y, turn = 1;
    bool status;

    while (true){
        scanf("%d %d", &x, &y);

        x--;
        y--;
        
        verifyMove(turn, board);

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