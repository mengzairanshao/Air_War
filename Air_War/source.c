#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include<windows.h>
#include<string.h>
#include<stdbool.h>
#define N 35
#define M 35
#define length  22
#define width  22
#define MY_BUFSIZE 1024
#define FALL_SPEED 1000000
void plane(int x, int y, bool vis);
void print();//输出函数
void movebul();//子弹移动函数
void movepla();//敌机移动函数
void setting();//设置函数
void menu();//菜单函数
void writeAndDelete(int wi, int wj, int di, int dj, char* wstr, char* dstr);
void createProject(int x, int y, char* wstr, char* dstr, bool vis);
void write(int wi, int wj, char* wstr);
void fixed_write();
HANDLE consoleHandle;
bool flag, flag1;
int plane_pos[width], bul_pos[width], my_plane_pos, pl = 9, speed = 10, density = 50, score = 0, death = 0;//全局变量：界面、我机初始位、界面宽度、敌机速度、敌机密度、得分、死亡
int main()
{
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfo;
	curInfo.dwSize = 1;
	curInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &curInfo);
	SetConsoleOutputCP(437);
	menu();
	int i = 0, j = 0;
	system("cls");
	my_plane_pos = pl;
	fixed_write();
	while (true)
	{
		if (_kbhit())/*_kbhit()函数为检查是否有键盘输入,若有返回非0值,否则返回0*/
			switch (_getch())//
			{
			case 'j':
			case 'J':
				if (my_plane_pos > 2)
				{
					my_plane_pos--;
					flag = true;
					flag1 = true;
				}
				break;
			case 'l':
			case 'L':
				if (my_plane_pos < width - 4)
				{
					my_plane_pos++;
					flag = false;
					flag1 = true;
				}
				break;
			case 'w':
			case 'W':
				if (bul_pos[my_plane_pos] == 0)bul_pos[my_plane_pos] = length - 4;
				break;
			case 27:
				setting();
				break;
			default: break;
			}
		if (++j % density == 0)//控制生产敌机的速度
		{
			j = 0;
			srand(time(NULL));
			int randn = rand() % (width - 5) + 2;
			if (plane_pos[randn] == 0)plane_pos[randn] = 1;
		}
		if (++i % speed == 0)//控制敌机移动速度，相对于子弹移动速度
		{
			i = 0;
			movepla();
		}
		movebul();
		print();
	}
}

void writeAndDelete(int wi, int wj, int di, int dj, char* wstr, char* dstr)
{
	write(di, dj, dstr);
	write(wi, wj, wstr);
}

void write(int wi, int wj, char* wstr)
{
	short wx = wi, wy = wj;
	COORD position = { wy, wx };
	WriteConsoleOutputCharacter(consoleHandle, wstr, strlen(wstr), position, NULL);
}

void fixed_write()
{
	for (int i = 0; i <= length; i++)
		write(i, width - 1, "|");
	write(2, width, "SCORE:");
	write(4, width, "DEATH:");
	write(6, width, "SETTING:Esc");
	write(8, width, "Copyright:HZY");
}

void print()
{
	char buf[100];

	for (int i = 0; i < width; i++)
	{
		for (int j = FALL_SPEED; j > 0; j--);
		if (plane_pos[i] > 0)
			writeAndDelete(plane_pos[i], i, plane_pos[i] - 1, i, "\x02", "\x20");
		if (plane_pos[i] <= 0)
		{
			write(abs(plane_pos[i]), i, "\x20");
			plane_pos[i] = 0;
		}
		if (bul_pos[i] > 0)
			writeAndDelete(bul_pos[i], i, bul_pos[i] + 1, i, "\x17", "\x20");
		if (bul_pos[i] <= 0)
		{
			write(abs(bul_pos[i]), i, " ");
			bul_pos[i] = 0;
		}
	}
	if (flag && flag1)
	{
		plane(my_plane_pos + 1, length - 1, false);
		flag1 = false;
	}
	else if (!flag && flag1)
	{
		plane(my_plane_pos - 1, length - 1, false);
		flag1 = false;
	}
	if (my_plane_pos)plane(my_plane_pos, length - 1, true);
	_itoa_s(score, buf, 100,10);
	write(2, width + strlen("SCORE:"), buf);
	_itoa_s(death, buf, 100,10);
	write(4, width + strlen("DEATH:"), buf);
}

void movebul()
{
	for (int i = 0; i < length; i++)
	{
		if (bul_pos[i] > 0 && plane_pos[i] == bul_pos[i])
		{
			score += 10;
			bul_pos[i] = -bul_pos[i];
			plane_pos[i] = -plane_pos[i];
		}
		else if (bul_pos[i] >= 0)bul_pos[i]--;
	}
}

void movepla()
{
	for (int i = width - 1; i >= 0; i--)//从最后一行往上是为了避免把敌机直接冲出数组。
		if (i <= my_plane_pos + 2 && i >= my_plane_pos - 2 && plane_pos[i] >= length - 5)
		{
			death++;
			plane_pos[i] = -plane_pos[i];
		}
		else if (plane_pos[i] <= length - 1 && plane_pos[i] > 0) plane_pos[i] ++;
		else if (plane_pos[i] == length)plane_pos[i] = -length;
}

void setting()
{
	int sw;
	system("cls");
	do
	{
		sw = 0;
		printf("\n Please set the density of the enemies'planes.:1.Large 2.Middle 3.Small >> ");
		switch (_getche())
		{
		case '0': density = 10;
			break;
		case '1': density = 20;
			break;
		case '2': density = 30;
			break;
		case '3': density = 40;
			break;
		default: printf("\n  error ，Please choose again...\n");
			sw = 1;
		}
	} while (sw);
	do
	{
		sw = 0;
		printf("\n The speed of the enemies' planes. :1.High speed 2.Middle speed 3.Low speed. >> ");
		switch (_getche())
		{
		case '1': speed = 2;
			break;
		case '2': speed = 3;
			break;
		case '3': speed = 4;
			break;
		default: printf("\n  error ，Please choose again...\n");
			sw = 1;
		}
	} while (sw);

	printf("\n Please press any key to save ...");
	_getch();
	system("cls");
	fixed_write();
}

void menu()
{
	printf("Introduction :Press J and L to control the left and right ,W shoot bullets\n\n SETTING :Please press Esc\n\n Start :any key\n\n\n\n\n\n\n\n                               by HZY");
	if (_getch() == 27)
		setting();
}

void plane(int x, int y, bool vis)
{
	/*第一行*/
	createProject(x, y - 3, "\x1E", "\x20", vis);
	/*第二行*/
	createProject(x - 2, y - 2, "\x11\xFE\xFE\xFE\x10", "\x20\x20\x20\x20\x20", vis);
	/*第三行*/
	createProject(x, y - 1, "\xFE", "\x20", vis);
	/*第四行*/
	createProject(x - 1, y, "\xFE\xFE\xFE", "\x20\x20\x20", vis);
}

void createProject(int x, int y, char* wstr, char* dstr, bool vis)
{
	if (vis)write(y, x, wstr);
	else write(y, x, dstr);
}
