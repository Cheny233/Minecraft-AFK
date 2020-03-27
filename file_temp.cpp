#include <bits/stdc++.h>
#include<Windows.h>
#include <io.h>
using namespace std;

bool find_File()
{
	vector<_finddata_t> vec;
	string path = "*.txt";
	struct _finddata_t file;
	long handle = _findfirst(path.c_str(), &file);
	if (handle == -1)
		return false;
	cout << "[0]不使用配置文件" << endl;
	vec.push_back(file);
	cout << "[1]" << file.name << endl;
	int num = 2;
	while (!_findnext(handle, &file))
	{
		vec.push_back(file);
		cout << '[' << num++ << ']' << file.name << endl;
	}
	cout << "请选择配置文件" << endl;
	int k;
	cin >> k;
	if (k == 0)
		return false;
	cout << vec[k - 1].name << endl;
	FILE* stream;
	freopen_s(&stream, vec[k - 1].name, "r", stdin);
	return true;
}

int main()
{
	if (!find_File())
		cout << "NULL" << endl;
	else
	{
		int a;
		cin >> a;
		cout << a << endl;
	}
}