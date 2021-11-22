
//
// このプログラムは使用しない。
//

/*
#ifndef MOVE_AI_PIECE
#define MOVE_AI_PIECE

// このファイルでは主に評価関数を用いたアルゴリズムの実装を行う
// 優先度高め

typedef struct {
    int x;
    int y;
} Geometry;

int min_step(int board[5][5], Geometry from, Geometry to) {
    // fromからtoへ駒を動かすための最短ステップ数を返す
    // たどり着けないなら-1を返す
    // fromの位置に駒が無い場合エラーメッセージを出す
    // toの位置に駒がある場合エラーメッセージを出す
}

#define GOAL_COUNT 48
Geometry all_goals[GOAL_COUNT][3];  // 縦、横、ナナメの連続する3マスの座標を格納

void init_all_goals() {
    // all_goals配列を埋める
    // 5×5盤面の縦、横、ナナメの連続する3マスの座標を入れる
    // 縦15通り、横15通り、ナナメ18通りの系48通りあるはず
}

int min_step_to_goal(int board[5][5], int player_piece, const Geometry goal[3]) {
    // 3つの駒を動かしてgoalの位置に移動させるのに必要な最短ステップ数を返す
    // たどり着けないなら-1を返す
    // min_step関数を利用せよ
    // AIの駒を動かす場合player_piece引数には-1が指定される
    // userの駒を動かす場合は1が指定される
    // goal変数の中身およびboard変数の中身は変更しないこと
}

#define INFINITY 100000

int evaluate(int board[5][5]) {
    // 盤面の評価関数f(G)を実装せよ
    // min_step_to_goal関数およびall_goals配列を利用せよ
    // all_goals配列は初期化済みだと仮定してよい
    // 戻り値はf(G)の値である、無限はINFINITYを用いよ
}

// 以下の関数はcomplete_analysis.cのforward_of関数と共通
int forward_of(int board[5][5], const char *player, int return_array[32][5][5]) {
    // 盤面boardについて、playerの一手後の状態として考えられる全ての盤面を
    // return_arrayに入れて、その盤面の個数を戻り値として返す。
    // 盤面の個数は最大でも8 * 3 = 24個になるはず。
    // player引数は"user"か"ai"かのどちらか。
}

// 以下のDelta構造体、およびdelta_of関数はcomplete_analysis.cのdelta_of関数と共通
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

void move_ai_piece_using_f(int board[5][5]) {
    // 評価関数のアルゴリズムを用いてAIの駒を動かす
    // move_user_piece.cのmove_piece関数を利用せよ
    // forward_of関数、delta_of関数、evaluate関数を利用せよ
}

// main関数中のmove_ai_piece関数の実装はcomplete_analysis.cが完成した後におこなう

#endif
*/