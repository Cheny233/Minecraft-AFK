#include<bits/stdc++.h>
#include<Windows.h>
#include<io.h>
using namespace std;

int Mode, Start, Continue, Space, Speed;

char ch;
short vkCode;
int keydown, keyup;

string file_data;

HWND hwnd;

int get_Lparam(int vk, bool flag)
{
	int scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
	return flag | (scanCode << 16) | (flag << 30) | (flag << 31);
}

bool getFile()
{
	vector<_finddata_t> vec;
	string path = "config\\*.afk";
	struct _finddata_t file;
	long handle = _findfirst(path.c_str(), &file);
	if (handle == -1)
		return false;
	cout << "A configuration file is detected under the directory:" << endl << endl;
	cout << "[0]Do not use configuration files" << endl;
	int num = 1;
	do
	{
		vec.push_back(file);
		cout << '[' << num++ << ']' << file.name << endl;
	} while (!_findnext(handle, &file));
	cout << endl;
	int count;
	cout << "Please select a configuration file:" << endl << "<< ";
	cin >> count;
	system("cls");
	if (count == 0)
		return false;
	char filename[50] = "config\\";
	strcat_s(filename, vec[count - 1].name);
	FILE* stream;
	freopen_s(&stream, filename, "r", stdin);
	cout << "Successful choice! file name:" << endl;
	cout << filename << endl << endl;
	return true;
}

void getWindow()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_LCONTROL) && GetAsyncKeyState(VK_LMENU))
		{
			hwnd = GetForegroundWindow();
			if (hwnd != NULL)
			{
				char title[50];
				::GetWindowTextA(hwnd, title, sizeof(title));
				system("cls");
				cout << "Successfully captured! Window name:" << endl;
				cout << title << endl;
			}
			else
				cout << "Capture failed! Please try again!" << endl;
			cout << endl;
			return;
		}
		Sleep(1);
	}
}

void Get(int& a, bool flag)
{
	cin >> a;
	if (flag)
		cout << a << endl;
	else file_data += to_string(a) + '\n';
}

void Get(char& a, bool flag)
{
	cin >> a;
	if (flag)
		cout << a << endl;
	else file_data.push_back(a), file_data += '\n';
}

void Input(bool flag)
{
	cout << "[1]Left click coninuously [2]Left click and hold" << endl;
	cout << "[3]Right click coninuously [4]Right click and hold" << endl;
	cout << "[5] Press and hold the letter key [6] Press and hold the space key" << endl;
	cout << "Please enter a number to select the operation" << endl << "<< ";
	Get(Mode, flag);
	if (Mode == 1 || Mode == 3)
	{
		cout << "Please enter the click speed (ms)" << endl << "<< ";
		Get(Speed, flag);
	}
	else if (Mode == 5)
	{
		cout << "Please enter letters (can also be numbers, punctuation)" << endl << "<< ";
		Get(ch, flag);
	}
	else if (Mode == 6)
		ch = ' ';
	cout << "After how much time to start operation (ms)" << endl << "<< ";
	Get(Start, flag);
	cout << "How much time the operation lasts (milliseconds, -1 is unlimited)" << endl << "<< ";
	Get(Continue, flag);
	cout << "How much time to continue operation after pause (ms)" << endl << "<< ";
	Get(Space, flag);
	if (Mode >= 5)
	{
		vkCode = LOBYTE(VkKeyScan(ch));
		keydown = get_Lparam(vkCode, false);
		keyup = get_Lparam(vkCode, true);
	}
}

bool sleep(int t)
{
	long now = clock();
	while (clock() - now < t)
	{
		Sleep(1);
		if (GetAsyncKeyState(VK_RCONTROL))
			return true;
	}
	return false;
}

void Do()
{
	if (sleep(Start))
		return;
	long tstart = clock();
	if (Mode == 1)
		while (true)
		{
			SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
			SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
			if (sleep(Speed))
				return;
			if (Continue != -1 && clock() - tstart >= Continue)
			{
				if (sleep(Space))
					return;
				tstart = clock();
			}
			if (GetAsyncKeyState(VK_RCONTROL))
				return;
		}
	if (Mode == 2)
	{
		SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
		while (true)
		{
			if (Continue != -1 && clock() - tstart >= Continue)
			{
				SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
				if (sleep(Space))
					return;
				SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
				tstart = clock();
			}
			if (GetAsyncKeyState(VK_RCONTROL))
			{
				SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
				return;
			}
			Sleep(1);
		}
	}
	if (Mode == 3)
		while (true)
		{
			SendMessage(hwnd, WM_RBUTTONDOWN, 0, 0);
			SendMessage(hwnd, WM_RBUTTONUP, 0, 0);
			if (sleep(Speed))
				return;
			if (Continue != -1 && clock() - tstart >= Continue)
			{
				if (sleep(Space))
					return;
				tstart = clock();
			}
			if (GetAsyncKeyState(VK_RCONTROL))
				return;
		}
	if (Mode == 4)
	{
		SendMessage(hwnd, WM_RBUTTONDOWN, 0, 0);
		while (true)
		{
			if (Continue != -1 && clock() - tstart >= Continue)
			{
				SendMessage(hwnd, WM_RBUTTONUP, 0, 0);
				if (sleep(Space))
					return;
				SendMessage(hwnd, WM_RBUTTONDOWN, 0, 0);
				tstart = clock();
			}
			if (GetAsyncKeyState(VK_RCONTROL))
			{
				SendMessage(hwnd, WM_RBUTTONUP, 0, 0);
				return;
			}
			Sleep(1);
		}
	}
	if (Mode >= 5)
	{
		PostMessage(hwnd, WM_KEYDOWN, vkCode, keydown);
		while (true)
		{
			if (Continue != -1 && clock() - tstart >= Continue)
			{
				PostMessage(hwnd, WM_KEYUP, vkCode, keyup);
				if (sleep(Space))
					return;
				PostMessage(hwnd, WM_KEYDOWN, vkCode, keydown);
				tstart = clock();
			}
			if (GetAsyncKeyState(VK_RCONTROL))
			{
				PostMessage(hwnd, WM_KEYUP, vkCode, keyup);
				return;
			}
			Sleep(1);
		}
	}
}

void createFile()
{
	string filename;
	cout << "\rPlease set the configuration file name (within 30 characters)" << endl << "<< ";
	getchar();
	getline(cin, filename);
	filename += ".afk";
	char file[50] = "config\\";
	strcat_s(file, filename.c_str());
	system("md config");
	ofstream create(file);
	create << file_data;
	create.close();
}

int main()
{
	system("mode con cols=45 lines=25");
	system("title Minecraft-AFK By Cheny");
	bool flag = getFile();
	cout << "Please go to the specified window and press left Ctrl+left Alt to capture the window" << endl;
	while (hwnd == NULL)
		getWindow();
	Input(flag);
	cout << endl << ">>>Press the right Alt at any position to start the operation <<<" << endl;
	cout << ">>>Press Ctrl anywhere to end <<<" << endl;
	if (!flag)
		cout << endl << ">>>After finishing, press F12 to save the configuration<<<" << endl;
	cout << endl;
	while (true)
	{
		if (GetAsyncKeyState(VK_RMENU))
		{
			cout << "\rThe operation begins! Press the right Ctrl to end!";
			system("color 03");
			Do();
			cout << "\rThe operation is over! Press right Alt to start over!";
			system("color 07");
		}
		if (!flag && GetAsyncKeyState(VK_F12))
		{
			createFile();
			return 0;
		}
		Sleep(1);
	}
}
