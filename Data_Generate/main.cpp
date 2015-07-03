#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <Vector>

#include "../DEFINE.h"

using namespace std;

int NoC, NoE, NoQ; // 总概念数、总实体数、总查询数

class Concept{
public:
	unsigned id;
	int E[2 * EPCUPBOUND];
	int i, LastRefer;
	Concept():i(0), LastRefer(-1) {
	}
	~Concept() {
	}
	unsigned init(unsigned _id) {
		id = _id;
		return id;
	}
	bool add(int e) {
		if (LastRefer < e && i < 2 * EPCUPBOUND) {
			E[i++] = e;
			LastRefer = e;
			return true;
		}
		return false;
	}
};

class Entity{
public:
	unsigned id;
	int LastRefer;
	Entity() : LastRefer(-1) {}
	unsigned init(unsigned _id) {
		id = _id;
		return id;
	}
	bool check(int c) {
		if (LastRefer < c) {
			LastRefer = c;
			return true;
		}
		return false;
	}
};

int main() {
	srand((unsigned)time(0));
	Concept *C; // 概念数组
	int *Tree; // 树结构数组，只给出了每个节点的父节点
	Entity *E; // 实体数组
	int ECount = 0; // 总实体数量（包括同名不同概）
	char Num[11];
	unsigned RandNum;
	ofstream tfout;

	// 输入
	printf("请依次输入总概念数、总实体数和总查询数：");
	cin >> NoC;
	while (NoC <= 2) {
		printf("总概念数必须大于2！重新输入：");
		cin >> NoC;
	}
	printf("随机生成总概念数：%d\n", NoC);
	cin >> NoE;
	while (NoE <= 2) {
		printf("总实体数必须大于2！重新输入：");
		cin >> NoE;
	}
	printf("随机生成总实体数：%d\n", NoE);
	cin >> NoQ;
	while (NoQ <= 0) {
		printf("总查询数必须大于0！重新输入：");
		cin >> NoQ;
	}
	printf("随机生成总查询数：%d\n", NoQ);

	// 概念生成
	C = new Concept[NoC];
	RandNum = 0;
	printf("生成概念...\n");
	for (int i = 0; i < NoC; ++i)
		C[i].init(RandNum += (unsigned)rand() % NoC + 1);

	// 实体生成
	E = new Entity[NoE];
	RandNum = 0;
	printf("生成实体...\n");
	for (int i = 0; i < NoE; ++i)
		E[i].init(RandNum += (unsigned)rand() % NoC + 1);

	// 生成文档编号
	unsigned temp = (unsigned)rand();
	_itoa_s((int)temp, Num, 10);
	string dn(Num);
	dn = path + dn + "Doc.dat";
	while (1) {
		ifstream tfin(dn);
		if (!tfin) {
			break;
			tfin.close();
		}
		temp = (unsigned)rand();
		_itoa_s((int)temp, Num, 10);
		dn = Num;
		dn = path + dn + "Doc.dat";
		tfin.close();
	}

	// 文件名定义
	string qn = Num;
	qn = path + qn + "Qry.dat";
	string tn = Num;
	tn = path + tn + "Tre.dat";
	string cn = Num;
	cn = path + cn + "Con.dat";
	string en = Num;
	en = path + en + "Ent.dat";

	// 概念输出
	tfout.open(cn);
	tfout << NoC << endl;
	for (int i = 0; i < NoC; ++i)
		tfout << i << "\tC" << C[i].id << endl;
	tfout.close();

	// 实体输出
	tfout.open(en);
	tfout << NoE << endl;
	for (int i = 0; i < NoE; ++i)
		tfout << i << "\tE" << E[i].id << endl;
	tfout.close();

	// 文档生成
	for (int i = 0; i < NoE; ++i) {
		int ReferCount = 0, len;
		if ((unsigned)rand() % MULTIPULPRO < MULTIPULPRO - 1)
			len = (unsigned)rand() % (MULTIPULUPB1 - MULTIPULUPB2) + MULTIPULUPB2 + 1;
		else
			len = (unsigned)rand() % MULTIPULUPB2 + 1;
		cout << i << '\t' << len << '\r';
		while (ReferCount < len) {
			RandNum = (RandNum + (unsigned)rand() % (NoC - 1) + 1) % NoC;
			if (C[RandNum].add(i))
				ReferCount++;
		}
	}
	for (int i = 0; i < NoC; ++i) {
		int ReferCount = 0;
		if (C[i].i == 0) {
			int len = rand() % MULTIPULUPB2 + 1;
			while (ReferCount < len) {
				RandNum = (RandNum + (unsigned)rand() % (NoE - 1) + 1) % NoE;
				if (C[i].add(RandNum))
					ReferCount++;
			}
		}
	}
	cout << "文档生成完毕..." << endl;

	// 文档输出
	tfout.open(dn);
	tfout << NoC << endl;
	tfout << NoE << endl;
	for (int i = 0; i < NoC; ++i) {
		tfout << i;
		ECount += C[i].i;
		for (int j = 0; j < C[i].i; ++j)
			tfout << '\t' << C[i].E[j];
		tfout << endl;
	}
	tfout.close();
	cout << "文档输出完毕..." << endl;

	// 查询生成
	tfout.open(qn);
	tfout << NoQ << endl;
	for (int i = 0; i < NoE; ++i)
		E[i].LastRefer = -1;
	for (int i = 0; i < NoC; ++i) {
		int q = C[i].i * NoQ / ECount;
		NoQ -= q;
		ECount -= C[i].i;
		int ReferCount = 0;
		cout << i << ':' << q << '\r';
		while (ReferCount < q) {
			RandNum = (RandNum + (unsigned)(1 + (C[i].i > 1 ? rand() % (C[i].i - 1) : 0))) % C[i].i;
			if (E[C[i].E[RandNum]].check(i)) {
				tfout << i << '\t' << C[i].E[RandNum] << endl;
				ReferCount++;
			}
		}
	}
	tfout.close();
	cout << "查询输出完毕..." << endl;

	// 树结构生成
	Tree = new int[2 * NoC];
	int pt = NoC;
	vector<int> _v;
	for (int i = 0; i < NoC; ++i) {
		Tree[i] = i;
		_v.push_back(i);
	}
	int r;
	while(_v.size() != 1) {
		cout << _v.size() << '\r';
		if ((unsigned)rand() % FEWNODESPRO < FEWNODESPRO - 1)
			r = min((unsigned)rand() % (TREEUPBOUND1 - 1) + 2, _v.size()); // 合并TREEUPBOUND1个及以下节点
		else
			r = min((unsigned)rand() % (TREEUPBOUND2 - 1) + 2, _v.size()); // 合并TREEUPBOUND2个及以下节点
		for (int j = 0; j < r; ++j) {
			RandNum = (unsigned)rand() % _v.size();
			Tree[_v[RandNum]] = pt;
			_v.erase(_v.begin() + RandNum);
		}
		_v.push_back(pt);
		Tree[pt] = pt;
		pt++;
	}
	_v.clear();

	// 树结构输出
	tfout.open(tn);
	tfout << NoC << endl << pt << endl;
	for (int i = 0; i < pt; ++i) {
		if (i == pt - 1)
			RandNum = 0;
		else
			RandNum = (unsigned)rand() % COST + 1; // 随机生成代价
		tfout << i << '\t' << Tree[i] << '\t' << RandNum << endl;
	}
	tfout.close();
	cout << "树结构输出完毕..." << endl;

	// 清理内存
	delete Tree;
	//delete C;
	//delete E;

	system("pause");
	return 0;
}