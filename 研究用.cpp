#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
#include<fstream>
#include<string>
using namespace std;
typedef long long int llint;
typedef vector<int> vint;
typedef vector<vint> vvint;
typedef vector<bool> vbool;
#define rep(i,n) for(int i=0;i<n;i++)
#define drep(i,n) for(int i=(n)-1;0<=i;i--)
//縦x横
int N, M;

void draw(int, string);

void refer(int, int, int[4]);

int chain(int, int, int);

bool Square_Search(int);

int Count(int);

void Search();

void Search2();

void Support();

int main() {
	cin.tie(nullptr);
	ios_base::sync_with_stdio(false);
	cout << "縦,横を入力" << endl;
	cin >> N >> M;
	Support();
	return 0;
}

//Search2の解析結果をもとに、プレイヤーのサポートをする
//盤面が渡されると、それが先手必勝か後手必勝かを返し、
//もし先手必勝盤面なら最善手を返す
//バグあり直せ6/10→直した6/17
void Support() {
	vint Grundy((llint)1 << (N * (M + 1) + (N + 1) * M));
	{
		string fName = "Grundy\\";
		fName += ('0' + N);
		fName += 'x';
		fName += ('0' + M);
		fName += ".txt";
		fstream fs(fName);
		if (!fs) {
			cout << "ファイルが開けませんでした" << endl;
			return;
		}

		drep(Game, 1 << (N * (M + 1) + (N + 1) * M)) {
			string buf;
			getline(fs, buf);
			if (fs.eof())break;
			if (buf[0] == '-')Grundy[Game] = -1;
			else {
				Grundy[Game] = 0;
				rep(i, buf.size()) {
					Grundy[Game] *= 10;
					Grundy[Game] += (int)(buf[i] - '0');
				}
			}
		}
	}

	cout << "今引かれている線の本数を入力" << endl;
	int x;
	cin >> x;
	llint Game = 0;
	if (x != 0) {
		cout << "線が引かれている箇所を入力" << endl;
		rep(i, x) {
			int y;
			cin >> y;
			Game += (llint)1 << y;
		}
	}
	while (1) {
		if (Grundy[Game] == -1) {
			cout << "探索対象外の盤面です" << endl;
		}
		else if (Grundy[Game]) {
			cout << "先手必勝盤面です" << endl;
			if (Grundy[Game] == 100) {
				cout << "正方形を取りまくってください、勝てます" << endl;
				break;
			}
			else {
				rep(i, N * (M + 1) + (N + 1) * M) {
					llint k = (llint)1 << i;
					if (!(Game & k) && !Grundy[Game | k]) {
						cout << i << "に線を引いてください" << endl;
						Game |= k;
						break;
					}
				}
				cout << "相手が線を引いた箇所を入力" << endl;
				int y; cin >> y;
				Game |= (llint)1 << y;
			}
		}
		else cout << "後手必勝盤面です\n諦めてください" << endl;
	}
	return;
}

//N*Mの盤面を解析し、盤面(draw)、Grundy数の順番でtest.txtに書き出す
//-1:探索対象外、0:後手必勝、100:その手番で先手必勝、その他:先手必勝
void Search() {
	
	string fileName = "Grundy_";
	fileName += ('0' + N);
	fileName += 'x';
	fileName += ('0' + M);
	fileName += ".txt";
	ofstream ofs(fileName, ofstream::app);
	if (!ofs) {
		cout << "ファイルが開けませんでした。" << endl;
		return;
	}

	//その盤面でスタートした時に
	//0なら後手必勝、1なら先手必勝、100ならその手番で勝ち
	vint Grundy((llint)1 << (N * (M + 1) + (N + 1) * M), -1);
	drep(Game, 1 << (N * (M + 1) + (N + 1) * M)) {
		if (Square_Search(Game)) {
			Grundy[Game] = -1;
		}
		else if (Count(Game) > N * M / 2) {
			Grundy[Game] = 100;
		}
		else {
			set<int>s;
			rep(i, N * (M + 1) + (N + 1) * M) {
				if (!(Game & (1 << i))) {
					s.insert(Grundy[Game | (1 << i)]);
				}
			}
			int mex = 0;
			if (s.size() == 0) {
				Grundy[Game] = 1;
			}
			else {
				while (s.find(mex) != s.end()) {
					mex++;
				}
				Grundy[Game] = mex;
			}
		}
		if (Grundy[Game] != -1) {
			draw(Game, fileName);
			ofs << Grundy[Game] << endl << endl;
		}
	}
	cout << Grundy[0] << endl;
	return;
}


