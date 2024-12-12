//https://youtu.be/ExlNWeIjAFo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 20
#define NUM_PLAYERS 2
#define NUM_COLORS 5

typedef enum
{
    PLAYER_VS_PLAYER,
    PLAYER_VS_COMPUTER
} GameMode;

typedef struct
{
    char **board;
    int size;
    int *playerScores;
    int **playerSets;
    int currentPlayer;
    GameMode mode;
} GameState;

typedef struct
{
    int startX, startY;
    int endX, endY;
    char capturedSkipper;
    int player;
} Move;

typedef struct
{
    Move *moves;
    int top;
    int max;
} MoveStack;

void initializeBoard(GameState *game, int size);
void placePieces(GameState *game);
void displayBoard(GameState *game);
void displayCapturedSkippers(GameState *game);
void makeMove(GameState *game, int player, int startRow, int startColumn, int endRow, int endColumn, MoveStack *moveStack, int isComputer);
void undoMove(GameState *game, MoveStack *undoStack, MoveStack *redoStack);
void redoMove(GameState *game, MoveStack *undoStack, MoveStack *redoStack);
void saveGame(GameState *game, const char *filename);
void loadGame(GameState *game, const char *filename);
void playGame(GameState *game);
int isValidMove(GameState *game, int startRow, int startColumn, int endRow, int endColumn);
int hasAvailableJump(GameState *game, int row, int col);
int hasAnyValidMove(GameState *game, int player);
void captureSkipper(GameState *game, int player, int startRow, int startColumn, int endRow, int endColumn);
void initMoveStack(MoveStack *stack, int max);
int isMoveStackEmpty(MoveStack *stack);
int isMoveStackFull(MoveStack *stack);
void pushMove(MoveStack *stack, Move move);
Move popMove(MoveStack *stack);
void declareWinner(GameState *game);
int calculateCompleteSets(GameState *game, int player);
void getMoveInput(char *prompt, char *input);
int parseMoveInput(const char *input, int *startColumn, int *startRow, int *endColumn, int *endRow);
int parseNextJumpInput(const char *input, int *newColumn, int *newRow);
void computerMove(GameState *game, MoveStack *moveStack);

int main()
{
    GameState game;
    int boardSize;

    // Display menu for selecting game mode
    int modeChoice;
    printf("Select game mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Computer\n");
    printf("Enter your choice: ");
    scanf("%d", &modeChoice);
    getchar(); // clear the newline character from the buffer
    if (modeChoice == 1)
    {
        game.mode = PLAYER_VS_PLAYER;
    }
    else if (modeChoice == 2)
    {
        game.mode = PLAYER_VS_COMPUTER;
    }
    else
    {
        printf("Invalid choice. Exiting.\n");
        return 1;
    }

    printf("Enter the size of the board (max 20x20): ");
    scanf("%d", &boardSize);
    getchar(); // clear the newline character from the buffer
    if (boardSize > MAX_SIZE || boardSize <= 0)
    {
        printf("Invalid board size. Please enter a size between 1 and 20.\n");
        return 1;
    }
    initializeBoard(&game, boardSize);
    placePieces(&game);
    playGame(&game);
    return 0;
}

void initializeBoard(GameState *game, int size)
{
    game->size = size;
    game->board = (char **)malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++)
    {
        game->board[i] = (char *)malloc(size * sizeof(char));
        for (int j = 0; j < size; j++)
        {
            game->board[i][j] = ' ';
        }
    }
    game->playerScores = (int *)malloc(NUM_PLAYERS * sizeof(int));
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        game->playerScores[i] = 0;
    }
    game->playerSets = (int **)malloc(NUM_PLAYERS * sizeof(int *));
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        game->playerSets[i] = (int *)calloc(NUM_COLORS, sizeof(int));
    }
    game->currentPlayer = 0;
}

void placePieces(GameState *game)
{
    srand(time(NULL));
    int center = game->size / 2;
    for (int i = 0; i < game->size; i++)
    {
        for (int j = 0; j < game->size; j++)
        {
            if (!(i >= center - 1 && i <= center && j >= center - 1 && j <= center))
            {
                game->board[i][j] = 'A' + (rand() % NUM_COLORS); // Only use 'A', 'B', 'C', 'D', 'E'
            }
        }
    }
}

