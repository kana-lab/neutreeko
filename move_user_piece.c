#ifndef MOVE_USER_PIECE
#define MOVE_USER_PIECE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//#define DEBUG_MODE  // デバッグ時以外は定義しないこと


void debug_print(const char *msg, ...) {
#ifdef DEBUG_MODE
    va_list ap;
    va_start(ap, msg);
    vprintf(msg, ap);
    puts("");
    fflush(stdin);
    va_end(ap);
#endif
}

// エラー時にゲームを強制終了させる
// loser引数には"user"か"ai"を指定する
// その値によって, You LoseかYou Winが表示される
void abort_game(const char *loser) {
    debug_print("abort called.");
    if (!strcmp(loser, "user")) {
        puts("You Lose");
    } else if (!strcmp(loser, "ai")) {
        puts("You Win");
    }
    exit(1);
}

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

    // 100行関数！だけど内容は単純だから気にしない

    int piece;  // 動かす駒の数字 (1または-1)

    if (!strcmp(player, "user")) {
        piece = 1;
    } else if (!strcmp(player, "ai")) {
        piece = -1;
    } else {
        debug_print("in function move_piece: invalid value of the parameter player.");
        exit(1);
    }

#define OUT_OF_RANGE(x) (x < 0 || 4 < x)

    // 指定された座標が0~4に収まっているか？
    if (OUT_OF_RANGE(d.from_x) || OUT_OF_RANGE(d.from_y)
        || OUT_OF_RANGE(d.to_x) || OUT_OF_RANGE(d.to_y)) {
        debug_print("in move_piece: index out of range");
        abort_game(player);
    }

    // 移動元の座標にplayerの駒があるか？
    if (board[d.from_x][d.from_y] != piece) {
        debug_print("in move_piece: player\'s piece does not exist at the specified place.");
        abort_game(player);
    }

    // 以下移動がゲームロジックに反していないかのチェック

    int delta_x = d.to_x - d.from_x;
    int delta_y = d.to_y - d.from_y;

    if (delta_x == 0 && delta_y == 0) { // 駒が動いていない
        debug_print("in move_piece: piece must move.");
        abort_game(player);
    } else if (delta_x == 0) {  // 横方向の移動
        int step = (d.from_y < d.to_y ? 1 : -1);
        int x = d.from_x;
        int y = d.from_y + step;

        // 移動の間、障害物にぶつからないことのチェック
        for (; y != d.to_y + step; y += step)
            if (board[x][y] != 0) {
                debug_print("in move_piece: piece collides while it moves.");
                abort_game(player);
            }

        // 移動先の次に障害物がある事のチェック
        if (!OUT_OF_RANGE(y) && board[x][y] == 0) {
            debug_print("in move_piece: piece cannot stop at the specified place.");
            abort_game(player);
        }
    } else if (delta_y == 0) {  // 縦方向の移動
        int step = (d.from_x < d.to_x ? 1 : -1);
        int x = d.from_x + step;
        int y = d.from_y;

        // 移動の間、障害物にぶつからないことのチェック
        for (; x != d.to_x + step; x += step)
            if (board[x][y] != 0) {
                debug_print("in move_piece: piece collides while it moves.");
                abort_game(player);
            }

        // 移動先の次に障害物がある事のチェック
        if (!OUT_OF_RANGE(x) && board[x][y] == 0) {
            debug_print("in move_piece: piece cannot stop at the specified place.");
            abort_game(player);
        }
    } else if (abs(delta_x) == abs(delta_y)) {  // ナナメ方向の移動
        int x_step = (d.from_x < d.to_x ? 1 : -1);
        int y_step = (d.from_y < d.to_y ? 1 : -1);
        int x = d.from_x + x_step;
        int y = d.from_y + y_step;

        // 移動の間、障害物にぶつからないことのチェック
        for (; x != d.to_x + x_step && y != d.to_y + y_step; x += x_step, y += y_step)
            if (board[x][y] != 0) {
                debug_print("in move_piece: piece collides while it moves.");
                abort_game(player);
            }

        // 移動先の次に障害物がある事のチェック
        if (!OUT_OF_RANGE(x) && !OUT_OF_RANGE(y) && board[x][y] == 0) {
            debug_print("in move_piece: piece cannot stop at the specified place.");
            abort_game(player);
        }
    } else {  // 変な動きをしている
        debug_print("in move_piece: movement against the game logic.");
        abort_game(player);
    }

#undef OUT_OF_RANGE

    // 駒を動かして終了
    board[d.from_x][d.from_y] = 0;
    board[d.to_x][d.to_y] = piece;
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