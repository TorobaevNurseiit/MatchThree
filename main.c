#include <stdio.h>
#include <stdlib.h>

/* Given Global Variables and Definition */
#define H 6  // height
#define W 6  // width

char candies[] = {'*', '#', '@', '%',' '};
/* NO other global variables are allowed */
/****************************************/

/****************************************/

/* Part I Functions */
void initGameBoard(int board[][W], int board_sample[][W]);
void printGameBoard(int board[][W]);
int askForSwap(int board[][W]);
void swap(int board[][6], int row1, int col1, int row2, int col2) ;
int findAndRemoveMatch(int board[][W], int row, int col);
int isMatching(int board[6][6], int row, int col);
/* Part II Functions */
int initGameBoardFromFile(int board[][W], int stacks[]);
void applyGravity(int board[][W]);
int fillEmpty(int board[][W], int stacks[], int current, int numCandies);
int cascade(int board[][6], int stacks[], int current, int numCandies);

/****************************************/
// Function Definitions/

/**
 * Main() will call this function in the beginning.
 * initGameBoard() load the game board with the pre-defined board_sample array
 * @param board The gameboard the game is using
 * @param board_sample The given game board sample values
 */
void initGameBoard(int board[H][W], int board_sample[][W]) {
  for (int i = 0; i < H; i++)
  {
    for (int j = 0; j < W; j++)
    {
      board[i][j]=board_sample[i][j];
    }
    
  }
}

/**
 * [Part II]
 * Main() will call this function in the beginning.
 * initGameBoardFromFile will load the file "board.txt" to load two information:
 * (1) Game Board ; (2) Candy Stacks
 * If the file loading is unsuccessful, quit the program with the error message.
 * @param board The Game board the program is using.
 * @param stacks The stack array to store candies. It will be inititalized from the file.
 * @return The number of candies in the stack.
 */
int initGameBoardFromFile(int board[][W], int stacks[]) {
    FILE *fptr;
    int height, width, NumberOfCandies;
    fptr = fopen("board.txt", "r");
    if (fptr==NULL){
        printf("Failed to open board.txt!\n");
        exit(1);
    }
    fscanf(fptr, "%d%d", &height, &width);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fscanf(fptr, "%d", &board[i][j]);
        }
    }
    fscanf(fptr, "%d", &NumberOfCandies);
    for (int i = 0; i < NumberOfCandies; i++)
    {
        fscanf(fptr, "%d", &stacks[i]);
    }
    fclose(fptr);
    return NumberOfCandies;
}

/**
 * Various functions will call this.
 * printGameBoard() will print the current game board in the specified format
 * @param board The game board the game is using
 */
void printGameBoard(int board[][W]){
  printf("=====\n");
  printf("  | 0 | 1 | 2 | 3 | 4 | 5 |\n");
  for (int i = 0; i < H; i++)
  {
    printf(" %d|", i);
    for (int j = 0; j < W; j++)
    {
      printf(" %c |", candies[board[i][j]]);
    }
    printf("\n");
  }
  
}

/**
 * Main() will call this function.
 * askForSwap() will perform the major game logic by (1) asking the users which candy to swap
 * (2)Validate whether the swapping can be done (3) Perform the swapping
 * @param board The game board the game is using.
 * @return 0 if the error is encountered (e.g. Coordinates out of bound);
 * 1 if swapping is successfully
 */
