/*
    Integrantes do Grupo 13:
        Israel Augusto Ribeiro Santos - 15553885 
        Mauro Henrique Brandão dos Santos - 15479896
        Pedro Luís Anghievisck - 15656521
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Cria constantes contendo a altura e altura do tabuleiro
#define HEIGHT 8 
#define LENGTH 8 

#define TOTALTILES HEIGHT*LENGTH

// Define os caracteres dos jogadores 1 e 2
#define PLAYERONE 'B'
#define PLAYERTWO 'W'

// Cria structs para facilitar a passagem de variaveis
typedef struct _validMoves{
    int validPos[TOTALTILES][2], affectedPos[TOTALTILES][2], totalMoves;
} validMoves;

/*
    Para facilitar a visualização e debug dentro do código, uso uma struct dentro de outra, relacionando 
    os movimentos válidos a struct do jogo em si
*/
typedef struct _othello{
    int tiles[TOTALTILES][2], tilesLength, totalPlayerOne, totalPlayerTwo;
    char board[HEIGHT][LENGTH];
    bool status;
    validMoves validMoves;
} othello;

// Função criada para filtrar a entrada inserida pelo usuário, ele sempre retornará um inteiro menor que 10 ou -1
int getInt(){
    int input;
    char cInput[2];

    scanf("%s", &cInput);
    if(cInput[0] == '1' && cInput[1] == '3'){
        printf("\n\nFAZ O L\n\n");
        input = -1;
    } else if(isdigit(cInput[0])){
        input = cInput[0] - '0';
    } else {
        input = -1;
    }

    return input;
}

