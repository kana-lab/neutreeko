#ifndef HASH
#define HASH


ull combination[25][25];

void init_combination() {
    for (int i=0; i<25; i++) {
        combination[0][i] = 0;
        combination[i][0] = 1;
    }
    for (int j=1; j<25; j++) {
        for (int k=1; k<25; k++) {
            combination[j][k] = combination[j-1][k-1] + combination[j-1][k];
        }
    }
}

ull hash(int board[5][5]) {
    // boardに対応する番号(ハッシュ)をunsigned long long型で返す。
    // 番号は最大で25C6 * 6C3 - 1 = 3541999となるはず。

    int n = 24;
    int r = 3; // 残りの1の個数
    int s = 3; // 残りの-1の個数
    ull board_hash = 0;
    init_combination();

    for (int i = 0; i < 25; i++, n--){
        if (board[i/5][i%5]==1){
            board_hash += combination[n][r] * combination[n-r][s];
            if (r<=0) {
                printf("不正な盤面です");
            } else {
                r--;
            }
        } else if (board[i/5][i%5]==-1){
            board_hash += combination[n][r] * combination[n-r][s]; // 0のとき
            if (r>=1) {
                board_hash += combination[n][r-1] * combination[n-r+1][s]; // 1のとき
            }
            if (s<=0) {
                printf("不正な盤面です");
            } else {
                s--;
            }
        }
    }
    if (r!=0) {
        printf("ユーザーの駒が不足しています");
    }
    if (s!=0) {
        printf("AIの駒が不足しています");
    }

    return board_hash;
}


#endif