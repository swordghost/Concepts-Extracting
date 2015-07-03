#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <ctime>

#include "../DEFINE.h"

using namespace std;

struct INDEX {
	int *i;
	int pt;
	INDEX() : pt(-1), i(NULL) {
	}
	void set(int len) {
		i = new int[len];
		pt = 0;
	}
	void add(int c) {
		i[pt++] = c;
	}
}*idx = NULL;

int NoC, NoE, NoQ, Res;
bool *mark;

bool check(int Con, int Ent) {
	int RandNum[3];
	if (idx[Ent].pt <= 3) {
		for (int i = 0; i < idx[Ent].pt; ++i)
			if (idx[Ent].i[i] == Con)
				return true;
	} else {
		RandNum[0] = rand() % idx[Ent].pt;
		RandNum[1] = (RandNum[0] + rand() % (idx[Ent].pt - 2) + 1) % idx[Ent].pt;
		do {
			RandNum[2] = (RandNum[1] + rand() % (idx[Ent].pt - 2) + 1) % idx[Ent].pt;
		} while (RandNum[2] == RandNum[0] || RandNum[2] == RandNum[1]);
		for (int i = 0; i < 3; ++i)
			if (idx[Ent].i[RandNum[i]] == Con)
				return true;
	}
	return false;
}

int main() {
	int n;
	char Num[11];
	ifstream tfin;
	string temp;
	string qn, on, dn; // ��ѯ��������ĵ������ļ���
	stringstream ss;
	
	srand((unsigned)time(0));

	// ��������
	cout << "���������ݼ���ţ�" << endl;
	cin >> n;
	_itoa_s(n, Num, 10);
	cout << "ѡ�����ݼ���" << Num << endl;
	qn = on = dn = Num;
	qn = path + qn + "Qry.dat";
	dn = path + dn + "Doc.dat";
	on = path + on + "Pck.out";

	// �����ĵ����ݣ�������������
	tfin.open(dn);
	if (tfin) {
		int c, e;
		getline(tfin, temp);
		ss.clear();
		ss.str(temp);
		ss >> NoC;
		getline(tfin, temp);
		ss.clear();
		ss.str(temp);
		ss >> NoE;
		cout << "��������" << NoC << endl;
		cout << "ʵ��������" << NoE << endl;
		idx = new INDEX[NoE];
		mark = new bool[NoC];
		for (int i = 0; i < NoE; ++i)
			idx[i].set(NoC);
		for (int i = 0; i < NoC; ++i)
			mark[i] = false;
		while(getline(tfin, temp)) {
			ss.clear();
			ss.str(temp);
			ss >> c;
			while (ss >> e)
				idx[e].add(c);
			cout << c << '\r';
		}
	} else {
		cout << "�ĵ����ݶ������\n";
		system("pause");
		exit(1);
	}
	tfin.close();
	cout << "�ĵ����ݶ������..." << endl;

	// ����������
	tfin.open(on);
	if (tfin) {
		getline(tfin, temp);
		getline(tfin, temp);
		ss.clear();
		ss.str(temp);
		ss >> Res;
		getline(tfin, temp);
		ss.clear();
		ss.str(temp);
		while(ss >> n)
			mark[n] = true;
	} else {
		cout << "������ݶ������\n";
		system("pause");
		exit(1);
	}
	tfin.close();
	cout << "������ݶ������..." << endl;

	// �����ѯ���ݲ����
	tfin.open(qn);
	if (tfin) {
		int c, e;
		getline(tfin, temp);
		ss.clear();
		ss.str(temp);
		ss >> NoQ;
		cout << "��ѯ����" << NoQ << endl;
		while(getline(tfin, temp)) {
			ss.clear();
			ss.str(temp);
			ss >> c >> e;
			cout << c << ':' << e << '\r';
			if (!mark[c] && check(c, e))
				Res++;
		}
	} else {
		cout << "��ѯ���ݶ������\n";
		system("pause");
		exit(1);
	}
	tfin.close();
	cout << "��ѯ���ݶ������..." << endl;
	cout << "���������..." << endl;
	cout << "׼ȷ�ʣ�" << Res << '/' << NoQ << '=' << (double)Res/(double)NoQ << endl;

	system("pause");
	return 0;
}