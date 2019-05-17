// 3d.cpp    by kakira9618
// 
// To complile and run, do:
// $ g++ -lncurses -lm 3d.cpp -o 3d
// $ ./3d
// for more information, check README.md

#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

const int H = 40; // コンソールの縦の文字数
const int W = 80; // コンソールの横の文字数
const double PP = ((double)H) / W; // ピクセル縦横比
std::vector<string> G; // 画面バッファ。この内容がコンソールに出力される

// ブレゼンハムのアルゴリズムを用いて線分描画
// https://ja.wikipedia.org/wiki/%E3%83%96%E3%83%AC%E3%82%BC%E3%83%B3%E3%83%8F%E3%83%A0%E3%81%AE%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0
void drawLine(int x0, int y0, int x1, int y1) {
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx, sy;
	if (x0 < x1) sx = 1; else sx = -1;
	if (y0 < y1) sy = 1; else sy = -1;
	int err = dx - dy;
	while(1) {
		if (x0 < 0 || x0 >= W || y0 < 0 || y0 >= H) break;
		G[y0][x0] = '*';
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

// Gの内容をコンソールに表示
void updateScreen() {
	move(0, 0); // カーソルを (0, 0) に移動
	for(int i = 0; i < G.size(); i++) {
		addstr(G[i].c_str()); // Gの内容をそのまま出力
	}
	refresh(); // 確実にコンソールに出力内容を反映させる
}

// Gの中身を空白で塗りつぶす
void clearScreen() {
	for(int i = 0; i < G.size(); i++) {
		for(int j = 0; j + 1 < G[i].size(); j++) {
			G[i][j] = ' ';
		}
	}
}

// 実際の描画処理
void draw() {
	static double time = 0;
	const double r = 20.0;
	const double p = 0.5;
	double tx = (double)W / 2, ty = 2.0 * H / 3;

	for(int i = 0; i < 4; i++) {
		double t1 = (90 * i + time) / 180.0 * M_PI;
		double t2 = t1 + M_PI / 2;
		drawLine(tx + r * cos(t1), ty + PP * p * r * sin(t1),
		         tx + r * cos(t2), ty + PP * p * r * sin(t2));
		drawLine(tx + r * cos(t1), ty + PP * p * r * sin(t1),
		         tx, ty - PP * 1.5 * r); // * 1.5 を入れなくてもOK(見た目の問題)
	}

	time++;
}

// Gの初期化
void init() {
	G.resize(H);
	for(int i = 0; i < G.size(); i++) {
		G[i].resize(W + 1, ' ');
		G[i].back() = '\n';
	}
}

int main() {
	initscr(); // ncurses関連の初期化
	init(); // 画面バッファGの初期化

	while(1) {
		clearScreen(); // Gを真っ白にする
		draw(); // Gに絵を書き込む
		updateScreen(); // Gをコンソールに反映させる
		usleep(10000); // ちょっと待つ
	}
	
	endwin(); // ncurses関連の終了処理
	return 0;
}
