# このプロジェクトについて
IS22erの2班によるボードゲーム「ニュートリーコ」用のAIです。  
面倒なのでヘッダーファイルやMakefileは作らずに、ソースコードを直接includeするようにします。  
(例) #include "is_game_over.c"

# これまでの取り決め
AIの駒を-1、ユーザーの駒を1、空きマスを0で表す。  
座標は以下。("4D"等の文字列と下のXY座標との変換はmove_user_piece関数やmove_ai_piece関数の中で行う)  
![ジオメトリ](https://user-images.githubusercontent.com/79792475/142727582-45234b19-c525-463a-aea4-45e7ef3e4ed7.png)

# AIの取る戦略
まず、盤面の見方は二通りある:
  1) 盤面をAIが駒を動かす直前であると見る
  2) 盤面をユーザーが駒を動かす直前であると見る

ある盤面が与えられたとして、これをAIが駒を動かす直前であると見ることにしよう。  
お互いが非常に賢いプレイヤーであるとすると、この盤面はすでに勝敗が決定している。  
これは、ニュートリーコが「完全情報ゲーム」であるためである。  
そこで、AIが勝つことが確定している状態を全て探し尽くしてしまおう、という戦略を取る。 

対戦において、現在の盤面がAIが勝つことが確定している盤面であればその勝ち筋通りに打つ。  
逆に、負けることが確定している盤面であれば評価関数の値を最大化するように次の手を決定する。

# AIのアルゴリズム
almost_winという集合を考える。これは、盤面をAIが駒を動かす直前の状態であると見た時に、AIの勝ちが確定しているような盤面の集合である。  
これの初期状態を次のように設定する:
  1) AIが勝っている盤面全てを列挙する
  2) そこから一手AIの駒を戻した状態として考えられる全てをalmost_winに入れる

こうすると、いまalmost_winに入っている盤面については、AIは次の一手で勝利することが出来る。

次に、checked_statesという集合を考える。これは、以下で述べるアルゴリズムの中で、すでにチェックした盤面を入れておいて二度調べるのを防ぐという役割を果たす。結果的には、盤面を「userが」駒を動かす直前であると見た時にAIの勝ちが確定しているような盤面が多く入る。これの初期状態は、勝敗がついている盤面と、不正な盤面の全て(is_game_over関数の戻り値が0でないような盤面)。

最後に、is_data_updatedという変数を用意する。これはTrueかFalseの真偽値を取り、~checked_states集合や~almost_win集合に更新があったか否かを示すものである。初期状態はTrue。

以上を用いて、勝ちが確定している状態を以下のように探索する:
  1) is_data_updated変数をFalseに変える。
  2) 盤面が取りうる全ての状態を列挙し、各状態(これをstateとする)について以下の3~4を行う。なお、stateはuserが駒を動かす直前の状態であると見なす。
  3) もしstateがchecked_states集合に入っていたら、stateはAIの勝ちが確定している盤面であり、すでに調べた盤面である。よって、2に戻って次の盤面を考える。そうでなければ4に進む。
  4) stateの盤面から、ユーザーが次に駒を動かしたときに考えられる全ての盤面を列挙する。それらが全てalmost_win集合に入っていたら、stateからどのようにユーザーが駒を動かしてもAIの勝ちとなるので、stateをchecked_states集合に追加する。そしてalmost_win集合にはstateからAIの駒を一手戻した状態として考えられる全ての盤面を追加する。最後に、is_data_updated変数をTrueに変更する。もしstateからユーザーが駒を動かしてalmost_win集合に入らない状態を作ることが出来るならば、なにもしない。2に戻りループを継続する。
  5) 2のループの終了後に、is_data_updated変数をチェックする。もしTrueならば1)に戻り、以上を繰り返す。そうでないなら終了。

こうすることで、AIが勝ち確定である状態を全てalmost_win集合に入れることが出来る。  
上のアルゴリズムでは、簡単のため勝つための駒の動かし方をメモしなかったが、実際のプログラムではメモしておきファイルに出力する事になる。

# AIが負け確定の盤面で、AIはどのように行動すべきか？
~~ある盤面Gに対して、Gの評価関数f(G)を以下のように定める:~~
  ~~1) min_ai変数を用意し、初期値を∞とする。~~
  ~~2) 5×5盤面のうち、「縦の3マスの組」は15通り。同様に、「横の3マスの組」は15通り、「ナナメの3マスの組」は18通り。これら48通りを全て列挙し、各並び(これをgoalとする)について以下の3を行う。~~
  ~~3) goalにAIの駒を持っていくための最短手数を計算し、これをLとする。Lとmin_aiを比較して、Lの方が小さければmin_aiにLを代入する。2に戻り次の並びを調べる。~~
  ~~4) 2のループを終えるとmin_aiはAIがゴールするまでの最短手数となる。同様に、相手がゴールするまでの最短手数min_userを求める。~~
  ~~5) f(G)=min_user-min_aiとして終了。~~

~~さて、いまゲームの最中であるとし、AIが駒を動かす直前であるとする。  
このとき、AIは次のように駒を動かす:~~
  ~~1) AIの駒を一手進め、ユーザーが駒を一手進めたあとの盤面として考えられる全ての盤面を列挙し、各盤面について評価関数f(G)の値を計算する。~~
  ~~2) f(G)の値を最大にするような駒の動かし方の中からランダムに1つ選び、そのとおりにAIの駒を動かす。~~

評価関数を使うのはやめました。以下の手法でAIの駒を動かします:  
  1) boardからhashを計算し、これをboard_hashとする。
  2) もしalmost_win\[board_hash\]が存在すれば、そこに書かれているとおりに駒を動かす
  3) もし存在しなければ、AIの駒を一手動かしたときの盤面を全列挙(これをstateとする)して以下を行う
  4) stateに『マイナス1を掛け』、相手と自分の立場を入れ替えて、almost_win\[hash(state*-1)\]\[4\]によりユーザーの勝利までの最長ステップ数を求める
  5) 3)のループによりユーザーの勝利までの最長ステップ数が最大となるAIの動き方を求め、そのとおりに駒を動かす
  6) もし最長ステップ数が最大となる動き方が複数ある場合は、それぞれを列挙する(これをstate_of_max_stepとする)
  7) state_of_max_stepからユーザーが一手駒を動かして到達できる盤面を全て列挙し、AI必勝の盤面となるものの数をカウントする
  8) それが最大となるようなstate_of_max_stepを求め、そのとおりにAIの駒を動かす。

※もし8においても最大となる盤面が複数あるならば、その中からランダムに動き方を選ぶ
