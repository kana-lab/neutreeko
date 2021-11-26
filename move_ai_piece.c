#ifndef MOVE_AI_PIECE
#define MOVE_AI_PIECE

typedef unsigned long long ull;

#define ACTUAL_SIZE 3542000

int almost_win[ACTUAL_SIZE][5];

#include "move_user_piece.c"
#include "complete_analysis/hash.c"
#include "complete_analysis/forward_of.c"


void load_almost_win() {
    const char *filename = "perfect_move.dat";
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

#define IN_ALMOST_WIN(x) ( \
       almost_win[hash(x)][0] \
    || almost_win[hash(x)][1] \
    || almost_win[hash(x)][2] \
    || almost_win[hash(x)][3])

void move_ai_piece(int board[5][5]) {
    // boardからhashを計算する
    // もしalmost_win[hash(board)]が存在すれば、そこに書かれているとおりに駒を動かす
    // 駒を動かすにはmove_piece関数を利用せよ
    // almost_win[hash(board)]の存在チェックにはIN_ALMOST_WIN(board)マクロを利用せよ
    // 動かしたら、その動きを"4E5D"等の文字列に変換してプリントせよ
    // もしalmost_win[hash(board)]が存在しなければ、forward_of関数を利用してAIの一手先を全列挙
    // そうして得た各盤面に『マイナス1を掛け』、相手と自分の立場を入れ替えて、相手の勝利までの
    // 最長ステップ数をalmost_win[hash(board_forward)][4]によりチェックする
    // 最長ステップ数が最も大きいような動き方を選択してAIの駒を動かす
}

#undef IN_ALMOST_WIN


#endif