int askForSwap(int board[][W]) {
  int row1,col1;
  printf("Enter the coordinate (row, column) of the candy:");
  scanf("%d %d", &row1, &col1);
  if(row1>5||row1<0||col1>5||col1<0)
  {
    printf("Coordinates Out of Bound.\n");
    return 0;
  }
  if(board[row1][col1]==4)
  {
    printf("Empty Cell Selected.\n");
    return 0;
  }
  char direction;
  printf("Enter the direction to swap (U for Up, D for Down, L for Left, R for Right):");
  scanf(" %c", &direction);
  int row2=row1, col2=col1;
  if(direction=='L')
  { 
    col2-=1;
  }
  if(direction=='R')
  {
    col2+=1;
  }
  if(direction=='U')
  {
    row2-=1;
  }
  if(direction=='D')
  {
    row2+=1;
  }
  if(board[row2][col2]==4)
  {
    printf("Empty Cell Selected.\n");
    return 0;
  }
  if((direction=='L'&&col2<0)||(direction=='R'&&col2>5)||(direction=='U'&&row2<0)||(direction=='D'&&row2>5))
  {
    printf("Move Out of Bound.\n");
    return 0;
  }
  
  if(direction!='L'&&direction!='R'&&direction!='U'&&direction!='D')
  {
    printf("Wrong Direction Input.\n");
    return 0;
  }
  else
  {
    swap(board, row1, col1, row2, col2);
    int matchOnTarget=findAndRemoveMatch(board, row2, col2);
    int matchOnOriginal=findAndRemoveMatch(board, row1, col1);
    if(matchOnTarget==0&&matchOnOriginal==0)
    {
      printGameBoard(board);
      printf("No Match found!\n");
      swap(board, row1, col1, row2, col2);
    }
    else
    {
    
      printGameBoard(board);
    }
    return 1;
  }
}

/**
 * Various functions will call this
 * swap() will perform value swapping of two cells,
 * with the given source and destination coordinates respectivel.
 * @param board the game board the game is using
 * @param row1 The row number of the source cell
 * @param col1 The column number of the source cell
 * @param row2 The row number of the destination cell
 * @param col2 The column number of the destination cell
 */
void swap(int board[][6], int row1, int col1, int row2, int col2) {
  int temp=board[row1][col1];
  board[row1][col1]=board[row2][col2];
  board[row2][col2]=temp;
  }

/**
 * Various functions will call this.
 * findAndRemoveMatch() will find possible matches at the given coordinates (row,col)
 * for all four directions (Up, Down, Left, Right).
 * If found, turn the cell to ASCII 32 (' ').
 *
 * @param board The game board the game is using
 * @param row The row number of the given coordinate
 * @param col The column number of the given coordinate
 * @return 1 if there is a match in the board, 0 if there is not.
 */
