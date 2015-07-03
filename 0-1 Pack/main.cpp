#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cstring>
#include <cmath>
#include <cstdlib>

#include <Stack>

#include "../DEFINE.h"

using namespace std;

int NoC, NoQ, Limit; // 叶节点数、查询数、背包容量
int *C = NULL, *cost = NULL; // 概念引用计数数组及代价数组
int *worth = NULL; // 价值数组
bool **last; // 结果输出用辅助数组

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
	stringstream ss;

	// 读入数据
	cout << "请输入数据集编号：" << endl;
	cin >> n;
	_itoa_s(n, Num, 10);
	cout << "选定数据集：" << Num << endl;
	qn = tn = on = Num;
	qn = path + qn + "Qry.dat";
	tn = path + tn + "Tre.dat";
	on = path + on + "Pck.out";
	tfin.open(tn);
	if (tfin) {
		getline(tfin, temp);
		ss.clear();
		ss.str(temp);
		ss >> NoC;
		cout << "概念数：" << NoC << endl;
		getline(tfin, temp);
		ss.clear();
		ss.str(temp);
		ss >> n;
		Limit = min(NoC * COST, LIMIT);
		C = new int[NoC];
		cost = new int[NoC];
		worth = new int[Limit + 1];
		last = new bool*[NoC];
		for (int i = 0; i < NoC; ++i) {
			C[i] = cost[i] = 0;
			last[i] = new bool[Limit + 1];
			for (int j = 0; j <= Limit; ++j)
				last[i][j] = false;
		}
		for (int i = 0; i <= Limit; ++i)
			worth[i] = 0;
		for (int i = 0; i < NoC; ++i) {
			getline(tfin, temp);
			ss.clear();
			ss.str(temp);
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
		getline(tfin, temp);
		ss.clear();
		ss.str(temp);
		ss >> NoQ;
		cout << "查询数：" << NoQ << endl;
		for (int i = 0 ; i < NoQ; ++i) {
			getline(tfin, temp);
			ss.clear();
			ss.str(temp);
			ss >> n;
			C[n]++;
		}
	} else {
		cout << "查询数据读入错误！\n";
		destroy();
		system("pause");
		exit(1);
	}
	cout << "数据读入结束..." << endl;
	tfin.close();

	// 0-1背包
	for (int i = 0; i < NoC; ++i) {
		for (int j = Limit; j >= cost[i]; --j) {
			if (worth[j - cost[i]] + C[i] >= worth[j]) {
				worth[j] = worth[j - cost[i]] + C[i];
				last[i][j] = true;
			}
		}
		cout << i << '\r';
	}
	cout << "0-1背包求解完毕..." << endl;

	// 输出结果
	tfout.open(on);
	tfout << Limit << endl;
	tfout << worth[Limit] << '/' << NoQ << endl; // 总概念频率
	stack<int> v;
	for (int i = NoC - 1, j = Limit; i >= 0 && j >= 0; --i) {
		if (last[i][j]) {
			v.push(i);
			j -= cost[i];
		}
	}
	if (!v.empty()) {
		tfout << v.top();
		v.pop();
		while(!v.empty()) {
			tfout << '\t' << v.top();
			v.pop();
		}
	}
	tfout.close();
	cout << "0-1背包解输出完成..." << endl;

	destroy();
	system("pause");
	return 0;
}