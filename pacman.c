#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13

typedef struct
{
	int X;
	int Y;
	int tempX;
	int tempY;
	int vector;
	int colour;
}Unit;

int map1[30][40];
int point1[30][40];

int map2[30][40];
int point2[30][40];

int map3[30][40];
int point3[30][40];

int map4[30][40];
int point4[30][40];

int speed;
int life = 2;

void gotoxy(int x, int y){
	//가로x, 세로y 좌표로 커서를 이동시키는 함수
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void textcolor(int color_number){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
	//0x00AB  A(background) B(Font)
	//#define BLACK                SetConsoleTextAttribute(COL, 0x0000);     
	//#define DARK_BLUE         SetConsoleTextAttribute(COL, 0x0001);        
	//#define GREEN                SetConsoleTextAttribute(COL, 0x0002);      
	//#define BLUE_GREEN        SetConsoleTextAttribute(COL, 0x0003);        
	//#define BLOOD               SetConsoleTextAttribute(COL, 0x0004);       
	//#define PURPLE               SetConsoleTextAttribute(COL, 0x0005);     
	//#define GOLD                 SetConsoleTextAttribute(COL, 0x0006);      
	//#define ORIGINAL            SetConsoleTextAttribute(COL, 0x0007);       (ORIGINAL CONSOLE COLOR)
	// #define GRAY                 SetConsoleTextAttribute(COL, 0x0008);       
	//#define BLUE                  SetConsoleTextAttribute(COL, 0x0009);     
	//#define HIGH_GREEN       SetConsoleTextAttribute(COL, 0x000a);      
	//#define SKY_BLUE           SetConsoleTextAttribute(COL, 0x000b);      
	//#define RED                   SetConsoleTextAttribute(COL, 0x000c);      
	//#define PLUM                SetConsoleTextAttribute(COL, 0x000d);      
	//#define YELLOW             SetConsoleTextAttribute(COL, 0x000e);      
	//#define WHITE                SetConsoleTextAttribute(COL, 0x000f);      
}
void removeCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void chg(Unit *);	// 키보드 입력을 받는 함수
void mapf(int);
void collision_test(Unit *, int(*)[30][40]);
void worm(Unit **);	// 방향키를 바꾸는 함수
void delay(int);
void ghost(Unit *, Unit *, int(*)[30][40]);
void food(int);
void scoref();
void gameoverf();
void game1();
void game2();
void game3();
void game4();
void ghost_move(Unit *, Unit *, int, int, int *, int *);
void warp(Unit *, int, int, int, int);
void lifef();
void make_map(int(*)[30][40], int (*)[30][40], int);

void main()
{
	srand((unsigned)time(NULL));	// 난수 생성

	unsigned char keyb = 99;
	char ch;
	int i = 1;
	int stop = 0;
	int select = 0;
	int select_speed = 0;


	make_map(map1, point1, 1);
	make_map(map2, point2, 2);
	make_map(map3, point3, 3);
	make_map(map4, point4, 4);


	while (1)
	{
		printf("Choose game speed\n");
		printf("1) Fast 2) Normal 3) Slow");
		ch = getch();

		switch (ch)
		{
		case '1':
			select_speed = 1;
			speed = 100;
			break;
		case '2':
			select_speed = 1;
			speed = 150;
			break;
		case '3':
			select_speed = 1;
			speed = 250;
			break;
		default :
			system("cls");
			printf("choose proper one !");
			delay(500);
			system("cls");
			break;
		}
		if (select_speed == 1)	break;
	}


	removeCursor();

	// 맵 선택
	while (1)
	{
		gotoxy(57, 5);
		printf("Map");
		gotoxy(62, 5);
		printf("%d / 4", i);
		gotoxy(57, 6);
		printf("Press Enter to start!");

		switch (i)
		{
		case 1:
			mapf(map1);
			food(point1);
			break;
		case 2:
			mapf(map2);
			food(point2);
			break;
		case 3:
			mapf(map3);
			food(point3);
			break;
		case 4:
			mapf(map4);
			food(point4);
		}

		keyb = getch();
		if (keyb == 0 || keyb == 0xE0)	keyb = getch();	// 방향키는 특수문자 이므로 한 번 더 입력받음
		// 입력받은 키보드 수에 따라 방향을 결정

		if (keyb == LEFT && i > 1)	i--;
		if (keyb == RIGHT && i < 4)	i++;
		if (keyb == ENTER)
		{
			select = i;
			break;
		}
	}

	system("cls");

	switch (select)
	{
	case 1:
		game1();
		break;
	case 2:
		game2();
		break;
	case 3:
		game3();
		break;
	case 4:
		game4();
		break;
	}
}
void game1()
{
	Unit Pacman = { 24, 16, 24, 16, 5 };
	Unit Ghost1 = { 2, 1, 2, 1, 5, 0x000c };
	Unit Ghost2 = { 52, 1, 50, 1, 5, 0x0003 };
	Unit Ghost3 = { 2, 27, 2, 27, 5, 0x000d };
	Unit Ghost4 = { 52, 27, 52, 27, 5, 0x000b };
	int move = 0;
	int score = 0;	// 점수 tot = 2760
	int gameover = 0;

	mapf(map1);	// 맵 출력

	food(point1);	// 먹이 출력
	scoref();	// 스코어보드 출력
	lifef();	// 라이프보드 출력


	while (1)
	{
		if (score == 2760)
		{
			gameoverf();
			printf("You won!");
			gotoxy(15, 18);
			break;
		}

		if (life > 0 && gameover == 1)
		{

			life -= 1;
			if (life == 1)
			{
				// 목숨 값을 하나 지움
				gotoxy(66, 6);
				printf("  ");
				// 팩맨 좌표 초기화
				Pacman.X = 24;
				Pacman.Y = 16;
				printf("\a");
				gameover = 0;
			}
			if (life == 0)
			{
				gotoxy(64, 6);
				printf("  ");
				printf("\a");
			}

		}

		if (life == 0 && gameover == 1)
		{
			gameoverf();
			break;
		}

		// 팩맨 이동
		chg(&Pacman);	// 방향 변환
		collision_test(&Pacman, &map1);	// 팩맨과 블럭 충돌 검사
		worm(&Pacman);	// 팩맨의 방향 변화

		gotoxy(Pacman.tempX, Pacman.tempY);	// 팩맨의 이동 흔적 지움
		if (Pacman.vector != 5)	printf("  ");

		gotoxy(Pacman.X, Pacman.Y);
		textcolor(0x000e);	// 노란색으로 팩맨 출력
		printf("●");
		textcolor(0x0007);

		// 팩맨의 이전 좌표 저장
		Pacman.tempX = Pacman.X;
		Pacman.tempY = Pacman.Y;

		// 유령
		ghost_move(&Ghost1, &Pacman, map1, point1, &move, &gameover);
		ghost_move(&Ghost2, &Pacman, map1, point1, &move, &gameover);
		ghost_move(&Ghost3, &Pacman, map1, point1, &move, &gameover);
		ghost_move(&Ghost4, &Pacman, map1, point1, &move, &gameover);

		delay(speed);	// 속도 설정

		// 먹이 표시
		if (point1[Pacman.Y][Pacman.X / 2] == 0)
		{
			point1[Pacman.Y][Pacman.X / 2] = 1;	// 포인터를 먹으면 흔적을 지워버림
			score += 10;
		}
		gotoxy(65, 5);
		printf("%d / 2760", score);	// 점수 표시

		// 워프
		warp(&Pacman, 2, 12, 56, 12);
		warp(&Pacman, 56, 12, 2, 12);

		warp(&Ghost1, 2, 12, 56, 12);
		warp(&Ghost1, 56, 12, 2, 12);

		warp(&Ghost2, 2, 12, 56, 12);
		warp(&Ghost2, 56, 12, 2, 12);

		warp(&Ghost3, 2, 12, 56, 12);
		warp(&Ghost3, 56, 12, 2, 12);

		warp(&Ghost4, 2, 12, 56, 12);
		warp(&Ghost4, 56, 12, 2, 12);
	}
}
void game2()
{
	Unit Pacman = { 24, 16, 24, 16, 5 };
	Unit Ghost1 = { 2, 1, 2, 1, 5, 0x000c };
	Unit Ghost2 = { 52, 1, 52, 1, 5, 0x0003 };
	Unit Ghost3 = { 2, 27, 2, 27, 5, 0x000d };
	Unit Ghost4 = { 52, 27, 52, 27, 5, 0x000b };
	int move = 0;
	int score = 0;	// 점수 tot = 4020
	int gameover = 0;

	mapf(map2);	// 맵 출력

	food(point2);	// 먹이 출력
	scoref();	// 스코어보드 출력
	lifef();

	while (1)
	{
		if (score == 4020)
		{
			gameoverf();
			printf("You won!");
			gotoxy(15, 18);
			break;
		}

		if (life > 0 && gameover == 1)
		{
			life -= 1;
			if (life == 1)
			{
				// 목숨 값을 하나 지움
				gotoxy(66, 6);
				printf("  ");
				// 팩맨 좌표 초기화
				Pacman.X = 24;
				Pacman.Y = 16;
				printf("\a");
				gameover = 0;
			}
			if (life == 0)
			{
				gotoxy(64, 6);
				printf("  ");
				printf("\a");
			}

		}

		if (life == 0 && gameover == 1)
		{
			gameoverf();
			break;
		}

		// 팩맨 이동
		chg(&Pacman);	// 방향 변환
		collision_test(&Pacman, &map2);	// 팩맨과 블럭 충돌 검사
		worm(&Pacman);	// 팩맨의 방향 변화

		gotoxy(Pacman.tempX, Pacman.tempY);	// 팩맨의 이동 흔적 지움
		if (Pacman.vector != 5)	printf("  ");

		gotoxy(Pacman.X, Pacman.Y);
		textcolor(0x000e);	// 노란색으로 팩맨 출력
		printf("●");
		textcolor(0x0007);

		// 팩맨의 이전 좌표 저장
		Pacman.tempX = Pacman.X;
		Pacman.tempY = Pacman.Y;

		// 유령
		ghost_move(&Ghost1, &Pacman, map2, point2, &move, &gameover);
		ghost_move(&Ghost2, &Pacman, map2, point2, &move, &gameover);
		ghost_move(&Ghost3, &Pacman, map2, point2, &move, &gameover);
		ghost_move(&Ghost4, &Pacman, map2, point2, &move, &gameover);

		delay(speed);	// 속도 설정

		// 먹이 표시
		if (point2[Pacman.Y][Pacman.X / 2] == 0)
		{
			point2[Pacman.Y][Pacman.X / 2] = 1;	// 포인터를 먹으면 흔적을 지워버림
			score += 10;
		}
		gotoxy(65, 5);
		printf("%d / 4020", score);	// 점수 표시

		// 워프
		warp(&Pacman, 0, 12, 54, 12);
		warp(&Pacman, 54, 12, 2, 12);

		warp(&Ghost1, 0, 12, 54, 12);
		warp(&Ghost1, 54, 12, 2, 12);

		warp(&Ghost2, 0, 12, 54, 12);
		warp(&Ghost2, 54, 12, 2, 12);

		warp(&Ghost3, 0, 12, 54, 12);
		warp(&Ghost3, 54, 12, 2, 12);

		warp(&Ghost4, 0, 12, 54, 12);
		warp(&Ghost4, 54, 12, 2, 12);


	}
}
void game3()
{
	Unit Pacman = { 24, 16, 24, 16, 5 };
	Unit Ghost1 = { 2, 1, 2, 1, 5, 0x000c };
	Unit Ghost2 = { 52, 1, 52, 1, 5, 0x0003 };
	Unit Ghost3 = { 2, 27, 2, 27, 5, 0x000d };
	Unit Ghost4 = { 52, 27, 52, 27, 5, 0x000b };
	int move = 0;
	int score = 0;	// 점수 tot = 2980
	int gameover = 0;

	mapf(map3);	// 맵 출력

	food(point3);	// 먹이 출력
	scoref();	// 스코어보드 출력
	lifef();

	while (1)
	{
		if (score == 2980)
		{
			gameoverf();
			printf("You won!");
			gotoxy(15, 18);
			break;
		}
		if (life > 0 && gameover == 1)
		{
			life -= 1;
			if (life == 1)
			{
				// 목숨 값을 하나 지움
				gotoxy(66, 6);
				printf("  ");
				// 팩맨 좌표 초기화
				Pacman.X = 24;
				Pacman.Y = 16;
				printf("\a");
				gameover = 0;
			}
			if (life == 0)
			{
				gotoxy(64, 6);
				printf("  ");
				printf("\a");
			}

		}

		if (life == 0 && gameover == 1)
		{
			gameoverf();
			break;
		}
		// 팩맨 이동
		chg(&Pacman);	// 방향 변환
		collision_test(&Pacman, &map3);	// 팩맨과 블럭 충돌 검사
		worm(&Pacman);	// 팩맨의 방향 변화

		gotoxy(Pacman.tempX, Pacman.tempY);	// 팩맨의 이동 흔적 지움
		if (Pacman.vector != 5)	printf("  ");

		gotoxy(Pacman.X, Pacman.Y);
		textcolor(0x000e);	// 노란색으로 팩맨 출력
		printf("●");
		textcolor(0x0007);

		// 팩맨의 이전 좌표 저장
		Pacman.tempX = Pacman.X;
		Pacman.tempY = Pacman.Y;

		// 유령
		ghost_move(&Ghost1, &Pacman, map3, point3, &move, &gameover);
		ghost_move(&Ghost2, &Pacman, map3, point3, &move, &gameover);
		ghost_move(&Ghost3, &Pacman, map3, point3, &move, &gameover);
		ghost_move(&Ghost4, &Pacman, map3, point3, &move, &gameover);

		delay(speed);	// 속도 설정

		// 먹이 표시
		if (point3[Pacman.Y][Pacman.X / 2] == 0)
		{
			point3[Pacman.Y][Pacman.X / 2] = 1;	// 포인터를 먹으면 흔적을 지워버림
			score += 10;
		}
		gotoxy(65, 5);
		printf("%d / 2980", score);	// 점수 표시

		// 워프
		warp(&Pacman, 0, 12, 54, 12);
		warp(&Pacman, 54, 12, 2, 12);

		warp(&Ghost1, 0, 12, 54, 12);
		warp(&Ghost1, 54, 12, 2, 12);

		warp(&Ghost2, 0, 12, 54, 12);
		warp(&Ghost2, 54, 12, 2, 12);

		warp(&Ghost3, 0, 12, 54, 12);
		warp(&Ghost3, 54, 12, 2, 12);

		warp(&Ghost4, 0, 12, 54, 12);
		warp(&Ghost4, 54, 12, 2, 12);
	}
}
void game4()
{
	Unit Pacman = { 24, 16, 24, 16, 5 };
	Unit Ghost1 = { 2, 1, 2, 1, 5, 0x000c };
	Unit Ghost2 = { 52, 1, 52, 1, 5, 0x0003 };
	Unit Ghost3 = { 2, 27, 2, 27, 5, 0x000d };
	Unit Ghost4 = { 52, 27, 52, 27, 5, 0x000b };
	int move = 0;
	int score = 0;	// 점수 tot = 4020
	int gameover = 0;

	mapf(map4);	// 맵 출력

	food(point4);	// 먹이 출력
	scoref();	// 스코어보드 출력
	lifef();

	while (1)
	{
		
		if (score == 3570)
		{
			gameoverf();
			printf("You won!");
			gotoxy(15, 18);
			break;
		}
		// 게임 오버 조건
		if (life > 0 && gameover == 1)
		{
			life -= 1;
			if (life == 1)
			{
				// 목숨 값을 하나 지움
				gotoxy(66, 6);
				printf("  ");
				// 팩맨 좌표 초기화
				Pacman.X = 24;
				Pacman.Y = 16;
				printf("\a");
				gameover = 0;
			}
			if (life == 0)
			{
				gotoxy(64, 6);
				printf("  ");
				printf("\a");
			}

		}

		if (life == 0 && gameover == 1)
		{
			gameoverf();
			break;
		}
		
		// 유령
		ghost_move(&Ghost1, &Pacman, map4, point4, &move, &gameover);
		ghost_move(&Ghost2, &Pacman, map4, point4, &move, &gameover);
		ghost_move(&Ghost3, &Pacman, map4, point4, &move, &gameover);
		ghost_move(&Ghost4, &Pacman, map4, point4, &move, &gameover);

		// 팩맨 이동
		chg(&Pacman);	// 방향 변환
		collision_test(&Pacman, &map4);	// 팩맨과 블럭 충돌 검사
		worm(&Pacman);	// 팩맨의 방향 변화

		gotoxy(Pacman.tempX, Pacman.tempY);	// 팩맨의 이동 흔적 지움
		if (Pacman.vector != 5)	printf("  ");

		gotoxy(Pacman.X, Pacman.Y);
		textcolor(0x000e);	// 노란색으로 팩맨 출력
		printf("●");
		textcolor(0x0007);

		// 팩맨의 이전 좌표 저장
		Pacman.tempX = Pacman.X;
		Pacman.tempY = Pacman.Y;



		delay(speed);	// 속도 설정

		// 먹이 표시
		if (point4[Pacman.Y][Pacman.X / 2] == 0)
		{
			point4[Pacman.Y][Pacman.X / 2] = 1;	// 포인터를 먹으면 흔적을 지워버림
			score += 10;
		}
		gotoxy(65, 5);
		printf("%d / 3570", score);	// 점수 표시

	}
}
void delay(int times)
{
	int start_time;
	start_time = clock();
	while (clock() - start_time < times);
}
void worm(Unit *units)
{
	switch (units->vector){	// 입력받은 방향키에 따라 좌표에 값을 더함
	case 1:	// UP
		units->Y -= 1;
		break;
	case 2:	// DOWN
		units->Y += 1;
		break;
	case 3:	// LEFT
		units->X -= 2;	// 특수문자는 2바이트 이므로 *2를 해줌
		break;
	case 4:	// RIGHT
		units->X += 2;
		break;
	}
}
void chg(Unit *units)
{
	/*		1은 UP, 2는 DOWN
	3은 LEFT, 4는 RIGHT
	for vector		*/
	unsigned char keyb = 99;
	if (kbhit())
	{	// 키보드 입력받은 경우
		keyb = getch();
		if (keyb == 0 || keyb == 0xE0)	keyb = getch();	// 방향키는 특수문자 이므로 한 번 더 입력받음
	}
	// 입력받은 키보드 수에 따라 방향을 결정
	if (keyb == UP)	units->vector = 1;
	if (keyb == DOWN)	units->vector = 2;
	if (keyb == LEFT) units->vector = 3;
	if (keyb == RIGHT) units->vector = 4;
}
void collision_test(Unit *units, int map[30][40])
{
	int i, j;	// i는 X축, j는 Y축
	i = (units->X) / 2;
	j = units->Y;
	/*		1은 UP, 2는 DOWN
	3은 LEFT, 4는 RIGHT
	for vector		*/
	switch (units->vector)
	{
	case 1:	// 위로 가는 경우
		j--;
		if (map[j][i] == 1)	units->vector = 5;
		break;
	case 2:	// 아래로 가는 경우
		j++;
		if (map[j][i] == 1)	units->vector = 5;
		break;
	case 3:	// 왼쪽으로 가는 경우
		i--;
		if (map[j][i] == 1)	units->vector = 5;
		break;
	case 4:
		i++;
		if (map[j][i] == 1)	units->vector = 5;
		break;
	}
}
void mapf(int map[30][40])
{
	int i, j;// 맵의 크기

	gotoxy(0, 0);
	for (i = 0; i < 30; i++)
	{
		for (j = 0; j < 28; j++)
		{
			textcolor(0x0009);
			if (map[i][j] == 1)	printf("■");
			else printf("  ");
		}
		printf("\n");
	}
	textcolor(0x0007);
}
void food(int point[30][40])
{
	int i, j;
	// 맵 출력과는 반대로 배열에 0이 있으면 출력
	gotoxy(0, 0);
	for (i = 0; i < 29; i++)
	{
		for (j = 0; j < 28; j++)
		{
			if (point[i][j] == 0)
			{
				textcolor(0x0003);
				gotoxy(j * 2, i);
				printf("ㆍ");
			}
		}
	}
	textcolor(0x0007);
}
void scoref()
{
	// 스코어보드 출력
	// 목숨 등 추가 예정
	gotoxy(57, 5);
	printf("Score :");
}
void gameoverf()
{
	int i, j;
	char ch;
	gotoxy(0, 0);
	for (i = 0; i < 29; i++)
	{
		for (j = 0; j < 28; j++)
		{
			textcolor(0x0009);
			printf("■");
		}
		printf("\n");
		delay(30);
	}
	textcolor(0x0007);
	gotoxy(2, 1);
	for (i = 0; i < 28; i++)
	{
		for (j = 0; j < 26; j++)
		{
			textcolor(0x0009);
			printf("  ");
		}
		gotoxy(2, i + 1);
		delay(30);
	}
	textcolor(0x0007);

	gotoxy(22, 15);
	printf("Game Over !");
	gotoxy(15, 16);
	printf("Press any key to continue...");
	gotoxy(15, 17);
	ch = getch();
}
void ghost(Unit *Pacman, Unit *Ghost, int(*map)[30][40])
{
	/*
	유령이 팩맨을 쫓아가는 알고리즘
	유령이 팩맨을 보면 그 방향으로 틀어서 쫓아간다
	만약 팩맨이 코너를 돌아서 더이상 유령이 팩맨을 볼 수 없는 경우에는
	더 이상 쫓아가지 않고 다른 랜덤한 방향으로 간다.
	*/

	int i;
	int ran = rand() % 4 + 1;	// 1 ~ 4 사이의 난수 생성

	// 팩맨과 같은 Y값을 갖는 경우
	if (Ghost->Y == Pacman->Y)
	{
		// 팩맨이 왼쪽에 있는 경우
		if (Ghost->X > Pacman->X)
		{
			for (i = Ghost->X / 2; i > Pacman->X / 2; i--)
			{
				if (map[Ghost->Y][i] == 1)	// 벽이 있는 경우
				{
					break;
				}

			}
			if (i == Pacman->X / 2) Ghost->vector = 3;
		}
		// 팩맨이 오른쪽에 있는 경우
		else if (Ghost->X < Pacman->X)
		{
			for (i = Ghost->X / 2; i < Pacman->X / 2; i++)
			{
				if (map[Ghost->Y][i] == 1)	// 벽이 있는 경우
					break;
			}
			if (i == Pacman->X / 2) Ghost->vector = 4;
		}
	}
	// 팩맨과 같은 X값을 갖는 경우
	else if (Ghost->X == Pacman->X)
	{
		// 팩맨이 아래쪽에 있는 경우
		if (Ghost->Y > Pacman->Y)
		{
			for (i = Ghost->Y; i > Pacman->Y; i--)
			{
				if (map[i][Ghost->X] == 1)	// 벽이 있는 경우
					break;
			}
			if (i == Pacman->Y) Ghost->vector = 1;
		}
		// 팩맨이 위쪽에 있는 경우
		else if (Ghost->Y < Pacman->Y)
		{
			for (i = Ghost->Y; i < Pacman->Y; i++)
			{
				if (map[i][Ghost->X] == 1)	// 벽이 있는 경우
					break;
			}
			if (i == Pacman->Y) Ghost->vector = 2;
		}
	}
	else Ghost->vector = ran;	// 찾지 못한 경우 랜덤한 방향으로 이동

}
void ghost_move(Unit *Ghost, Unit *Pacman, int map, int(*point)[40], int *move, int *gameover)
{
	if (Ghost->vector == 5 || *move == 3)
	{
		ghost(Pacman, Ghost, map);
		*move = 0;
	}
	*move += 1;	// 유령의 움직인 횟수 증가

	if (Ghost->vector != 5)	// 유령이 움직이는 경우
	{
		gotoxy(Ghost->tempX, Ghost->tempY);	// 유령의 이동 흔적 지움
		if (point[Ghost->tempY][Ghost->tempX / 2] == 1)	printf("  ");
		else
		{
			textcolor(0x0003);
			printf("ㆍ");
			textcolor(0x0007);
		}

	}

	collision_test(Ghost, map);	// 벽과 충돌 검사
	worm(Ghost);
	gotoxy(Ghost->X, Ghost->Y);
	textcolor(Ghost->colour);	// 유령을 빨간색으로 출력
	printf("◎");
	textcolor(0x0007);

	// 유령의 이전 좌표 저장
	Ghost->tempX = Ghost->X;
	Ghost->tempY = Ghost->Y;

	if (Pacman->X == Ghost->X && Pacman->Y == Ghost->Y)	*gameover = 1;
}
void warp(Unit *units, int X, int Y, int gotoX, int gotoY)
{
	if ((units->X == X && units->Y == Y) && (units->tempX == X && units->tempY == Y))
	{
		units->X = gotoX;
		units->Y = gotoY;
	}
}
void lifef()
{
	gotoxy(57, 6);
	printf("Life : ");
	textcolor(0x000e);
	printf("●●");
}
void make_map(int(*map)[30][40], int (*point)[30][40], int chooser)
{
	int x = 0, y = 0;

	FILE *mapp;
	FILE *map_point;
	switch (chooser)
	{
	case 1:
		mapp = fopen("map1.txt", "r");
		map_point = fopen("point1.txt", "r");
		break;
	case 2:
		mapp = fopen("map2.txt", "r");
		map_point = fopen("point2.txt", "r");
		break;
	case 3:
		mapp = fopen("map3.txt", "r");
		map_point = fopen("point3.txt", "r");
		break;
	case 4:
		mapp = fopen("map4.txt", "r");
		map_point = fopen("point4.txt", "r");
		break;
	}
	if (mapp == NULL || map_point == NULL)	printf("파일 개방 실패!");


	char read_txt;
	char read_point;
	while (1)
	{
		while (1)
		{
			read_txt = fgetc(mapp);
			if (read_txt == '}' || read_txt == EOF)	break;
			if (read_txt == '{' || read_txt == ' ' || read_txt == ',' || read_txt == '\n')	continue;
			if (read_txt == '0') read_txt = 0;
			else if (read_txt == '1')	read_txt = 1;
			(*map)[y][x] = read_txt;
			x++;
		}
		x = 0;
		y++;
		if (read_txt == EOF)	break;
	}
	x = 0; y = 0;
	while (1)
	{
		while (1)
		{
			read_point = fgetc(map_point);
			if (read_point == '}' || read_point == EOF)	break;
			if (read_point == '{' || read_point == ' ' || read_point == ',' || read_point == '\n')	continue;
			if (read_point == '0') read_point = 0;
			else if (read_point == '1')	read_point = 1;
			(*point)[y][x] = read_point;
			x++;
		}
		x = 0;
		y++;
		if (read_point == EOF)	break;
	}
}