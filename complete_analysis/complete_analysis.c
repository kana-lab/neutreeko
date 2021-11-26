#include "../is_game_over.c"

//
// このファイルではニュートリーコの完全解析を行うアルゴリズムを実装する
//

// 3542000通りの盤面が収まれば良いが、念の為大きめに取る
#define SIZE_OF_SET 4000000
#define ACTUAL_SIZE 3542000

typedef unsigned long long ull;


/*******************************************************************************************
 * almost_win
 *     盤面をAIが駒を動かす直前の状態であると見た時に、AIの勝ちが確定しているような盤面の集合。
 *     以下の定義から分かるとおり、盤面Gに対して、almost_win[hash(G)]には5個の整数を入れることが
 *     出来るが、最初の2個には盤面Gから次にAIが動かす駒の座標を、次の2個には駒の移動先の座標を、
 *     そして最後の1個にはAIが勝利するまでの最長ステップ数を入れる。
 * checked_states
 *     すでにチェックした盤面を入れておいて二度調べるのを防ぐという役割を果たす。
 *     盤面Gを既にチェックしてある場合には、checked_states[hash(G)]は1であり、そうでなければ0。
 *     初期値はいずれかのプレイヤーが勝利している盤面と、不正な盤面の全て。
 * is_data_updated
 *     TrueかFalseの真偽値を取り、almost_win集合に更新があったか否かを示す。初期値はTrue。
 *     以下のプログラムでは、is_data_updatedは整数であるから、もののついでにalmost_winに
 *     どれだけの回数追加があったかを入れている。
 *******************************************************************************************/
int almost_win[SIZE_OF_SET][5];  // 160MB, 各要素には駒の移動元/移動先の座標、及び最長ステップ数を入れる
int checked_states[SIZE_OF_SET];  // 32MB, 個々の要素には1または0を入れ、TrueとFalseを表現
int is_data_updated = 1;

int all_state[SIZE_OF_SET][5][5];  // 800MB, 全ての盤面を列挙して入れておく


typedef struct {
    int from_x;
    int from_y;
    int to_x;
    int to_y;
} Delta;

#include "hash.c"
#include "init_all_state.c"
#include "back_of.c"
#include "forward_of.c"
#include "delta_of.c"


int add_to_almost_win(int board_of_user_turn[5][5], int step) {
    // 引数board_of_user_turnは、ユーザーが駒を動かす直前であると見た時の盤面である。
    // board_of_user_turnから一手戻した状態全てをalmost_win集合に追加する。
    // almost_winに新たに追加された状態の数を戻り値として返す。

    int add_count = 0;

    int back[48][5][5];
    int back_count = back_of(board_of_user_turn, "ai", back);
    for (int i = 0; i < back_count; ++i) {
        int *ptr = almost_win[hash(back[i])];
        if (!ptr[0] && !ptr[1] && !ptr[2] && !ptr[3]) {
            Delta d = delta_of(back[i], board_of_user_turn);
            ptr[0] = d.from_x;
            ptr[1] = d.from_y;
            ptr[2] = d.to_x;
            ptr[3] = d.to_y;
            ptr[4] = step;
            ++add_count;
        }
    }

    return add_count;
}

// almost_win集合とchecked_states集合を初期化する
// この関数が呼ばれるよりも前にall_stateが初期化されている必要がある
void init_arrays() {
    for (ull i = 0; i < ACTUAL_SIZE; ++i) {
        int kind = is_game_over(all_state[i]);
        if (kind != 0) {  // 盤面iは勝敗がついているか、もしくは不正な盤面
            checked_states[i] = 1;
            if (kind == -1) {  // 盤面iはAIの勝利である
                add_to_almost_win(all_state[i], 1);

                // 2021/11/26(金)追記
                // AIの勝利である盤面Gについてはalmost_win[hash(G)]には5を入れておく
                for (int j = 0; j < 4; ++j)
                    almost_win[i][j] = 5;
            }
        }
    }
}

void output_almost_win() {
    // いい感じのフォーマットを考えて、almost_win配列をファイル出力する。
    // 単にalmost_winの中の0~4を文字の0~4に変換してテキストファイルに吐き出すだけでも良いかも

    char filename[32] = "perfect_move.dat";
    FILE *fp;

    // ファイルオープン (エラー時にファイル名の再入力を促す)
    while ((fp = fopen(filename, "w")) == NULL) {
        printf("could not open the file \"%s\"\n", filename);
        puts("please specify a new filename:");
        memset(filename, 0, 32);
        scanf("%31s", filename);
    }

    // 数字をテキストに変換して出力
    for (ull i = 0; i < ACTUAL_SIZE; ++i) {
        int *ptr = almost_win[i];
        fprintf(fp, "%d %d %d %d %d\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4]);
    }

    fclose(fp);
}

#define IN_ALMOST_WIN(board_hash) ( \
       almost_win[board_hash][0] \
    || almost_win[board_hash][1] \
    || almost_win[board_hash][2] \
    || almost_win[board_hash][3])

int main() {
    printf("initializing various arrays...");
    fflush(stdout);

    init_combination();
    init_all_state();
    init_arrays();

    puts("done");
    puts("analyzing the game...");
    fflush(stdout);

    while (is_data_updated) {
        is_data_updated = 0;

        for (ull i = 0; i < ACTUAL_SIZE; ++i) {
            int (*state)[5] = all_state[i];  // 盤面の取りうる全ての状態を列挙

            if (checked_states[hash(state)])
                continue;  // stateがchecked_statesに入っていたら戻ってループ継続

            // stateからユーザーが次に駒を動かした時に考えられる状態を全列挙
            int forward[48][5][5];
            int forward_count = forward_of(state, "user", forward);

            // forwardの全てがalmost_win集合に入っているか？ (ついでに勝利までの最長ステップ数も求める)
            int longest_step = 0;
            for (int j = 0; j < forward_count; ++j) {
                ull forward_hash = hash(forward[j]);
                if (!IN_ALMOST_WIN(forward_hash))
                    goto LOOP_CONTINUE;
                if (longest_step < almost_win[forward_hash][4])
                    longest_step = almost_win[forward_hash][4];
            }

            // forwardの全てがalmost_win集合に入っている場合
            checked_states[hash(state)] = 1;
            is_data_updated += add_to_almost_win(state, longest_step + 1);

            // forwardの全てがalmost_win集合に入っているわけではない場合
            LOOP_CONTINUE:;
        }

        printf("1 loop is finished. update: %d\n", is_data_updated);
        fflush(stdout);
    }

    puts("all analysis is finished.");
    printf("writing the result to a file...");
    fflush(stdout);

    output_almost_win();

    puts("done");
    fflush(stdout);

    return 0;
}

#undef IN_ALMOST_WIN
