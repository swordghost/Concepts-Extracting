#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cstring>
#include <cmath>
#include <cstdlib>

#include <List>

using namespace std;

char *path = "D:\\CEData\\"; // 数据文件夹
int NoC, NoQ, Limit; // 叶节点数、查询数、背包容量
int *C = NULL, *cost = NULL; // 概念引用计数数组及代价数组
int *worth = NULL; // 价值数组
list<int> *last = NULL; // 结果输出用辅助数组

void destroy() {
	//if (C != NULL) delete C;
	//if (cost != NULL) delete cost;
	//if (worth != NULL) delete worth;
	//if (last != NULL) delete last;
}

int main() {
	ifstream tfin;
	ofstream tfout;
	int n;
	char Num[12]; // 用于itoa的数组
	string qn, tn; // 查询数据和树结构数据的文件名
	string on; // 输出文件文件名
	string temp;

	// 读入数据
	cin >> n;
	_itoa_s(n, Num, 10);
	cout << "选定数据集：" << Num << endl;
	qn = tn = on = Num;
	qn = path + qn + "Qry.dat";
	tn = path + tn + "Tre.dat";
	on = path + on + "Pck.out";
	tfin.open(tn);
	if (tfin) {
		tfin >> NoC;
		cout << "概念数：" << NoC << endl;
		tfin >> n;
		Limit = min(n * 10, 10000);
		C = new int[NoC];
		cost = new int[NoC];
		worth = new int[Limit + 1];
		last = new list<int>[Limit + 1];
		for (int i = 0; i < NoC; ++i)
			C[i] = cost[i] = 0;
		for (int i = 0; i <= Limit; ++i) {
			worth[i] = 0;
			last[i].clear();
		}
		for (int i = 0; i < NoC; ++i) {
			getline(tfin, temp);
			stringstream ss(temp);
			ss >> n >> n >> cost[i];
		}
	} else {
		cout << "树结构数据读入错误！\n";
		destroy();
		system("pause");
		exit(1);
	}
	tfin.close();

	tfin.open(qn);
	if (tfin) {
		tfin >> NoQ;
		cout << "查询数：" << NoQ << endl;
		for (int i = 0 ; i < NoQ; ++i) {
			getline(tfin, temp);
			stringstream ss(temp);
			ss >> n;
			C[n]++;
		}
	} else {
		cout << "查询数据读入错误！\n";
		destroy();
		system("pause");
		exit(1);
	}
	tfin.close();

	// 0-1背包
	for (int i = 0; i < NoC; ++i) {
		for (int j = Limit; j >= cost[i]; --j) {
			if (worth[j - cost[i]] + C[i] >= worth[j]) {
				worth[j] = worth[j - cost[i]] + C[i];
				last[j] = last[j - cost[i]];
				last[j].push_back(i);
			}
		}
		cout << i << '\r';
	}

	// 输出结果
	tfout.open(on);
	tfout << worth[Limit] << endl;
	if (last[Limit].size()) {
		list<int>::iterator it = last[Limit].begin();
		tfout << *it;
		for (++it; it != last[Limit].end(); ++it)
			tfout << '\t' << *it;
		tfout << endl;
	}

	destroy();
	system("pause");
	return 0;
}