#include <stdio.h>
#include <string.h>

#define DEBUG_MODE  // デバッグ時以外はコメントアウトすること！

#include "init_board.c"
#include "is_game_over.c"
#include "move_user_piece.c"
#include "move_ai_piece.c"


int main(int argc, char *argv[]) {
    // 引数の個数をチェック
    if (argc != 2) {
        puts("the number of command line arguments must be 2.");
        return -1;
    }


    // 先手か後手か
    int is_user_first;
    if (!strcmp(argv[1], "0")) {
        is_user_first = 1;
    } else if (!strcmp(argv[1], "1")) {
        is_user_first = 0;
    } else {
        puts("invalid command line argument.");
        return -1;
    }


    // 盤面の状態を表す二次元配列を宣言
    int board[5][5];


    // 盤面を初期化(駒を並べる)
    init_board(board, is_user_first);


    // AIのデータの読み込み
    load_almost_win();


    // move_ai_piece.cの中でhash()関数を使っているので、
    // init_combination()を呼び出さないといけない！
    init_combination();


    // ゲームのループをまわし、勝者を決める
    int winner;
    if (is_user_first) {
        for (int i = 0; i < 300; i += 2) {  // 300手以内
            print_board_for_debug(board);
            move_user_piece(board);  // ユーザー入力を受けて駒を動かす
            if ((winner = is_game_over(board))) {
                break;
            }

            print_board_for_debug(board);
            move_ai_piece(board);  // AIの判断を受けて駒を動かす
            if ((winner = is_game_over(board))) {
                break;
            }
        }
    } else {
        for (int i = 0; i < 300; i += 2) {
            print_board_for_debug(board);
            move_ai_piece(board);
            if ((winner = is_game_over(board))) {
                break;
            }

            print_board_for_debug(board);
            move_user_piece(board);
            if ((winner = is_game_over(board))) {
                break;
            }
        }
    }


    // 勝敗の表示
    if (winner == 1) {
        puts("You Win");
    } else if (winner == -1) {
        puts("You Lose");
    } else {
        puts("Draw");
    }


    return 0;
}