void displayBoard(GameState *game)
{
    printf("   ");
    for (int j = 0; j < game->size; j++)
    {
        printf("%2d ", j + 1);
    }
    printf("\n");
    for (int i = 0; i < game->size; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < game->size; j++)
        {
            printf(" %c ", game->board[i][j]);
        }
        printf("\n");
    }
    displayCapturedSkippers(game);
}

void displayCapturedSkippers(GameState *game)
{
    for (int player = 0; player < NUM_PLAYERS; player++)
    {
        printf("Player %d\n", player + 1);
        for (int color = 0; color < NUM_COLORS; color++)
        {
            printf("%c: %d  ", 'A' + color, game->playerSets[player][color]);
        }
        printf("\n");
    }
}

int isValidMove(GameState *game, int startRow, int startColumn, int endRow, int endColumn)
{
    if (startRow < 0 || startRow >= game->size || startColumn < 0 || startColumn >= game->size ||
        endRow < 0 || endRow >= game->size || endColumn < 0 || endColumn >= game->size)
    {
        return 0;
    }
    if (game->board[startRow][startColumn] == ' ' || game->board[endRow][endColumn] != ' ')
    {
        return 0;
    }
    int dx = endRow - startRow;
    int dy = endColumn - startColumn;
    if ((dx == 0 && abs(dy) == 2) || (dy == 0 && abs(dx) == 2))
    {
        int midRow = startRow + dx / 2;
        int midColumn = startColumn + dy / 2;
        if (game->board[midRow][midColumn] != ' ')
        {
            return 1;
        }
    }
    return 0;
}

int hasAvailableJump(GameState *game, int row, int col)
{
    int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
    for (int i = 0; i < 4; i++)
    {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        if (isValidMove(game, row, col, newRow, newCol))
        {
            return 1;
        }
    }
    return 0;
}

