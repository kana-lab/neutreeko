#ifndef IS_GAME_OVER
#define IS_GAME_OVER

#include <stdio.h>
#include <string.h>


int is_game_over(int board[5][5]) {
    // boardを見て、勝敗がついていなければ0、AIが勝っていれば-1、ユーザーが勝っていれば1を返す
    // AIの駒は-1、ユーザーの駒は1で表されている
    // board変数の中身は変更しない
    // 縦3つ：i2以下　横3つ：j2以下　斜め右：i2以下、j2以下　斜め左：i2以下、j2以上
    int flg = 0;
    int arr[] = {1, -1};
    for (int i = 0; i < 5; i++) { // 左上からユーザーとAIの駒を探す
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 2; k++) {
                if (board[i][j] == arr[k]) { // 見つかったら3つ並んでいるか確認
                    if (i < 3) { // 縦
                        if ((board[i + 1][j] == arr[k]) && (board[i + 2][j] == arr[k])) {
                            if (flg == 0) {
                                flg = arr[k];
                            } else { // ユーザーもAIも3つ並んでいる（本来は有り得ない）
                                flg = 2;
                            }
                        }
                    }
                    if (j < 3) { // 横
                        if ((board[i][j + 1] == arr[k]) && (board[i][j + 2] == arr[k])) {
                            if (flg == 0) {
                                flg = arr[k];
                            } else {
                                flg = 2;
                            }
                        }
                    }
                    if ((i < 3) && (j < 3)) { // 斜め右
                        if ((board[i + 1][j + 1] == arr[k]) && (board[i + 2][j + 2] == arr[k])) {
                            if (flg == 0) {
                                flg = arr[k];
                            } else {
                                flg = 2;
                            }
                        }
                    }
                    if ((i < 3) && (j > 3)) { // 斜め左
                        if ((board[i + 1][j - 1] == arr[k]) && (board[i + 2][j - 2] == arr[k])) {
                            if (flg == 0) {
                                flg = arr[k];
                            } else {
                                flg = 2;
                            }
                        }
                    }
                }
            }
        }
    }
    return flg;
}

#endif