// Inicia o tabuleiro com as posições iniciais
othello initializeGame(){
    othello board;

    board.totalPlayerOne = 2;
    board.totalPlayerTwo = 2;

    board.status = false;

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

// Inicializa os vetores usados para 0, podendo usar isso para validações futuras
othello initializeValidMoves(othello game){
    for(int i = 0; i < TOTALTILES; i++){
      for(int j = 0; j < 3; j++){
            if(j < 2){
                game.validMoves.validPos[i][j] = 0;
            }
            game.validMoves.affectedPos[i][j] = 0;
        }
    }

    return game;
}

// Limpa as marcas de jogadas possíveis
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

// Encontra o total de quadrados ocupados pela peça do jogador atual
othello findTiles(char currentPlayer, othello game){
    // Zera todos os quadrados para não haver erros
    for(int i = 0; i < TOTALTILES; i++){
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

// Valida se há jogas validas em determinada posição e direção
othello verifyCurrentPos(int xAxis, int yAxis, int index, int i, char currentPlayer, othello game){
    // Dois vetores, para que eu possa usar um único for e verificar qualquer direção dentre as 8 possíveis
    int directionX[] = {-1, -1, -1, 0, 0, 1, 1, 1},
        directionY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int dirX, dirY;

    dirX = directionX[index];
    dirY = directionY[index];

    // Basicamente, se estamo verificando fora do tabuleiro, ele deve parar
    if(xAxis < 0 || yAxis < 0 || xAxis > 7 || yAxis > 7){
        return game;
    } else {
        /*
            Ao encontrar um quadrado em branco, visto que anteriormente (na outra função) achamos uma peça
            inimiga, ele marca como posição valida e salva as variáveis necessárias para o funcionamento do
            código
        */
        if(game.board[yAxis][xAxis] == ' '){
            game.board[yAxis][xAxis] = '*';

            game.validMoves.validPos[game.validMoves.totalMoves][0] = yAxis;
            game.validMoves.validPos[game.validMoves.totalMoves][1] = xAxis;

            game.validMoves.totalMoves++;

            return game;
        // Se achar uma peça do jogador atual, ou um X, então a jogada é inválida
        } else if(game.board[yAxis][xAxis] == currentPlayer || game.board[yAxis][xAxis] == '*'){
            return game;
        } else {
            // Entra em loop seguindo a mesma direção para poder continuar andando na "linha" atual
            yAxis += dirY;
            xAxis += dirX;

            return verifyCurrentPos(xAxis, yAxis, index, i, currentPlayer, game);
        }
    }
}

// Verifica todas as 8 direções dado as peças do jogador atual no tabuleiro
othello verifyMoves(char currentPlayer, othello game){
    game = findTiles(currentPlayer, game);
    game.validMoves.totalMoves = 0;

    game = initializeValidMoves(game);

    // Permite que apenas um for seja usado para as 8 direções
    int directionX[] = {-1, -1, -1, 0, 0, 1, 1, 1},
        directionY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(int j = 0; j < game.tilesLength; j++){
        for(int i = 0; i < 8; i++){
            int yAxis = game.tiles[j][0] + directionY[i];
            int xAxis = game.tiles[j][1] + directionX[i];

            // Na primeira validação de jogada válida, um espaço em branco significa que não há jogada válida nessa direção
            if(xAxis < 0 || yAxis < 0 || xAxis > 7 || yAxis > 7 || game.board[yAxis][xAxis] == currentPlayer ||
                game.board[yAxis][xAxis] == '*' || game.board[yAxis][xAxis] == ' '){
            } else {
                yAxis += directionY[i];
                xAxis += directionX[i];

                // Passa para a outra função de validação, que entra em loop até verificar se realmente é uma jogada válida ou não
                game = verifyCurrentPos(xAxis, yAxis, i, j, currentPlayer, game);
            }
        }
    }

    return game;
}

// Lendo todo o tabuleiro, quando achamos uma peça do jogador um, aumentamos sua pontuação em um, e analogamente para o segundo jogador
othello getScore(othello game){
    // Zeramos a pontuação primeiro, pois cada turno deve ser feito a contagem desde o 0
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

// Após o usuário responder uma jogada válida, o tabuleiro sofrerá mudanças baseadas na jogada
othello updateBoard(int posX, int posY, char currentPlayer, othello game){

    // Novamente, dois vetores para possibilitar o uso de um único for
    int directionX[] = {-1, -1, -1, 0, 0, 1, 1, 1},
        directionY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int currentX, currentY;

    // Limpa os '*'s do tabuleiro
    game = cleanBoard(game);
    game.board[posY][posX] = currentPlayer;

    currentX = posX;
    currentY = posY;

    for(int i = 0; i < 8; i++){
        /*
            O primeiro loop é usado para manter a direção atual, onde ele ficará em loop até achar um espaço em branco ou 
            do proprio jogador
        */
        while(true){
            currentX += directionX[i];
            currentY += directionY[i];

            // Verifica se saiu do tabuleiro, achou uma peça do mesmo tipo da jogada ou um espaço em branco
            if(
                currentX < 0 ||
                currentY < 0 ||
                currentX > 7 ||
                currentY > 7 ||
                game.board[currentY][currentX] == currentPlayer ||
                game.board[currentY][currentX] == ' '
            ){
                // Reseta a posição atual para poder seguir outra direção
                currentX = posX;
                currentY = posY;
                break;
            } else {
                currentX += directionX[i];
                currentY += directionY[i];
                /*
                    Verifica se não é um espaço em branco ou se saiu do tabuleiro, pois apartir daqui ele pode ser tanto uma pedrinha
                    do jogador da jogada ou não
                */
                if(
                    currentX < 0 ||
                    currentY < 0 ||
                    currentX > 7 ||
                    currentY > 7 ||
                    game.board[currentY][currentX] == ' '
                ){
                    currentX = posX;
                    currentY = posY;
                    break;
                } else {
                    // Entra em um segundo loop "infinito"
                    while(true){
                        /*
                            Ao encontrar outra pedrinha do jogador da jogada, iremos começar a substituir todas as pedrinhas no caminho
                            pela pedrinha da jogada, até a posição original, em um terceiro e último loop
                        */
                        if(game.board[currentY][currentX] == currentPlayer){
                            while(true){
                                game.board[currentY][currentX] = currentPlayer; 
                                if(currentX == posX && currentY == posY){
                                    break;
                                }
                                currentX -= directionX[i];
                                currentY -= directionY[i];
                            }
                            break;
                        } else {
                            // Aqui seguimos analisando o proximo, até achar um espaço em branco ou uma peça do mesmo tipo da jogada atual
                            if(
                                currentX < 0 ||
                                currentY < 0 ||
                                currentX > 7 ||
                                currentY > 7 ||
                                game.board[currentY][currentX] == ' '
                            ){
                                currentX = posX;
                                currentY = posY;
                                break;
                            } else {
                                currentX += directionX[i];
                                currentY += directionY[i];
                            }
                        }
                    }
                    break;
                }
            }
        }
    }

    game = getScore(game);

    return game;
}


// Escreve o tabuleiro atual para os usuários
void printBoard(othello game){
    printf("Pontos jogador 1: %d\nPontos jogador 2: %d\n\n", game.totalPlayerOne, game.totalPlayerTwo);

    printf("\n    1 2 3 4 5 6 7 8\n");
    printf("\n   -----------------\n");

    for(int i = 0; i < HEIGHT; i++){
        printf("%d  |", i + 1);
        for(int j = 0; j < LENGTH; j++){
            printf("%c|", game.board[i][j]);
        }
            printf("\n   -----------------\n");
    }

    return;
}

/*
    Quando chegamos ao final da partida, ele verifica qual o status do ganhador, para escrever de forma
    personalizada para cada resultado possível
*/
void showWinner(othello game){
    printBoard(game);

    if(game.totalPlayerOne > game.totalPlayerTwo){
        printf("Jogador 1 ganhou!!!");
    } else if(game.totalPlayerTwo > game.totalPlayerOne){
        printf("Jogador 2 ganhou!!!");
    } else {
        printf("Empate!!!");
    }

    return;
}

// Crio uma função para o jogador escolher qual versão irá ser rodada
int menu(){
    int option;

    printf("Bem-vindo ao Othello do Grupo 13\nEscolha uma versao para jogar:\n\n1. JxJ (jogador contra jogador)"
            "\n2. JxC (jogador contra computador)\n\n>>> ");

    // Loop gerado para caso a opção escolhida não seja válida
    while(true){
        option = getInt();

        if(option == 1 || option == 2){
            break;
        } else {
            printf("\nPor favor, escolhar uma opcao valida (1 ou 2):\n1. JxJ (jogador contra jogador)"
                "\n2. JxC (jogador contra computador)\n\n>>> ");
        }
    }

    return option;
}

othello getUserInput(othello game, char currentPlayer){
    int posX, posY;

    posX = getInt();
    posY = getInt();

    // Se posX ou posY não forem um digito únitário, ele pedirá de novo para o usuário uma respota
    if(posX == -1 || posY == -1){
        game.status = false;

        return game;
    } else {
        // Subtrai um das variáveis para poder bater com as coordenadas salvas nos vetores
        posX--;
        posY--;
        
        // Verifica se a resposta do usuário coincide com alguma jogada válida
        for(int i = 0; i < game.validMoves.totalMoves; i++){
            if(posX == game.validMoves.validPos[i][1] && posY == game.validMoves.validPos[i][0]){
                game = updateBoard(posX, posY, currentPlayer, game);

                game.status = true;

                return game;
            }
        }
    }
}

// Praticamente toda a IA é feita aqui
othello getAiInput(othello game, char currentPlayer){
    // Matriz com os pesos de cada jogada, pegamos ela da internet através de pesquisas.
    int weights[8][8] = {
        {120, -20, 20, 5, 5, 20, 20, 120},
        {-20, -40, -5, -5, -5, -5, -40, -20},
        {20, -5, 15, 3, 3, 15, -5, 20},
        {5, -5, 3, 3, 3, 3, -5, 5},
        {5, -5, 3, 3, 3, 3, -5, 5},
        {20, -5, 15, 3, 3, 15, -5, 20},
        {-20, -40, -5, -5, -5, -5, -40, -20},
        {120, -20, 20, 5, 5, 20, 20, 120}
    };

    int currentMove[3];

    /*
        Esse if permite a gente verificar se os quadrados mais importantes já foram preenchidos,
        caso seja verdade, ao invés de usarmos nossa tabela de pesos, iremos ir atrás da jogada
        com maior pontuação.
    */
    if(
        game.board[0][0] != ' ' && game.board[0][0] != '*' &&
        game.board[7][0] != ' ' && game.board[7][0] != '*' &&
        game.board[0][7] != ' ' && game.board[0][7] != '*' &&
        game.board[7][7] != ' ' && game.board[7][7] != '*'
    ){
        int points = game.totalPlayerTwo;
        othello *holder;
        holder = (othello *)malloc(sizeof(othello) * game.validMoves.totalMoves);

        currentMove[1] = game.validMoves.validPos[0][1];
        currentMove[0] = game.validMoves.validPos[0][0];

        for(int i = 0; i < game.validMoves.totalMoves; i++){
            holder[i] = updateBoard(
                game.validMoves.validPos[i][1],
                game.validMoves.validPos[i][0],
                currentPlayer,
                holder[i]
            );

            if(holder[i].totalPlayerTwo > points){
                points = holder[i].totalPlayerTwo;
                currentMove[1] = game.validMoves.validPos[i][1];
                currentMove[0] = game.validMoves.validPos[i][0];
            }
        }
    } else {
        currentMove[2] = -50;

        for(int i = 0; i < game.validMoves.totalMoves; i++){
            if(weights[game.validMoves.validPos[i][1]][game.validMoves.validPos[i][0]] > currentMove[2]){
                currentMove[0] = game.validMoves.validPos[i][0];
                currentMove[1] = game.validMoves.validPos[i][1];

                currentMove[2] = weights[game.validMoves.validPos[i][0]][game.validMoves.validPos[i][1]];
            }
        }
    }

    // Realiza a jogada e devolve o jogo
    game = updateBoard(currentMove[1], currentMove[0], currentPlayer, game);
    game.status = true;

    return game;
}

int main() {
    // Pega um inteiro que define qual versão do jogo será rodada
    int option = menu();

    // Cria as variáveis necessárias para o funcionamento do jogo
    int posX, posY, turn = 1, finished = 0;

    // Inicializa o jogo 
    othello game = initializeGame();

    // Loop onde todo o jogo é executado
    while(finished < 2){
        /*
            Usando o operador ternário, definimos o jogador atual pensando na paridade do turno, caso par,
            é vez do segundo jogador, caso impar, é a vez do primeiro jogador
        */
        char currentPlayer = turn % 2 == 0 ? PLAYERTWO : PLAYERONE;
        printf("\nTurno: %d\n", turn);

        /*
            Verifica as jogadas possíveis para marcar no tabuleiro e pegar uma lista com todas as coordenadas
            das jogadas
        */
        game = verifyMoves(currentPlayer, game);
        printBoard(game);

        // Se há jogadas possíveis, pedimos uma coordenanda do jogador atual
        if(game.validMoves.totalMoves != 0){
            // Define a variavel responsavel pelo termino do jogo como 0
            finished = 0;

            // Loop para pegar uma jogada válida
            while(finished == 0){

                // Verifica se é JxJ ou JxC para fazer as jogadas de acordo
                if(option == 1){
                    // Mostra todas as jogadas possíveis
                    printf("Jogadas validas:\n| ");
                    for(int i = 0; i < game.validMoves.totalMoves; i++){
                        printf("Col: %d Lin: %d | ", game.validMoves.validPos[i][1]+1, game.validMoves.validPos[i][0]+1);
                    }

                    printf("\nDigite a coluna e a linha separadas por um espaco\n>>> ");

                    game = getUserInput(game, currentPlayer);
                } else if(option == 2){
                    if(turn % 2 == 0){
                        game = getAiInput(game, currentPlayer);
                    } else {
                        // Mostra todas as jogadas possíveis
                        printf("Jogadas validas:\n| ");
                        for(int i = 0; i < game.validMoves.totalMoves; i++){
                            printf("Col: %d Lin: %d | ", game.validMoves.validPos[i][1]+1, game.validMoves.validPos[i][0]+1);
                        }

                        printf("\nDigite a coluna e a linha separadas por um espaco\n>>> ");

                        game = getUserInput(game, currentPlayer); 
                    }
                }

                // Se uma jogada foi feita, então o turno sobe um, e sai do loop para que o segundo jogador possa jogar
                if(game.status){
                    game = cleanBoard(game);
                    turn++;
                    break;
                } else {
                    // Pede uma nova resposta do usuário
                    printf("Por favor, digite uma jogada valida\n\n");
                }
            }
        } else {
            // Verifica se o turno foi passado duas vezes seguidas, o que implica no final do jogo, o que explica o "return 0;" ali
            if(finished == 1){
                showWinner(game);
                return 0;
            } else {
                // Pula a vez se não há jogadas possiveis e não for a segunda vez que é passado um turno
                printf("Sem jogadas possiveis, pulando a vez...\n");
                turn++;
                finished++;
            }
        }
    }

    return 0;
}