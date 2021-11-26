#ifndef BACK_OF
#define BACK_OF


int back_of(int board[5][5], const char *player, int return_array[48][5][5]) {
    // 盤面boardについて、playerの一手前の状態として考えられる全ての盤面を
    // return_arrayに入れて、その盤面の個数を戻り値として返す。
    // 盤面の個数は最大でも8 * 3 = 24個になるはず。←嘘です！！最大48通り
    // 障害物がないと止まれないので、それも考慮して戻れる盤面を探すこと
    // player引数は"user"か"ai"かのどちらか。

    int x;
    if (strcmp(player, "user") == 0) x = 1;
    else if (strcmp(player, "ai") == 0) x = -1;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 48; k++) {
                return_array[k][i][j] = board[i][j];
            }
        }
    }

    int l = 0;  // 一手前の状態の個数

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == x) {

                // 一手前に下にあった場合
                if (i == 0 || board[i - 1][j] != 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i + k < 5 && board[i + k][j] == 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i + k][j] = x;
                            l++;
                        } else {
                            break;
                        }
                    }
                }

                // 一手前に右にあった場合
                if (j == 0 || board[i][j - 1] != 0) {
                    for (int k = 1; k < 5; k++) {
                        if (j + k < 5 && board[i][j + k] == 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i][j + k] = x;
                            l++;
                        } else {
                            break;
                        }
                    }
                }

                // 一手前に左にあった場合
                if (j == 4 || board[i][j + 1] != 0) {
                    for (int k = 1; k < 5; k++) {
                        if (j - k >= 0 && board[i][j - k] == 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i][j - k] = x;
                            l++;
                        } else {
                            break;
                        }
                    }
                }

                // 一手前に上にあった場合
                if (i == 4 || board[i + 1][j] != 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i - k >= 0 && board[i - k][j] == 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i - k][j] = x;
                            l++;
                        } else {
                            break;
                        }
                    }
                }

                // 一手前に右下にあった場合
                if (i == 0 || j == 0 || board[i - 1][j - 1] != 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i + k < 5 && j + k < 5 && board[i + k][j + k] == 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i + k][j + k] = x;
                            l++;
                        } else {
                            break;
                        }
                    }
                }

                // 一手前に左下にあった場合
                if (i == 0 || j == 4 || board[i - 1][j + 1] != 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i + k < 5 && j - k >= 0 && board[i + k][j - k] == 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i + k][j - k] = x;
                            l++;
                        } else {
                            break;
                        }
                    }
                }

                // 一手前に右上にあった場合
                if (i == 4 || j == 0 || board[i + 1][j - 1] != 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i - k >= 0 && j + k < 5 && board[i - k][j + k] == 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i - k][j + k] = x;
                            l++;
                        } else {
                            break;
                        }
                    }
                }

                // 一手前に左上にあった場合
                if (i == 4 || j == 4 || board[i + 1][j + 1] != 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i - k >= 0 && j - k >= 0 && board[i - k][j - k] == 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i - k][j - k] = x;
                            l++;
                        } else {
                            break;
                        }
                    }
                }
            }
        }
    }

    return l;
}


#endif