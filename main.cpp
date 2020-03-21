#include<bits/stdc++.h>
#include<Windows.h>
using namespace std;

struct Node
{
	unsigned short repeatCount;
	unsigned char scanCode;
	bool extendedKey, prevKeyState, transitionState;
	operator unsigned int()
	{
		return repeatCount | (scanCode << 16) | (extendedKey << 24) | (prevKeyState << 30) | (transitionState << 31);
	}
};

int Mode, Start, Continue, Space, Speed;
char ch;
short vkCode;
HWND hwnd;
int keydown, keyup;

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
				cout << "捕获成功！窗口名称：" << endl;
				cout << title << endl;
			}
			else
				cout << "捕获失败！请重试！" << endl;
			cout << endl;
			return;
		}
	}
}

void Input()
{
	cout << "请输入数字选择操作" << endl;
	cout << "1:左键连点 2:左键长按" << endl;
	cout << "3:右键连点 4:右键长按" << endl;
	cout << "5:字母键长按 6:空格长按" << endl;
	cin >> Mode;
	if (Mode == 1 || Mode == 3)
	{
		cout << "请输入连点速度（单位毫秒）：" << endl;
		cin >> Speed;
	}
	else if (Mode == 5)
	{
		cout << "请输入字母（小写）" << endl;
		cin >> ch;
	}
	else if (Mode == 6)
		ch = ' ';
	cout << "请输入多少时间后后开始操作（单位毫秒）：" << endl;
	cin >> Start;
	cout << "请输入操作持续多少时间后暂停（单位毫秒，-1为无限，设为0则表示操作一次立刻暂停）" << endl;
	cin >> Continue;
	cout << "请输入暂停后间隔多少时间继续操作（单位毫秒）：" << endl;
	cin >> Space;
	if (Mode >= 5)
	{
		Node down = {}, up = {};
		vkCode = LOBYTE(VkKeyScan(ch));
		down.scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
		up.scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
		up.repeatCount = 1;
		up.prevKeyState = true;
		up.transitionState = true;
		keydown = down;
		keyup = up;
	}
}

void Do()
{
	Sleep(Start);
	long tstart = clock();
	if (Mode == 1)
		while (true)
		{
			SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
			SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
			Sleep(Speed);
			if (Continue != -1 && clock() - tstart >= Continue)
			{
				Sleep(Space);
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
				Sleep(Space);
				SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
				tstart = clock();
			}
			if (GetAsyncKeyState(VK_RCONTROL))
			{
				SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
				return;
			}
		}
	}
	if (Mode == 3)
		while (true)
		{
			SendMessage(hwnd, WM_RBUTTONDOWN, 0, 0);
			SendMessage(hwnd, WM_RBUTTONUP, 0, 0);
			Sleep(Speed);
			if (Continue != -1 && clock() - tstart >= Continue)
			{
				Sleep(Space);
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
				Sleep(Space);
				SendMessage(hwnd, WM_RBUTTONDOWN, 0, 0);
				tstart = clock();
			}
			if (GetAsyncKeyState(VK_RCONTROL))
			{
				SendMessage(hwnd, WM_RBUTTONUP, 0, 0);
				return;
			}
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
				Sleep(Space);
				PostMessage(hwnd, WM_KEYDOWN, vkCode, keydown);
				tstart = clock();
			}
			if (GetAsyncKeyState(VK_RCONTROL))
			{
				PostMessage(hwnd, WM_KEYUP, vkCode, keyup);
				return;
			}
		}
	}
}

int main()
{
	system("title Minecraft后台挂机程序 By Cheny");
	cout << "请前往指定窗口按下左Ctrl+左Alt捕获窗口" << endl;
	while (hwnd == NULL)
		getWindow();
	Input();
	cout << "若要后台挂机，请按F3+P停用失去焦点后暂停，再按Alt+tab切出" << endl;
	cout << "请按下右Alt开始操作，按下右Ctrl结束" << endl;
	while (true)
	{
		if (GetAsyncKeyState(VK_RMENU))
		{
			cout << "开始操作" << endl;
			Do();
			cout << "操作结束！按右Alt重新开始！" << endl;
		}
	}
}