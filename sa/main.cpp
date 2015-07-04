#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "../DEFINE.h"

using namespace std;

string test_num;
int leaf_num;
int node_num;
int ent_num;
int qry_num;
int Limit;
double u[2000];
int d[2000] = {};
int w[2000] = {};
int f[2000] = {};
int chnum[2000] = {};
int c[2000][2000] = {};
bool tag[2000], best_tag[2000];
int countnum[2000];
double Tmin = 0.0001;
double Tstart = 200;
double alpha = 0.99;

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
			d[x]++;
	}
	fin.close();
	Limit = min(leaf_num * COST, LIMIT);
}

void input_qry()
{
	ifstream fin;
	string filename;
	filename = string(path) + test_num + "Qry.dat";
	fin.open(filename.c_str());
	fin >> qry_num;
	for (int i = 0; i < leaf_num; i++)
		u[i] = 0.0;
	for (int i = 0; i < qry_num; i++)
	{
		int x, y;
		fin >> x >> y;
		u[x]++;
	}
	for (int i = 0; i < leaf_num; i++)
		u[i] /= qry_num;
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
		w[x] = z;
	}
	fin.close();
}

void input_pck()
{
	ifstream fin;
	string filename;
	filename = string(path) + test_num + "Pck.dat";
	fin.open(filename.c_str());
	string s;
	fin >> s;
	fin >> s;
	int x;
	while (fin >> x)
		tag[x] = true;
	fin.close();
}

int count(int cur)
{
	if (cur < leaf_num)
		return d[cur];
	if (countnum[cur] > 0)
		return countnum[cur];
	for (int i = 0; i < chnum[cur]; i++)
	{
		if (tag[c[cur][i]])
			continue;
		countnum[cur] += count(c[cur][i]);
	}
	//cout << cur<< " " << countnum[cur]<< endl;
	return countnum[cur];
}

double val()
{
	double ret = 0.0;
	for (int i = 0; i < node_num; i++)
		countnum[i] = 0;
	tag[node_num - 1] = true;
	for (int i = 0; i < leaf_num; i++)
	{
		if (tag[i])
		{
			ret += u[i];
			continue;
		}
		int cur = i;
		while (!tag[cur])
			cur = f[cur];
		ret += u[i] * double(d[i])/count(cur);
	}
	return ret;
}

int main()
{
	getline(cin, test_num);
	input_doc();
	input_qry();
	input_tre();
	input_pck();

	int val_num = leaf_num;

	double best_val = 0;
	double cur_val = val();
	int cur_wei = 0;
	
	for (int i = 0; i < val_num; i++)
		if (tag[i])
			cur_wei += w[i];

	srand((unsigned)time(0));

	double T = Tstart;
	
	while (T > Tmin)
	{
		int x;
		while (tag[x = rand() % val_num]);
		if (cur_wei + w[x] <= Limit)
		{
			tag[x] = true;
			cur_wei += w[x];
			cur_val = val();
		}
		else
		{
			bool pre_tag[2000];
			for (int i = 0;i < val_num; i++)
				pre_tag[i] = tag[i];
			int wei = cur_wei;
			tag[x] = true;
			wei += w[x];
			while (wei > Limit)
			{
				int y;
				while (!tag[y = rand() % val_num]);
				tag[y] = false;
				wei -= w[y];
			}
			double new_val = val();
			if (new_val >= cur_val || exp((new_val - cur_val)/T) > (rand() % 100) / 100.0)
			{
				cur_val = new_val;
				cur_wei = wei;
			}
			else
			{
				for (int i = 0; i < val_num; i++)
					tag[i] = pre_tag[i];
			}
		}
		if (cur_val > best_val)
		{
			best_val = cur_val;
			for (int i = 0; i < val_num; i++)
				best_tag[i] = tag[i];
		}
		cout << "T: " << T << " cur_val: " << cur_val << " cur_wei: " << cur_wei << " best_val: " << best_val << endl;
		T = T * alpha;
	}

	ofstream fout;
	string filename = string(path) + test_num + "Sa0.dat";
	fout.open(filename);
	fout << 0 << endl;
	fout << 0 << endl;
	for (int i = 0; i < node_num; i++)
		if (best_tag[i])
			fout << i << " ";
	fout << endl;
	fout.close();

	val_num = node_num;

	T = Tstart;
	
	while (T > Tmin)
	{
		int x;
		while (tag[x = rand() % val_num]);
		if (cur_wei + w[x] <= Limit)
		{
			tag[x] = true;
			cur_wei += w[x];
			cur_val = val();
		}
		else
		{
			bool pre_tag[2000];
			for (int i = 0;i < val_num; i++)
				pre_tag[i] = tag[i];
			int wei = cur_wei;
			tag[x] = true;
			wei += w[x];
			while (wei > Limit)
			{
				int y;
				while (!tag[y = rand() % val_num]);
				tag[y] = false;
				wei -= w[y];
			}
			double new_val = val();
			if (new_val >= cur_val || exp((new_val - cur_val)/T) > (rand() % 100) / 100.0)
			{
				cur_val = new_val;
				cur_wei = wei;
			}
			else
			{
				for (int i = 0; i < val_num; i++)
					tag[i] = pre_tag[i];
			}
		}
		if (cur_val > best_val)
		{
			best_val = cur_val;
			for (int i = 0; i < val_num; i++)
				best_tag[i] = tag[i];
		}
		cout << "T: " << T << " cur_val: " << cur_val << " cur_wei: " << cur_wei << " best_val: " << best_val << endl;
		T = T * alpha;
	}

	filename = string(path) + test_num + "Sa1.dat";
	fout.open(filename);
	fout << 0 << endl;
	fout << 0 << endl;
	for (int i = 0; i < node_num; i++)
		if (best_tag[i])
			fout << i << " ";
	fout << endl;
	fout.close();

	system("pause");
}