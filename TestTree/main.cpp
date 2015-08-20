#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include "../DEFINE.h"

using namespace std;

string TestName;
string test_num;
int leaf_num,ent_num,node_num,qry_num;
int ent[2000][500];
int d[2000] = {};
int f[2000] = {};
int chnum[2000] = {};
int c[2000][2000] = {};
bool tag[2000] = {};
vector<int> countit[2000];
int ans = 0;

void input_doc()
{
	ifstream fin;
	string filename;
	filename = string(path) + test_num + "Doc.dat";
	fin.open(filename.c_str());
	fin >> leaf_num;
	fin >> ent_num;
	for (int i = 0; i < leaf_num; i++)
	{
		string tmp;
		getline(fin, tmp);
		stringstream ss(tmp);
		int x,y;
		ss >> x;
		while (ss >> y)
			ent[x][d[x]++] = y;
	}
	fin.close();
}

void input_tre()
{
	ifstream fin;
	string filename;
	filename = string(path) + test_num + "Tre.dat";
	fin.open(filename.c_str());
	fin >> leaf_num;
	fin >> node_num;
	for (int i = 0; i < node_num; i++)
	{
		int x, y, z;
		fin >> x >> y >> z;
		f[x] = y;
		c[y][chnum[y]++] = x;
	}
	fin.close();
}

void input_rst()
{
	ifstream fin;
	string filename;
	filename = string(path) + test_num + TestName + ".out";
	fin.open(filename.c_str());
	string s;
	fin >> s;
	fin >> s;
	int x;
	while (fin >> x)
		tag[x] = true;
	fin.close();
}

void input_qry()
{
	ifstream fin;
	string filename;
	filename = string(path) + test_num + "Qry.dat";
	fin.open(filename.c_str());
	fin >> qry_num;
	for (int i = 0; i < qry_num; i++)
	{
		int x, y;
		fin >> x >> y;
		int cur = x;
		while (!tag[cur])
			cur = f[cur];
		vector<int> tmp;
		for (vector<int>::iterator j = countit[cur].begin(); j != countit[cur].end(); j++)
		{
			//cout << *j << endl;
			int con = *j;
			for (int k = 0; k < d[con]; k++)
				if (ent[con][k] == y)
					tmp.push_back(con);
		}
		int times = min((int)tmp.size(),2);
		bool flag = false;
		for (int j = 0; j < times; j++)
		{
			//cout << rand() << " " << tmp.size() << endl;
			int choose = rand() % tmp.size();
			if (tmp[choose] == x)
				flag = true;
			tmp.erase(tmp.begin() + choose);
		}
		if (flag) ans++;
	}
	cout << (double)ans / qry_num << endl;
	fin.close();
}

void get_list(int fa,int cur)
{
	if (cur < leaf_num)
	{
		countit[fa].push_back(cur);
		return;
	}
	for (int i = 0; i < chnum[cur]; i++)
	{
		if (tag[c[cur][i]])
			continue;
		get_list(fa, c[cur][i]);
	}
}

int main()
{
	cout << "输入Pck或Sa0或Sa1" << endl;
	getline(cin, TestName);
	if (TestName != "Pck" && TestName != "Sa0" && TestName != "Sa1") {
		cout << "结果指定错误！\n";
		exit(1);
	}
	srand((unsigned)time(0));
	getline(cin, test_num);
	cin >> TestName;
	input_doc();
	input_tre();
	input_rst();
	tag[node_num - 1] = true;
	for (int i = 0; i < node_num; i++)
		if (tag[i])
		{
			get_list(i,i);
		}
	input_qry();
	system("pause");
	return 0;
}