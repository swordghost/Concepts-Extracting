#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

#define UPBOUND 50

int NoC, NoE, NoQ; // 总概念数、总实体数、总查询数

class Concept{
public:
	unsigned id;
	int E[2 * UPBOUND];
	int length, i;
	Concept():i(0) {
	}
	~Concept() {
	}
	unsigned init(unsigned _id) {
		id = _id;
		length = id % min(NoE, UPBOUND);
		if (length <= 0)
			length = 1;
		return id;
	}
	bool add(int e) {
		if (i < 2 * UPBOUND) {
			E[i++] = e;
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
	Entity *E; // 实体数组
	int ECount = 0; // 总实体数量（包括同名不同概）
	char Num[11];
	unsigned RandNum;
	ofstream tfout;

	// 输入
	printf("请依次输入总概念数、总实体数和总查询数：");
	cin >> NoC;
	while (NoC <= 0) {
		printf("总概念数必须大于0！重新输入：");
		cin >> NoC;
	}
	printf("随机生成总概念数：%d\n", NoC);
	cin >> NoE;
	while (NoE <= 0) {
		printf("总实体数必须大于0！重新输入：");
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
	RandNum = (unsigned)rand();
	printf("生成概念：\n");
	for (int i = 0; i < NoC; ++i)
		printf("C%u\n", C[i].init(RandNum += (unsigned)rand() % NoC + 1));
	printf("\n");

	// 实体生成
	E = new Entity[NoE];
	RandNum = (unsigned)rand();
	printf("生成实体：\n");
	for (int i = 0; i < NoE; ++i)
		printf("E%u\n", E[i].init(RandNum += (unsigned)rand() % NoC + 1));
	printf("\n");

	// 生成文档编号
	unsigned temp = (unsigned)rand();
	_itoa_s((int)temp, Num, 10);
	string dn(Num);
	dn = "D:\\CEData\\" + dn + "Doc.dat";
	while (1) {
		ifstream tfin(dn);
		if (!tfin) {
			break;
			tfin.close();
		}
		temp = (unsigned)rand();
		_itoa_s((int)temp, Num, 10);
		dn = Num;
		dn = "D:\\CEData\\" + dn + "Doc.dat";
		tfin.close();
	}

	// 文件名定义
	string qn = Num;
	qn = "D:\\CEData\\" + qn + "Qry.dat";
	string tn = Num;
	tn = "D:\\CEData\\" + tn + "Tre.dat";

	// 文档生成
	for (int i = 0; i < NoC; ++i) {
		int ReferCount = 0;
		RandNum = (unsigned)(rand() % NoE);
		cout << i << ':' << C[i].length << '\r';
		while (ReferCount < C[i].length) {
			RandNum = (RandNum + (unsigned)(1 + rand() % (NoE - 2))) % NoE;
			if (E[RandNum].check(i)) {
				C[i].add(RandNum);
				ReferCount++;
			}
		}
	}
	for (int i = 0; i < NoE; ++i) {
		int cnt = 0;
		if (E[i].check(0)) {
			RandNum = (unsigned)(rand() % NoC);
			while (cnt < 2) {
				cnt += C[RandNum].add(i);
				RandNum = (RandNum + (unsigned)(1 + rand() % (NoC - 2))) % NoC;
			}
		}
	}
	cout << "文档生成完毕" << endl;

	// 文档输出
	tfout.open(dn);
	for (int i = 0; i < NoC; ++i) {
		tfout << 'C' << C[i].id;
		ECount += C[i].i;
		for (int j = 0; j < C[i].i; ++j)
			tfout << "\tE" << E[C[i].E[j]].id;
		tfout << endl;
	}
	tfout.close();
	cout << "文档输出完毕" << endl;

	// 查询生成
	//tfout.open(qn);
	//for (int i = 0; i < NoE; ++i)
	//	E[i].LastRefer = -1;
	//for (int i = 0; i < NoC; ++i) {
	//	int q = 
	//}

	// 清理内存
	//delete C;
	//delete E;

	system("pause");
	return 0;
}