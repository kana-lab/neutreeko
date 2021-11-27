#ifndef MOVE_AI_PIECE
#define MOVE_AI_PIECE

typedef unsigned long long ull;

#define ACTUAL_SIZE 3542000

int almost_win[ACTUAL_SIZE][5];

#include "move_user_piece.c"
#include "complete_analysis/hash.c"
#include "complete_analysis/forward_of.c"
#include "complete_analysis/delta_of.c"


void load_almost_win() {
    const char *filename = "complete_analysis/perfect_move.dat";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        debug_print("could not open \"%s\"", filename);
        exit(1);
    }

    for (ull i = 0; i < ACTUAL_SIZE; ++i) {
        int *ptr = almost_win[i];
        fscanf(fp, "%d%d%d%d%d", &ptr[0], &ptr[1], &ptr[2], &ptr[3], &ptr[4]);
    }

    fclose(fp);
}

#define IN_ALMOST_WIN(board_hash) ( \
       almost_win[board_hash][0] \
    || almost_win[board_hash][1] \
    || almost_win[board_hash][2] \
    || almost_win[board_hash][3])

void move_ai_piece(int board[5][5]) {
    // githubのREADMEの一番下に書いてあるアルゴリズムを実装する
    // 駒を動かすにはmove_piece関数を利用せよ
    // almost_win[board_hash]の存在チェックにはIN_ALMOST_WIN(board_hash)関数を利用せよ
    // 一手先の盤面の列挙にはforward_of関数を利用せよ
    // 2つの盤面から駒の動きを求めるにはdelta_of関数を利用せよ
    // 駒を動かしたら、その動きを"5C3E"等の文字列に変換しプリントせよ
}

#undef IN_ALMOST_WIN


#endif