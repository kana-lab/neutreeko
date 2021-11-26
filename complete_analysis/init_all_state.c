#ifndef INIT_ALL_STATE
#define INIT_ALL_STATE

#include "hash.c"


ull init_all_state_num = 0;

void permutation(int board[5][5], int i, int j, int q, int r, int s){
   if ((i==4) && (j==4)) {
       ull hash_board = hash(board);
       for (int a = 0; a < 5; a++){
           for (int b = 0; b < 5; b++){
               all_state[hash_board][a][b] = board[a][b];
           }
       }
       init_all_state_num++;
   } else {
       int m, n;
       if (j==4) {
           m = i + 1;
           n = 0;
       } else {
           m = i;
           n = j + 1;
       }
       if (q>=1) {
           int new_board[5][5];
           memcpy(new_board, board, sizeof(new_board));
           new_board[m][n] = 0;
           permutation(new_board, m, n, q-1, r, s);
       }
       if (r>=1) {
           int new_board[5][5];
           memcpy(new_board, board, sizeof(new_board));
           new_board[m][n] = 1;
           permutation(new_board, m, n, q, r-1, s);
       }
       if (s>=1) {
           int new_board[5][5];
           memcpy(new_board, board, sizeof(new_board));
           new_board[m][n] = -1;
           permutation(new_board, m, n, q, r, s-1);
       }
   }
}

void init_all_state() {
   // 考えうる全ての盤面を列挙し、all_state配列に格納する。
   // 盤面Gは、all_state[hash(G)]に(コピーして)入れる。
   // 必ず、列挙した盤面の数がACTUAL_SIZEと一致しているかどうかのエラーチェックをすること。

   int board[5][5];
   int q = 19; // 残りの0の個数
   int r = 3; // 残りの1の個数
   int s = 3; // 残りの-1の個数

   permutation(board, 0, -1, q, r, s);

   if (init_all_state_num != ACTUAL_SIZE) {
       printf("盤面の数が間違っています");
   }
}


#endif