int findAndRemoveMatch(int board[][W], int row, int col) {
  int HorizontalMatches[20]={0};
  int count=1;
  int NumberOfHorizontalMatches=1;
  int MatchIsFound=1;
  while(col-count>=0&&MatchIsFound==1)
  {
    if(board[row][col]==board[row][col-count])
    {
      HorizontalMatches[NumberOfHorizontalMatches-1]=col-count;
      NumberOfHorizontalMatches++;
    }
    else
    {
      MatchIsFound=0;
    }
    count++;
  }
  MatchIsFound=1;
  count=1;
  while(col+count<=5&&MatchIsFound==1)
  {
    if(board[row][col]==board[row][col+count])
    {
      HorizontalMatches[NumberOfHorizontalMatches-1]=col+count;
      NumberOfHorizontalMatches++;
    }
    else
    {
      MatchIsFound=0;
    }
    count++;
  }
  int VerticalMatches[20]={0};
  int NumberOfVerticalMatches=1;
  count=1;
  MatchIsFound=1;
  while(row-count>=0&&MatchIsFound==1)
  {
    if(board[row][col]==board[row-count][col])
    {
      VerticalMatches[NumberOfVerticalMatches-1]=row-count;
      NumberOfVerticalMatches++;
    }
    else
    {
      MatchIsFound=0;
    }
    count++;
  }
  MatchIsFound=1;
  count=1;
  while(row+count<=5&&MatchIsFound==1)
  {
    if(board[row][col]==board[row+count][col])
    {
      VerticalMatches[NumberOfVerticalMatches-1]=row+count;
      NumberOfVerticalMatches++;
    }
    else
    {
      MatchIsFound=0;
    }
    count++;
  }
  if(NumberOfHorizontalMatches>=3)
  {
    if(board[row][col]!=4)
    {
      printf("Horizontal Match found at row %d!\n", row);
    }
    board[row][col]=4;
    for (int i = 0; i < NumberOfHorizontalMatches-1; i++)
    {
      board[row][HorizontalMatches[i]]=4;
    }
  }
  if(NumberOfVerticalMatches>=3)
  {
    if((board[row][col]!=4)||(board[row][col]==4&&NumberOfHorizontalMatches>=3))
    {
      printf("Vertical Match found at column %d!\n", col);
    }
    board[row][col]=4;
    for (int i = 0; i < NumberOfVerticalMatches-1; i++)
    {
      board[VerticalMatches[i]][col]=4;
    }
  }
  if (NumberOfVerticalMatches>=3||NumberOfHorizontalMatches>=3)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
 * Main() function will call this.
 * isGameOver() is to check whether there exists a game over situation, i.e.
 * no more match can be made under the current board scenerio.
 *
 * @param board The game board the game is using.
 * @return 1 if it is game over; 0 if it isn't .
 */
int isGameOver(int board[][W]) {
  for (int row = 0; row < H; row++) 
  {
    for (int col = 0; col < W; col++) 
    {
      if (board[row][col] == 4) 
      {
        continue;
      }
      if (col+1<W) 
      {
        swap(board, row, col, row, col+1);
        if (isMatching(board, row, col) || isMatching(board, row, col + 1)) 
        {
          swap(board, row, col, row, col+1);
          return 0;
        }
        swap(board, row, col, row, col+1);
      }
      if (col-1>=0) 
      {
        swap(board, row, col, row, col-1);
        if (isMatching(board, row, col) || isMatching(board, row, col-1)) 
        {
          swap(board, row, col, row, col-1);
          return 0;
        }
        swap(board, row, col, row, col-1);
      }
      if (row+1<H) 
      {
        swap(board, row, col, row+1, col);
        if (isMatching(board, row, col) || isMatching(board, row+1, col)) {
          swap(board, row, col, row+1, col);
          return 0;
        }
        swap(board, row, col, row+1, col);
      }
      if (row-1>=0) 
      {
        swap(board, row, col, row-1, col);
        if (isMatching(board, row, col) || isMatching(board, row-1, col)) {
          swap(board, row, col, row-1, col);
          return 0;
        }
        swap(board, row, col, row-1, col);
      }
    }
  }
  return 1;
}

/**
 * isGameOver() will call this function.
 * isMatching will check if there is at least 1 match-three at the given coordinate (row,col)
 * for four directions (Top,Down,Left,Right).
 * @param board The game board the game is using.
 * @param row The row number of the target cell
 * @param col The column number of the target cell
 * @return 1 if there is at least 1 match in any direction; 0 if there is not.
 */
int isMatching(int board[6][6], int row, int col) {
  if ( board[row][col] == 4) { 
    return 0;
  }
  int NumberOfHorizontalMatches = 1;
  for (int i = col - 1; i >= 0 && board[row][i] == board[row][col]; i--) {
    NumberOfHorizontalMatches++;
  }
  for (int i = col + 1; i < W && board[row][i] == board[row][col]; i++) {
    NumberOfHorizontalMatches++;
  }
  int NumberOfVerticalMatches = 1;
  for (int i = row - 1; i >= 0 && board[i][col] == board[row][col]; i--) {
    NumberOfVerticalMatches++;
  }
  for (int i = row + 1; i < H && board[i][col] == board[row][col]; i++) {
    NumberOfVerticalMatches++;
  }
  if (NumberOfVerticalMatches >= 3) {
    return 1;
  }
  if (NumberOfHorizontalMatches >= 3) {
    return 1;
  }
  return 0;
}


/**
 * [Part II]
 * askForSwap() and cascade() will call this function
 * applyGravity() will scan the all columns from left to right, and
 * if there are empty cells, it will move down the candy. Lastly, print the
 * gameboard.
 * @param board The game board the game is using
 */
void applyGravity(int board[][6]) 
{
    int Non_EmptyH, Non_EmptyW;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 5; j >= 1; j--)
        {
            if (board[j][i]==4)
            {
                Non_EmptyH=j;
                Non_EmptyW=i;
                while(board[Non_EmptyH][Non_EmptyW]==4)
                {
                    Non_EmptyH--;
                }
                if(Non_EmptyH<0)
                {
                    continue;
                }
                else
                {
                    swap(board, Non_EmptyH, Non_EmptyW, j, i);
                    board[Non_EmptyH][Non_EmptyW]=4;
                }
            }
        }
    }
}

/**
 * [Part II]
 * askForSwap() and cascade() will call this.
 * fillEmpty() will scan the columns from left to right. If there are empty cells,
 * it will fill them with candies got from the stacks, in bottom-to-top manner. Lastly,
 * print the gameboard.
 * @param board the game board the game is using
 * @param stacks the 1D array containing the candies
 * @param current the counter of the next candy location in stacks array
 * @param numCandies the total number of candies in the stacks
 * @return the updated current value, i.e. the updated counter pointing to the next
 * available candy
 */