//N*Mの盤面を解析し、ナンバリング逆順でGrundy数をtest.txtに書き出す
//-1:探索対象外、0:後手必勝、100:その手番で先手必勝、その他:先手必勝
void Search2() {

	string fileName = "Grundy\\";
	fileName += ('0' + N);
	fileName += 'x';
	fileName += ('0' + M);
	fileName += ".txt"; 
	ofstream ofs(fileName, ofstream::app);
	if (!ofs) {
		cout << "ファイルが開けませんでした。" << endl;
		return;
	}

	//その盤面でスタートした時に
	//0なら後手必勝、1なら先手必勝、100ならその手番で勝ち
	vint Grundy((llint)1 << (N * (M + 1) + (N + 1) * M), -1);
	drep(Game, 1 << (N * (M + 1) + (N + 1) * M)) {
		if (Square_Search(Game)) {
			Grundy[Game] = -1;
		}
		else if (Count(Game) > N * M / 2) {
			Grundy[Game] = 100;
		}
		else {
			set<int>s;
			rep(i, N * (M + 1) + (N + 1) * M) {
				if (!(Game & (1 << i))) {
					s.insert(Grundy[Game | (1 << i)]);
				}
			}
			int mex = 0;
			if (s.size() == 0) {
				Grundy[Game] = 1;
			}
			else {
				while (s.find(mex) != s.end()) {
					mex++;
				}
				Grundy[Game] = mex;
			}
		}
		ofs << Grundy[Game] << endl;
	}
	cout << Grundy[0] << endl;
	return;
}

//Gameを描画する
void draw(int Game, string fileName) {
	ofstream ofs(fileName, ofstream::app);
	rep(i, 2 * N + 1) {
		rep(j, 2 * M + 1) {
			if (i % 2 == 0) {
				if (j % 2 == 0)ofs << " ";
				else {
					if (Game & (1 << (i / 2 * M + j / 2)))ofs << "-";
					else ofs << " ";
				}
			}
			else {
				if (j % 2 == 0) {
					if (Game & (1 << (M * (N + 1) + j / 2 * N + i / 2)))ofs << "|";
					else ofs << " ";
				}
				else ofs << " ";
			}
		}
		ofs << endl;
	}
	return;
}

//(x,y)の正方形の4辺の番号を求める
//O(1)
void refer(int x, int y, int num[4]) {
	num[0] = x * M + y;
	num[1] = (x + 1) * M + y;
	num[2] = M * (N + 1) + y * N + x;
	num[3] = M * (N + 1) + (y + 1) * N + x;
}

//(x,y)の正方形のどこかから塗るときの最大点数
//O(NM)
int chain(int Game, int x, int y) {
	if (x < 0 || N <= x || y < 0 || M <= y)return Count(Game);
	int udlr[4];
	refer(x, y, udlr);
	int tmp = 0;
	int memo = 0;
	int dx[4] = { -1,1,0,0 }, dy[4] = { 0,0,-1,1 };
	rep(k, 4) {
		if (Game & (1 << udlr[k]))tmp++;
		else memo = k;
	}
	if (tmp == 3) {
		return 1 + chain(Game | (1 << udlr[memo]), x + dx[memo], y + dy[memo]);
	}
	if (tmp == 4) {
		return 1 + Count(Game);
	}
	return 0;
}

//もしこの手番なら一気にいくつのマスを取れるかを計算する
//(やばい例外あり)(直せ)4/22→治った！天才！5/13
//O(N²M²)
int Count(int Game) {
	int ans = 0;
	rep(i, N) {
		rep(j, M) {
			int cnt = 0;
			int udlr[4];
			refer(i, j, udlr);
			int tmp = 0;
			int memo = 0;
			int dx[4] = { -1,1,0,0 }, dy[4] = { 0,0,-1,1 };
			rep(k, 4) {
				if (Game & (1 << udlr[k]))tmp++;
				else memo = k;
			}
			if (tmp == 3) {
				cnt = 1 + chain(Game | (1 << udlr[memo]), i + dx[memo], j + dy[memo]);
			}
			ans = max(cnt, ans);
		}
	}
	return ans;
}

//盤面に既に正方形があるかないかを返す
//O(NM)
bool Square_Search(int Game) {
	rep(i, N) {
		rep(j, M) {
			int udlr[4];
			refer(i, j, udlr);
			if ((Game & (1 << udlr[0])) &&
				(Game & (1 << udlr[1])) &&
				(Game & (1 << udlr[2])) &&
				(Game & (1 << udlr[3]))) {
				return true;
			}
		}
	}
	return false;
}