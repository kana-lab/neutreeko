#ifndef DELTA_OF
#define DELTA_OF


Delta delta_of(int board_before[5][5], int board_after[5][5]) {
    // 盤面board_beforeとboard_afterを受け取って、駒がどの座標からどの座標へ動いたかを返す
    // ただし、board_beforeとboard_afterの差異は、駒1つが一手分動いただけの差であるとする
    // エラーチェックは余裕があれば…

    int from_x, from_y, to_x, to_y;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board_before[i][j] != board_after[i][j]) {
                if (board_before[i][j] == 0) {
                    to_x = i;
                    to_y = j;
                } else {
                    from_x = i;
                    from_y = j;
                }
            }
        }
    }

    Delta d;
    d.from_x = from_x;
    d.from_y = from_y;
    d.to_x = to_x;
    d.to_y = to_y;

    return d;
}


#endif