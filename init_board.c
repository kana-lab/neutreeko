void init_board(int board[5][5], int is_user_first) {
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            board[i][j] = 0;

    if (is_user_first == 0) {
        board[0][1] = board[0][3] = board[3][2] = 1;
        board[1][2] = board[4][1] = board[4][3] = -1;
    } else if (is_user_first == 1) {
        board[0][1] = board[0][3] = board[3][2] = -1;
        board[1][2] = board[4][1] = board[4][3] = 1;
    }
}