int fillEmpty(int board[][W], int stacks[], int current, int numCandies) 
{
  if (current>=numCandies)
  {
    printf("No more candies available.\n");
    exit(1);
    return current;
  }
  else{
    for (int i = 0; i < 6; i++)
    {
      for (int j = 5; j >= 0; j--)
      {
        if(board[j][i]==4)
        {
          if (current<numCandies)
          {
            board[j][i]=stacks[current];
            current++;
          }
          else
          {
            if (current>=numCandies)
            {
                printf("No more candies available.\n");
                exit(1);
                return current;
            }
          }
        }
      }
    }
    return current;
  }
    
}

/**
 * [Part II]
 * main() function will call this
 * cascade() will continously check the board to see if there is any matches
 * and perform removal until no further matches can be found.
 * First, applying the gravity and fill in the empty cells.
 * Then, for each cells on the board and if it is not empty, calling
 * findAndRemoveMatch() to remove matches if any. If there is a match,
 * printing the specified message.
 * collecting the return value of current in fillEmpty() such that
 * returning to the main function to update the variable.
 * @param board the game board the game is using
 * @param stacks the 1D array containing the candies
 * @param current the counter of the next candy location in stacks array
 * @param numCandies the total number of candies in the stacks
* @return the updated current value, i.e. the updated counter pointing to the next
 * available candy
 */
int cascade(int board[][6], int stacks[], int current, int numCandies) 
{
  int cascadeFound=1;
  if (current>=numCandies)
  {
    printf("No more candies available.\n");
    exit(1);
    return current;
  }
  applyGravity(board);
  printGameBoard(board);
  current=fillEmpty(board, stacks, current, numCandies);
  printGameBoard(board);
  while (cascadeFound>0)
  {
    cascadeFound=0;
    for (int i = 0; i < H; i++)
    {
      for (int j = 0; j < W; j++)
      {
        if(board[i][j]!=4&&findAndRemoveMatch(board, i, j)==1)
        {
          cascadeFound=1;
        }
      }
    }
    if (cascadeFound==1)
    {
      printf("Cascade Matches found!\n");
      applyGravity(board);
      printGameBoard(board);
      current=fillEmpty(board, stacks, current, numCandies);
      if (current>=numCandies)
      {
        printf("No more candies available.\n");
        exit(1);
        return current;
      }
      printGameBoard(board);
    }
  }
  applyGravity(board);
  current=fillEmpty(board, stacks, current, numCandies);
  
  return current;
}
/* Main Function */
/**
 * The main function will hold the game loop and hold the game logic.
 * main() will call various functions to accompolish various tasks such as
 * initializing the game board, asking for Input, and etc.
 * It will hold an infinite loop to repeatly hold the game. If it is a game over status,
 * i.e. isGameOver() returns one (1), then it will break out of the loop and the program
 * finishes.
 *
 * @return 0
 */
int main(void) {
 // This is the variable holding a sample board value
int board_sample[6][6] = {
  {1,3,2,0,1,0},
  {2,2,0,1,1,3},
  {1,3,3,2,3,0},
  {3,0,2,1,0,2},
  {1,0,2,3,3,2},
  {3,2,1,0,3,3}
 };





 int board[H][W] = {0}; // The game board, initialized to 0
 int stacks[100] = {0}; // Stacks array containing candy, Maximum capacity 100
 int numCandies = 0; // Number of candies in the stacks
 int current = 0; // the counter pointing to the next available candy
 int flag=1;

 numCandies=initGameBoardFromFile(board, stacks);
 
while (flag==1)
{
  printf("=====\n");
  printf("New Round:\n");
  printGameBoard(board);
  if (askForSwap(board)==0)
  {
    printf("Please try again.\n");
  }
  current=cascade(board, stacks, current, numCandies);
  if (current>=numCandies)
  {
    flag=0;
    printf("No more candies available.\n");
    break;
  }
  if(isGameOver(board)==1)
  {
    printf("Game Over! No more possible moves.\n");
    flag=0;
    break;
  }
}

// Hope you enjoy the game : )

 return 0;
}
