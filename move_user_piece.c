#ifndef MOVE_USER_PIECE
#define MOVE_USER_PIECE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define DEBUG_MODE  // デバッグ時以外は定義しないこと


void debug_print(const char *msg, ...) {
#ifdef DEBUG_MODE
    va_list ap;
    va_start(ap, msg);
    vprintf(msg, ap);
    puts("");
    va_end(ap);
#endif
}

// エラー時にゲームを強制終了させる
// loser引数には"user"か"ai"を指定する
// その値によって, You LoseかYou Winが表示される
void abort_game(const char *loser) {
    debug_print("abort called");
    if (!strcmp(loser, "user")) {
        puts("You Lose");
    } else if (!strcmp(loser, "ai")) {
        puts("You Win");
    }
    exit(1);
}

// 盤面boardにおいて, board[from_x][from_y]にあるplayerの駒をboard[to_x][to_y]に動かす
// player引数には"user"か"ai"を指定する(それ以外が指定されたときはエラーとなるので注意)
// ゲームロジックに反した動きを指定した場合はplayerの負けとなり, ゲームが強制終了する
//void move_piece(int board[5][5], const char *player, int from_x, int from_y, int to_x, int to_y) {
//    int piece;
//
//    if (!strcmp(player, "user")) {
//        piece = 1;
//    } else if (!strcmp(player, "ai")) {
//        piece = -1;
//    } else {  // ERROR
//        puts("in function move_piece: invalid value of the parameter player");
//        exit(1);
//    }
//
//    if (from_x < 0 || 4 < from_x || from_y < 0 || 4 < from_y
//        || to_x < 0 || 4 < to_x || to_y < 0 || 4 < to_y) {
//#ifdef DEBUG_MODE
//        puts("in move_piece: index out of range");
//#endif
//        abort_game(player);  // index out of range
//    }
//
//    const int delta_x = to_x - from_x;
//    const int delta_y = to_y - from_y;
//
//    if (!(-1 <= delta_x && delta_x <= 1 && -1 <= delta_y && delta_y <= 1)
//        || delta_x == 0 && delta_y == 0) {
//#ifdef DEBUG_MODE
//        puts("in move_piece: movement against the game logic");
//#endif
//        abort_game(player);  // movement against the game logic
//    }
//
//    if (board[from_x][from_y] != piece || board[to_x][to_y] != 0) {
//#ifdef DEBUG_MODE
//        puts("the specified place is filled");
//#endif
//        abort_game(player);  // cannot move the piece
//    }
//
//    board[from_x][from_y] = 0;
//    board[to_x][to_y] = piece;
//}

typedef struct {
    int from_x;
    int from_y;
    int to_x;
    int to_y;
} Delta;

// 盤面boardにおいて, board[d.from_x][d.from_y]にあるplayerの駒をboard[d.to_x][d.to_y]に動かす
// player引数には"user"か"ai"を指定する (それ以外が指定されたときはエラーとなるので注意)
// ゲームロジックに反した動きを指定した場合はplayerの負けとなり, ゲームが強制終了する
void move_piece(int board[5][5], const char *player, Delta d) {
    int piece;  // 動かす駒の数字 (1または-1)

    if (!strcmp(player, "user")) {
        piece = 1;
    } else if (!strcmp(player, "ai")) {
        piece = -1;
    } else {
        debug_print("in function move_piece: invalid value of the parameter player");
        exit(1);
    }

#define OUT_OF_RANGE(x) (x < 0 || 4 < x)

    // 指定された座標が0~4に収まっているか？
    if (OUT_OF_RANGE(d.from_x) || OUT_OF_RANGE(d.from_y)
        || OUT_OF_RANGE(d.to_x) || OUT_OF_RANGE(d.to_y)) {
        debug_print("in move_piece: index out of range");
        abort_game(player);
    }

#undef OUT_OF_RANGE

    // 移動元の座標にplayerの駒があるか？ 移動先の座標は空いているか？
    if (board[d.from_x][d.from_y] != piece || board[d.to_x][d.to_y] != 0) {
        debug_print("in move_piece: the specified place is filled");
        abort_game(player);
    }

    // 移動がゲームロジックに反していないか？
}

void move_user_piece(int board[5][5]) {
    // ユーザーからの入力をオーバーフロー対策をしつつ受ける
    // 入力は4文字ジャストでないとダメ
    char buf[32] = {};
    if (fgets(buf, 32, stdin) == NULL || strlen(buf) != 5) {
        debug_print("in move_user_piece: invalid input");
        abort_game("user");
    }

    // 受け取った座標を内部的な座標に変換
    Delta d = {
            .from_x = 5 - (buf[0] - '0'),
            .from_y = buf[1] - 'A',
            .to_x = 5 - (buf[2] - '0'),
            .to_y = buf[3] - 'A',
    };

    move_piece(board, "user", d);
}


// below are for debug
//void print_board(int board[5][5]) {
//    for (int i = 0; i < 5; ++i) {
//        for (int j = 0; j < 5; ++j)
//            printf("%d ", board[i][j]);
//        puts("");
//    }
//}
//
//int main() {
//    int board[5][5] = {
//            {0, 1,  0,  1,  0},
//            {0, 0,  -1, 0,  0},
//            {0, 0,  0,  0,  0},
//            {0, 0,  1,  0,  0},
//            {0, -1, 0,  -1, 0}
//    };
//    for (;;) {
//        print_board(board);
//        move_user_piece(board);
//    }
//    return 0;
//}


#endif