int hasAnyValidMove(GameState *game, int player)
{
    for (int startRow = 0; startRow < game->size; startRow++)
    {
        for (int startColumn = 0; startColumn < game->size; startColumn++)
        {
            if (game->board[startRow][startColumn] != ' ' &&
                (player == 0 ? (game->board[startRow][startColumn] >= 'A' && game->board[startRow][startColumn] <= 'E') : (game->board[startRow][startColumn] >= 'A' && game->board[startRow][startColumn] <= 'E')))
            {
                int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
                for (int i = 0; i < 4; i++)
                {
                    int endRow = startRow + directions[i][0];
                    int endColumn = startColumn + directions[i][1];
                    if (isValidMove(game, startRow, startColumn, endRow, endColumn))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void captureSkipper(GameState *game, int player, int startRow, int startColumn, int endRow, int endColumn)
{
    int midRow = (startRow + endRow) / 2;
    int midColumn = (startColumn + endColumn) / 2;
    char captured = game->board[midRow][midColumn];
    game->board[midRow][midColumn] = ' ';
    game->board[endRow][endColumn] = game->board[startRow][startColumn];
    game->board[startRow][startColumn] = ' ';
    game->playerScores[player]++;
    game->playerSets[player][captured - 'A']++;
}

void makeMove(GameState *game, int player, int startRow, int startColumn, int endRow, int endColumn, MoveStack *moveStack, int isComputer)
{
    if (!isValidMove(game, startRow, startColumn, endRow, endColumn))
    {
        printf("Invalid move. Try again.\n");
        return;
    }

    // Perform the first move
    Move move = {startRow, startColumn, endRow, endColumn, game->board[(startRow + endRow) / 2][(startColumn + endColumn) / 2], player};
    captureSkipper(game, player, startRow, startColumn, endRow, endColumn);
    pushMove(moveStack, move);

    int currentRow = endRow;
    int currentColumn = endColumn;
    int newRow, newColumn;

    // Handle multiple jumps
    while (1)
    {
        if (!hasAvailableJump(game, currentRow, currentColumn))
        {
            break;
        }

        displayBoard(game);
        if (isComputer == 0){
            printf("Player %d, enter your next jump (column row) or -1 -1 to end turn: ", player + 1);
            char input[20];
            getMoveInput("", input);
            if (!parseNextJumpInput(input, &newColumn, &newRow))
            {
                printf("Invalid input. Try again.\n");
                continue;
            }
        }
        newRow -= 1;
        newColumn -= 1;
        if (newRow == -2 && newColumn == -2)
            break; // -1 -1 will be converted to -2 -2 after subtracting 1

        if (isValidMove(game, currentRow, currentColumn, newRow, newColumn))
        {
            Move nextMove = {currentRow, currentColumn, newRow, newColumn, game->board[(currentRow + newRow) / 2][(currentColumn + newColumn) / 2], player};
            captureSkipper(game, player, currentRow, currentColumn, newRow, newColumn);
            pushMove(moveStack, nextMove);
            currentRow = newRow;
            currentColumn = newColumn;
        }
        else
        {
            printf("Invalid move. Turn ended.\n");
            break;
        }
    }
}

void initMoveStack(MoveStack *stack, int max)
{
    stack->moves = (Move *)malloc(max * sizeof(Move));
    stack->top = -1;
    stack->max = max;
}

int isMoveStackEmpty(MoveStack *stack)
{
    return stack->top == -1;
}

int isMoveStackFull(MoveStack *stack)
{
    return stack->top == stack->max - 1;
}

void pushMove(MoveStack *stack, Move move)
{
    if (!isMoveStackFull(stack))
    {
        stack->moves[++stack->top] = move;
    }
}

Move popMove(MoveStack *stack)
{
    if (!isMoveStackEmpty(stack))
    {
        return stack->moves[stack->top--];
    }
    Move invalidMove = {-1, -1, -1, -1, ' ', -1};
    return invalidMove;
}

void undoMove(GameState *game, MoveStack *undoStack, MoveStack *redoStack)
{
    if (isMoveStackEmpty(undoStack))
    {
        printf("No moves to undo.\n");
        return;
    }
    Move move = popMove(undoStack);
    game->board[move.startX][move.startY] = game->board[move.endX][move.endY];
    game->board[move.endX][move.endY] = ' ';
    game->board[(move.startX + move.endX) / 2][(move.startY + move.endY) / 2] = move.capturedSkipper;
    game->playerScores[move.player]--;
    game->playerSets[move.player][move.capturedSkipper - 'A']--;
    pushMove(redoStack, move);
}

void redoMove(GameState *game, MoveStack *undoStack, MoveStack *redoStack)
{
    if (isMoveStackEmpty(redoStack))
    {
        printf("No moves to redo.\n");
        return;
    }
    Move move = popMove(redoStack);
    captureSkipper(game, move.player, move.startX, move.startY, move.endX, move.endY);
    pushMove(undoStack, move);
}

void saveGame(GameState *game, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error opening file for saving.\n");
        return;
    }
    fwrite(&(game->size), sizeof(int), 1, file);
    fwrite(&(game->currentPlayer), sizeof(int), 1, file);
    fwrite(&(game->mode), sizeof(GameMode), 1, file);
    fwrite(game->playerScores, sizeof(int), NUM_PLAYERS, file);
    for (int i = 0; i < game->size; i++)
    {
        fwrite(game->board[i], sizeof(char), game->size, file);
    }
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        fwrite(game->playerSets[i], sizeof(int), NUM_COLORS, file);
    }
    fclose(file);
}

void loadGame(GameState *game, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file for loading.\n");
        return;
    }
    fread(&(game->size), sizeof(int), 1, file);
    fread(&(game->currentPlayer), sizeof(int), 1, file);
    fread(&(game->mode), sizeof(GameMode), 1, file);
    game->playerScores = (int *)malloc(NUM_PLAYERS * sizeof(int));
    fread(game->playerScores, sizeof(int), NUM_PLAYERS, file);
    game->board = (char **)malloc(game->size * sizeof(char *));
    for (int i = 0; i < game->size; i++)
    {
        game->board[i] = (char *)malloc(game->size * sizeof(char));
        fread(game->board[i], sizeof(char), game->size, file);
    }
    game->playerSets = (int **)malloc(NUM_PLAYERS * sizeof(int *));
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        game->playerSets[i] = (int *)malloc(NUM_COLORS * sizeof(int));
        fread(game->playerSets[i], sizeof(int), NUM_COLORS, file);
    }
    fclose(file);
}

void playGame(GameState *game)
{
    int startRow, startColumn, endRow, endColumn;
    MoveStack undoStack, redoStack;
    initMoveStack(&undoStack, 100);
    initMoveStack(&redoStack, 100);

    while (1)
    {
        displayBoard(game);
        if (game->currentPlayer == 0 || game->mode == PLAYER_VS_PLAYER)
        {
            printf("Player %d, enter your move (startColumn startRow endColumn endRow), or enter 'u' to undo, 'r' to redo, 's' to save, 'l' to load, or 'q' to quit: ", game->currentPlayer + 1);

            char input[20];
            getMoveInput("", input);

            if (input[0] == 'q')
            {
                break;
            }
            else if (input[0] == 'u')
            {
                undoMove(game, &undoStack, &redoStack);
            }
            else if (input[0] == 'r')
            {
                redoMove(game, &undoStack, &redoStack);
            }
            else if (input[0] == 's')
            {
                saveGame(game, "savegame.dat");
                printf("Game saved.\n");
            }
            else if (input[0] == 'l')
            {
                loadGame(game, "savegame.dat");
                printf("Game loaded.\n");
            }
            else
            {
                int validMove = 0;
                while (!validMove)
                {
                    if (parseMoveInput(input, &startColumn, &startRow, &endColumn, &endRow))
                    {
                        startRow -= 1;
                        startColumn -= 1;
                        endRow -= 1;
                        endColumn -= 1;

                        if (isValidMove(game, startRow, startColumn, endRow, endColumn))
                        {
                            makeMove(game, game->currentPlayer, startRow, startColumn, endRow, endColumn, &undoStack, 0);
                            if (!hasAnyValidMove(game, 1 - game->currentPlayer))
                            {
                                declareWinner(game);
                                return;
                            }
                            game->currentPlayer = 1 - game->currentPlayer; // Switch players
                            while (!isMoveStackEmpty(&redoStack))
                                popMove(&redoStack); // Clear redo stack
                            validMove = 1;
                        }
                        else
                        {
                            printf("Invalid move. Try again: ");
                            getMoveInput("", input);
                        }
                    }
                    else
                    {
                        printf("Invalid input. Try again: ");
                        getMoveInput("", input);
                    }
                }
            }
        }

        if (game->mode == PLAYER_VS_COMPUTER && game->currentPlayer == 1)
        {
            computerMove(game, &undoStack);
            if (!hasAnyValidMove(game, 1 - game->currentPlayer))
            {
                declareWinner(game);
                return;
            }
            game->currentPlayer = 0; // Switch back to player
        }
    }
    printf("Game Over.\n");
    declareWinner(game);
}

void computerMove(GameState *game, MoveStack *moveStack)
{
    printf("Computer is thinking...\n");

    int bestScore = -1;
    Move bestMove;
    int currentRow, currentColumn;

    while (1)
    {
        bestScore = -1;

        for (int startRow = 0; startRow < game->size; startRow++)
        {
            for (int startColumn = 0; startColumn < game->size; startColumn++)
            {
                if (game->board[startRow][startColumn] != ' ' && game->board[startRow][startColumn] != 'A')
                {
                    int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
                    for (int i = 0; i < 4; i++)
                    {
                        int endRow = startRow + directions[i][0];
                        int endColumn = startColumn + directions[i][1];
                        if (isValidMove(game, startRow, startColumn, endRow, endColumn))
                        {
                            int score = 1; // Simple heuristic: 1 point per capture
                            if (score > bestScore)
                            {
                                bestScore = score;
                                bestMove.startX = startRow;
                                bestMove.startY = startColumn;
                                bestMove.endX = endRow;
                                bestMove.endY = endColumn;
                                bestMove.capturedSkipper = game->board[(startRow + endRow) / 2][(startColumn + endColumn) / 2];
                                bestMove.player = 1; // Computer is player 2
                            }
                        }
                    }
                }
            }
        }

        if (bestScore > -1)
        {
            makeMove(game, 1, bestMove.startX, bestMove.startY, bestMove.endX, bestMove.endY, moveStack, 1);
            currentRow = bestMove.endX;
            currentColumn = bestMove.endY;
            // Handle multiple jumps
            while (hasAvailableJump(game, currentRow, currentColumn))
            {
                bestScore = -1;
                for (int i = 0; i < 4; i++)
                {
                    int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
                    int newRow = currentRow + directions[i][0];
                    int newColumn = currentColumn + directions[i][1];
                    if (isValidMove(game, currentRow, currentColumn, newRow, newColumn))
                    {
                        int score = 1; // Simple heuristic: 1 point per capture
                        if (score > bestScore)
                        {
                            bestScore = score;
                            bestMove.startX = currentRow;
                            bestMove.startY = currentColumn;
                            bestMove.endX = newRow;
                            bestMove.endY = newColumn;
                            bestMove.capturedSkipper = game->board[(currentRow + newRow) / 2][(currentColumn + newColumn) / 2];
                            bestMove.player = 1; // Computer is player 2
                        }
                    }
                }
                if (bestScore > -1)
                {
                    makeMove(game, 1, bestMove.startX, bestMove.startY, bestMove.endX, bestMove.endY, moveStack, 1);
                    currentRow = bestMove.endX;
                    currentColumn = bestMove.endY;
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            // Perform a random valid move if no high-scoring move is found
            int foundMove = 0;
            while (!foundMove)
            {
                int startRow = rand() % game->size;
                int startColumn = rand() % game->size;
                if (game->board[startRow][startColumn] != ' ' && game->board[startRow][startColumn] != 'A')
                {
                    int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
                    for (int i = 0; i < 4; i++)
                    {
                        int endRow = startRow + directions[i][0];
                        int endColumn = startColumn + directions[i][1];
                        if (isValidMove(game, startRow, startColumn, endRow, endColumn))
                        {
                            makeMove(game, 1, startRow, startColumn, endRow, endColumn, moveStack, 1);
                            foundMove = 1;
                            break;
                        }
                    }
                }
            }
            return;
        }
    }
}

void declareWinner(GameState *game)
{
    int setsPlayer1 = calculateCompleteSets(game, 0);
    int setsPlayer2 = calculateCompleteSets(game, 1);

    printf("Player 1: %d sets, %d extra skippers\n", setsPlayer1, game->playerScores[0] - setsPlayer1 * NUM_COLORS);
    printf("Player 2: %d sets, %d extra skippers\n", setsPlayer2, game->playerScores[1] - setsPlayer2 * NUM_COLORS);

    if (setsPlayer1 > setsPlayer2)
    {
        printf("Player 1 wins!\n");
    }
    else if (setsPlayer2 > setsPlayer1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        if ((game->playerScores[0] - setsPlayer1 * NUM_COLORS) > (game->playerScores[1] - setsPlayer2 * NUM_COLORS))
        {
            printf("Player 1 wins!\n");
        }
        else if ((game->playerScores[1] - setsPlayer2 * NUM_COLORS) > (game->playerScores[0] - setsPlayer1 * NUM_COLORS))
        {
            printf("Player 2 wins!\n");
        }
        else
        {
            printf("It's a draw!\n");
        }
    }
}

int calculateCompleteSets(GameState *game, int player)
{
    int minCount = game->playerSets[player][0];
    for (int i = 1; i < NUM_COLORS; i++)
    {
        if (game->playerSets[player][i] < minCount)
        {
            minCount = game->playerSets[player][i];
        }
    }
    return minCount;
}

void getMoveInput(char *prompt, char *input)
{
    printf("%s", prompt);
    fgets(input, 20, stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove the newline character
}

int parseMoveInput(const char *input, int *startColumn, int *startRow, int *endColumn, int *endRow)
{
    return sscanf(input, "%d %d %d %d", startColumn, startRow, endColumn, endRow) == 4;
}

int parseNextJumpInput(const char *input, int *newColumn, int *newRow)
{
    return sscanf(input, "%d %d", newColumn, newRow) == 2;
}
