#ifndef FORWARD_OF
#define FORWARD_OF


int forward_of(int board[5][5], const char *player, int return_array[48][5][5]) {
    // 盤面boardについて、playerの一手後の状態として考えられる全ての盤面を返す。
    // 以下のようにback_ofをそのまま利用可能。←利用不可！(菊池さんありがとうございます)
    // 盤面の個数は最大でも24個。

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

    int l = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == x) {

                // 一手後に下に行く場合
                if (i < 4 && board[i + 1][j] == 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i + k > 3 || board[i + 1 + k][j] != 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i + k][j] = x;
                            l++;
                            break;
                        }
                    }
                }

                // 一手後に右に行く場合
                if (j < 4 && board[i][j + 1] == 0) {
                    for (int k = 1; k < 5; k++) {
                        if (j + k > 3 || board[i][j + 1 + k] != 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i][j + k] = x;
                            l++;
                            break;
                        }
                    }
                }

                // 一手後に左に行く場合
                if (j > 0 && board[i][j - 1] == 0) {
                    for (int k = 1; k < 5; k++) {
                        if (j - k < 1 || board[i][j - 1 - k] != 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i][j - k] = x;
                            l++;
                            break;
                        }
                    }
                }

                // 一手後に上に行く場合
                if (i > 0 && board[i - 1][j] == 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i - k < 1 || board[i - 1 - k][j] != 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i - k][j] = x;
                            l++;
                            break;
                        }
                    }
                }

                // 一手後に右下に行く場合
                if (i < 4 && j < 4 && board[i + 1][j + 1] == 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i + k > 3 || j + k > 3 || board[i + 1 + k][j + 1 + k] != 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i + k][j + k] = x;
                            l++;
                            break;
                        }
                    }
                }

                // 一手後に左下に行く場合
                if (i < 4 && j > 0 && board[i + 1][j - 1] == 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i + k > 3 || j - k < 1 || board[i + 1 + k][j - 1 - k] != 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i + k][j - k] = x;
                            l++;
                            break;
                        }
                    }
                }

                // 一手後に右上に行く場合
                if (i > 0 && j < 4 && board[i - 1][j + 1] == 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i - k < 1 || j + k > 3 || board[i - 1 - k][j + 1 + k] != 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i - k][j + k] = x;
                            l++;
                            break;
                        }
                    }
                }

                // 一手後に左上に行く場合
                if (i > 0 && j > 0 && board[i - 1][j - 1] == 0) {
                    for (int k = 1; k < 5; k++) {
                        if (i - k < 1 || j - k < 1 || board[i - 1 - k][j - 1 - k] != 0) {
                            return_array[l][i][j] = 0;
                            return_array[l][i - k][j - k] = x;
                            l++;
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