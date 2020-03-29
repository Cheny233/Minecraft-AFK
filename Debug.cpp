#include<bits/stdc++.h>
#include<Windows.h>
using namespace std;

int Mode, Start, Continue, Space, Speed;
char ch;
short vkCode;
HWND hwnd;
int keydown, keyup;

int get_Lparam(int vk, bool flag)
{
	int scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
	return flag | (scanCode << 16) | (flag << 30) | (flag << 31);
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
				cout << "捕获成功！窗口名称：" << endl;
				cout << title << endl;
			}
			else
				cout << "捕获失败！请重试！" << endl;
			cout << endl;
			return;
		}
		Sleep(1);
	}
}

void Input()
{
	cout << "[1]左键连点 [2]左键长按" << endl;
	cout << "[3]右键连点 [4]右键长按" << endl;
	cout << "[5]字母键长按 [6]空格长按" << endl;
	cout << "请输入数字选择操作" << endl << "<< ";
	cin >> Mode;
	if (Mode == 1 || Mode == 3)
	{
		cout << "请输入连点速度（毫秒）" << endl << "<< ";
		cin >> Speed;
	}
	else if (Mode == 5)
	{
		cout << "请输入字母" << endl << "<< ";
		cin >> ch;
	}
	else if (Mode == 6)
		ch = ' ';
	cout << "多少时间后后开始操作（毫秒）" << endl << "<< ";
	cin >> Start;
	cout << "操作持续多少时间后暂停（毫秒，-1为无限）" << endl << "<< ";
	cin >> Continue;
	cout << "暂停后间隔多少时间继续操作（毫秒）" << endl << "<< ";
	cin >> Space;
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

int main()
{
	system("title Minecraft-AFK By Cheny");
	system("mode con cols=45 lines=25");
	cout << "请前往指定窗口按下左Ctrl+左Alt捕获窗口" << endl;
	while (hwnd == NULL)
		getWindow();
	Input();
	cout << endl << ">>>请在任意位置按下右Alt开始操作<<<" << endl;
	cout << ">>>任意位置按下右Ctrl结束<<<" << endl << endl;
	while (true)
	{
		if (GetAsyncKeyState(VK_RMENU))
		{
			cout << "\r开始操作！按右Ctrl可结束！";
			system("color 03");
			Do();
			cout << "\r操作结束！按右Alt重新开始！";
			system("color 07");
		}
		Sleep(1);
	}
} 