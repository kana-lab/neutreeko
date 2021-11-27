#ifndef MOVE_AI_PIECE
#define MOVE_AI_PIECE

typedef unsigned long long ull;

#define ACTUAL_SIZE 3542000

int almost_win[ACTUAL_SIZE][5];

#include "move_user_piece.c"
#include "complete_analysis/hash.c"
#include "complete_analysis/forward_of.c"
#include "complete_analysis/delta_of.c"


void load_almost_win() {
    // almost_win配列をファイルから読み込む

    const char *filename = "complete_analysis/perfect_move.dat";
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

void display_movement(Delta d) {
    // Delta型の引数dで指定された駒の動きを、"5C3E"等の文字列に変換してプリントする

    char from_x = '5' - (char) d.from_x;
    char from_y = 'A' + (char) d.from_y;
    char to_x = '5' - (char) d.to_x;
    char to_y = 'A' + (char) d.to_y;
    printf("%c%c%c%c\n", from_x, from_y, to_x, to_y);
}

#define IN_ALMOST_WIN(board_hash) ( \
       almost_win[board_hash][0] \
    || almost_win[board_hash][1] \
    || almost_win[board_hash][2] \
    || almost_win[board_hash][3])

int user_step_to_win(int board_of_user_turn[5][5]) {
    // board_of_user_turnはユーザーが駒を動かす直前の盤面
    // ユーザーの勝ちが確定している時に、あと何ステップで勝利するのかを返す
    // ユーザーの勝ちが確定していない盤面が渡された場合はエラーメッセージを出す

    int state[5][5];

    // 盤面にマイナス1を掛け、AIとユーザーの立場を入れ替える
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            state[i][j] = board_of_user_turn[i][j] * -1;

    ull hash_of_state = hash(state);
    if (!IN_ALMOST_WIN(hash_of_state))
        debug_print("in user_step_to_win: state * -1 is NOT in almost_win.");

    return almost_win[hash_of_state][4];
}

int count_ai_win_state(int board_of_user_turn[5][5]) {
    // board_of_user_turnはユーザーが駒を動かす直前の盤面
    // board_of_user_turnからユーザーが一手駒を進めた時に、AIの必勝状態となるような駒の
    // 動かし方の個数を戻り値として返す

    int count = 0;

    int forward[48][5][5];
    int forward_count = forward_of(board_of_user_turn, "user", forward);
    for (int i = 0; i < forward_count; ++i)
        if (IN_ALMOST_WIN(hash(forward[i])))
            ++count;

    return count;
}

void move_ai_piece(int board[5][5]) {
    // githubのREADMEの一番下に書いてあるアルゴリズムを実装する
    // 駒を動かすにはmove_piece関数を利用せよ
    // almost_win[board_hash]の存在チェックにはIN_ALMOST_WIN(board_hash)関数を利用せよ
    // 一手先の盤面の列挙にはforward_of関数を利用せよ
    // 2つの盤面から駒の動きを求めるにはdelta_of関数を利用せよ
    // 駒を動かしたら、その動きを"5C3E"等の文字列に変換しプリントせよ

    ull board_hash = hash(board);
    if (IN_ALMOST_WIN(board_hash)) {  // AIの必勝状態の場合
        Delta d = {
                .from_x=almost_win[board_hash][0],
                .from_y=almost_win[board_hash][1],
                .to_x=almost_win[board_hash][2],
                .to_y=almost_win[board_hash][3]
        };
        move_piece(board, "ai", d);
        display_movement(d);
    } else {  // AIの必敗状態の場合
        int forward[48][5][5];
        int forward_count = forward_of(board, "ai", forward);

        // AIの駒を一手動かした時の盤面を全列挙(これをstateとする)
        // ユーザーが勝つまでの最長ステップ数を求める
        int longest_step = 0;
        for (int i = 0; i < forward_count; ++i) {
            int step = user_step_to_win(forward[i]);
            if (longest_step < step)
                longest_step = step;
        }

        // 最長ステップ数が最大となる盤面をこれに入れる
        int state_of_max_step[48][5][5];
        int index = 0;
        for (int i = 0; i < forward_count; ++i)
            if (longest_step == user_step_to_win(forward[i]))
                memcpy(state_of_max_step[index++], forward[i], 25 * sizeof(int));

        // state_of_max_stepの中で、ユーザーが次に駒を動かした時にAIの勝ち筋が最も多くなるものを探る
        int most_suitable_state[5][5];
        int max_count_of_ai_win = 0;
        for (int i = 0; i < index; ++i) {
            int count = count_ai_win_state(state_of_max_step[i]);
            if (max_count_of_ai_win < count) {
                max_count_of_ai_win = count;
                memcpy(most_suitable_state, state_of_max_step[i], 25 * sizeof(int));
            }
        }

        // boardからstate_of_max_stepに盤面を変えるためのAIの動きを求め、そのとおりに駒を動かす
        Delta d = delta_of(board, most_suitable_state);
        move_piece(board, "ai", d);
        display_movement(d);
    }
}

#undef IN_ALMOST_WIN


#endif