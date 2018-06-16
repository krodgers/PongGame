#include <iostream>
#include <stdio.h>

int main(){
  
  int boardLen = 20;
  int board[boardLen][boardLen];


  for(int i = 0; i < boardLen; i ++){
    for(int j = 0; j < boardLen; j ++){
      board[i][j] = 0;
      
    } 
    board[0][0] = 1;
    board[19][19] = 1;
    board[0][19] = 1;
    board[19][0] = 1;
  }
  for(int i = 0; i < boardLen; i ++){
    for(int j = 0; j < boardLen; j ++){
      if(board[i][j] == 1)
	printf("X");
      else
	printf(" ");
    }
      
    printf("\n");
  } 
  

  return 0;
}
