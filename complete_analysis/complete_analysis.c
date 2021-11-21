#include "../is_game_over.c"

// このファイルではニュートリーコの完全解析を行うアルゴリズムを実装する
// 優先度低め

// 3542000通りの盤面が収まれば良いが、念の為大きめに取る
#define SIZE_OF_SET 4000000
#define ACTUAL_SIZE 3542000

typedef unsigned long long ull;

int almost_win[SIZE_OF_SET][4];  // 128MB, 4つの要素には駒の移動元の座標、移動先の座標を入れる
int checked_states[SIZE_OF_SET];  // 32MB, 個々の要素には1または0を入れ、TrueとFalseを表現
int is_data_updated = 1;

int all_state[SIZE_OF_SET][5][5];  // 800MB, 全ての盤面を列挙して入れておく


ull hash(int board[5][5]) {
    // boardに対応する番号(ハッシュ)をunsigned long long型で返す。
    // 番号は最大で25C6 * 6C3 = 3542000となるはず。
}

void init_all_state() {
    // 考えうる全ての盤面を列挙し、all_state配列に格納する。
    // 盤面Gは、all_state[hash(G)]に(コピーして)入れる。
    // 必ず、列挙した盤面の数がACTUAL_SIZEと一致しているかどうかのエラーチェックをすること。
}

int back_of(int board[5][5], const char *player, int return_array[32][5][5]) {
    // 盤面boardについて、playerの一手前の状態として考えられる全ての盤面を
    // return_arrayに入れて、その盤面の個数を戻り値として返す。
    // 盤面の個数は最大でも8 * 3 = 24個になるはず。
    // player引数は"user"か"ai"かのどちらか。
}

int forward_of(int board[5][5], const char *player, int return_array[32][5][5]) {
    // 盤面boardについて、playerの一手後の状態として考えられる全ての盤面を返す。
    // 以下のようにback_ofをそのまま利用可能。
    return back_of(board, player, return_array);
}

typedef struct {
    int from_x;
    int from_y;
    int to_x;
    int to_y;
} Delta;

Delta delta_of(int board_before[5][5], int board_after[5][5]) {
    // 盤面board_beforeとboard_afterを受け取って、駒がどの座標からどの座標へ動いたかを返す
    // ただし、board_beforeとboard_afterの差異は、駒1つが一手分動いただけの差であるとする
    // エラーチェックは余裕があれば…
}

void add_to_almost_win(int board_of_user_turn[5][5]) {
    // 引数board_of_user_turnは、ユーザーが駒を動かす直前であると見た時の盤面である。
    // board_of_user_turnから一手戻した状態全てをalmost_win集合に追加する。
    int back[32][5][5];
    int back_count = back_of(board_of_user_turn, "ai", back);
    for (int i = 0; i < back_count; ++i) {
        int[4] ptr = almost_win[hash(back[i])];
        if (!ptr[0] && !ptr[1] && !ptr[2] && !ptr[3]) {
            Delta d = delta_of(back[i], board_of_user_turn);
            ptr[0] = d.from_x;
            ptr[1] = d.from_y;
            ptr[2] = d.to_x;
            ptr[3] = d.to_y;
        }
    }
}

// almost_win集合とchecked_states集合を初期化する
// この関数が呼ばれるよりも前にall_stateが初期化されている必要がある
void init_arrays() {
    for (ull i = 0; i < ACTUAL_SIZE; ++i) {
        int kind = is_game_over(all_state[i]);
        if (kind != 0) {  // 盤面iは勝敗がついているか、もしくは不正な盤面
            checked_states[i] = 1;
            if (kind == -1) {  // 盤面iはAIの勝利である
                add_to_almost_win(all_state[i]);
            }
        }
    }
}

void output_almost_win() {
    // いい感じのフォーマットを考えて、almost_win配列をファイル出力する。
}

#define IN_ALMOST_WIN(x) ( \
       almost_win[hash(x)][0] \
    || almost_win[hash(x)][1] \
    || almost_win[hash(x)][2] \
    || almost_win[hash(x)][3])

int main() {
    init_all_state();
    init_arrays();

    while (is_data_updated) {
        is_data_updated = 0;
        for (ull i = 0; i < ACTUAL_SIZE; ++i) {
            int[5][5] state = all_state[i];  // 盤面の取りうる全ての状態を列挙

            if (checked_states[hash(state)])
                continue;  // stateがchecked_statesに入っていたら戻ってループ継続

            // stateからユーザーが次に駒を動かした時に考えられる状態を全列挙
            int forward[32][5][5];
            int forward_count = forward_of(state, "user", forward);

            // forwardの全てがalmost_win集合に入っているか？
            for (int j = 0; j < forward_count; ++j) {
                if (!IN_ALMOST_WIN(forward[j]))
                    goto LOOP_CONTINUE;
            }

            // forwardの全てがalmost_win集合に入っている場合
            checked_states[hash(state)] = 1;
            add_to_almost_win(state);
            is_data_updated = 1;

            // forwardの全てはalmost_winに入っているわけではない場合
            LOOP_CONTINUE:;
        }
    }

    output_almost_win();

    return 0;
}

#undef IN_ALMOST_WIN