#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>

//蛇的状态//
#define U 1//上
#define D 2//下
#define L 3 //左
#define R 4   //右

typedef struct SNAKE //蛇身的一个节点
{
	int x;
	int y;
	struct SNAKE *next;
}snake;

//全局变量//
int life = 1;//生命为0时死亡
int node = 2;//蛇开始时的节点
int score = 0;//分数
int add = 10;//每次吃食物得分。
int status, sleeptime = 300;//每次运行的时间间隔
snake *head, *food,*grass,*boom;//定义蛇头指针，食物指针，毒草指针，炸弹指针
snake *q,*p;//遍历蛇的时候用到的指针
int Gameoverstatus = 0; //游戏结束的情况，1：撞到墙；2：咬到自己；3：按Esc主动退出游戏。

//函数声明//
void Pos();//控制光标位置的函数
void creatMap();//地图
void Map_1();
void Map_2();
void initSnake(); // 初始化蛇身
int biteSelf();//不能咬到自己
void createFood();//食物
void createpoison();//毒草
void createBomb();//炸弹
void notCrossWall();//不能穿墙
void snakeMove();//蛇动
void pause();//暂停
void controlGame();//控制游戏
void initGame();//初始化游戏
void endGame();
void gameStart();
void Paihangbang();//排行榜


void Pos(int x, int y)//设置光标位置
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//返回标准的输入、输出或错误的设备的句柄，也就是获得输入、输出/错误的屏幕缓冲区的句柄
	SetConsoleCursorPosition(hOutput, pos);
}


void creatMap()//创建地图
{
	
		int i;
		for (i = 0; i < 58; i += 2)//打印上下边框，一个方块占两个位置
		{
			Pos(i, 0);
			printf("■");
			Pos(i, 26);
			printf("■");
		}
		for (i = 1; i < 26; i++)//打印左右边框
		{
			Pos(0, i);
			printf("■");
			Pos(56, i);
			printf("■");
		}
	}


  void	Map_1()
	{
		int i, j;
		for (i = 0; i < 58; i += 2)//打印上下边框，一个方块占两个位置
		{
			Pos(i, 0);
			printf("■");
			Pos(i, 26);
			printf("■");
		}
		for (i = 1; i < 26; i++)//打印左右边框
		{
			Pos(0, i);
			printf("■");
			Pos(56, i);
			printf("■");
			   
		}
		for (j = 8; j < 18; j++)//打印中间边框
		{
			Pos(28, j);
			printf("■");
		}
	}


  void	Map_2()
	{
		int i;
		for (i = 0; i < 58; i += 2)//打印上下边框，一个方块占两个位置
		{
			Pos(i, 0);
			printf("■");
			Pos(i, 26);
			printf("■");
		}
		for (i = 1; i < 26; i++)//打印左右边框
		{
			Pos(0, i);
			printf("■");
			Pos(56, i);
			printf("■");
		}
		for (i = 9; i < 15; i++)//打印中间左右边框
		{
			Pos(10, i);
			printf("■");
			Pos(40, i);
			printf("■");
		}
		for (i = 8; i < 32; i+=2)//打印中间上下边框
		{
			Pos(i, 8);
			printf("■");
			Pos(i, 18);
			printf("■");
		}
	}


  void initSnake()//初始化蛇身
  {
	snake *tail;
	int i;
	tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，
	tail->x = 12;
	tail->y = 10;//以x,y设定开始的位置
	tail->next = NULL;
	for (i = 1; i <2; i++)//初始蛇长度为2
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 12 + 2 * i;
		head->y = 10;
		tail = head;
	}
	while (tail != NULL)//从头到尾，输出蛇身
	{
		Pos(tail->x, tail->y);
		printf("■");
		tail = tail->next;
	}
}


int biteSelf()//判断蛇是否咬到了自己
{
	snake *self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)
		{
			life = 0;
			endGame();
			Gameoverstatus = 2;
			return 1;
		}
		self = self->next;
	}
	return 0;
}


void createFood()//随机出现食物
{
	snake *food_1;
	srand((unsigned)time(NULL));//为了防止出现产生的随机数相同的情况，种子设置为time
	food_1 = (snake*)malloc(sizeof(snake));
	while ((food_1->x % 2) != 0)    //保证为偶数，使食物与蛇头对齐
	{
		food_1->x = rand() % 52 + 2;
	}
	food_1->y = rand() % 24 + 1;
	q = head;
	while (q->next == NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
		{
			free(food_1);
			createFood();//如果重合，重新制造食物
		}
		q = q->next;
	}
	Pos(food_1->x, food_1->y);
	food = food_1;
	printf("■");
}


void createpoison()//随机出现毒草
{
	snake  *food_2;
	srand(((unsigned)time(NULL))+1000);//为了防止出现产生的毒草和炸弹,食物重合，使time+1000
	food_2 = (snake*)malloc(sizeof(snake));
	while ((food_2->x % 2) != 0)  
	{
		food_2->x = rand() % 52 + 2;
	}
	food_2->y = rand() % 24 + 1;
	q = head;
	while (q->next == NULL)
	{
		if (q->x == food_2->x && q->y == food_2->y) //判断蛇身是否与毒草重合
		{
			free(food_2);
			createpoison();//如果重合，重新制造食物毒草
		}
		q = q->next;
	}
	Pos(food_2->x, food_2->y);
	grass = food_2;
	printf("◆");
}


void createBomb()
{
	snake  *food_3;
	srand(((unsigned)time(NULL))+5000);//为了防止出现产生的炸弹和毒草,食物重合，使time+5000
	food_3 = (snake*)malloc(sizeof(snake));
	while ((food_3->x % 2) != 0)  
	{
		food_3->x = rand() % 52 + 2;
	}
	food_3->y = rand() % 24 + 1;
	q = head;
	while (q->next == NULL)
	{
		if (q->x == food_3->x && q->y == food_3->y) //判断蛇身是否与炸弹重合
		{
			free(food_3);
			createBomb();//如果重合，重新制造炸弹
		}
		q = q->next;
	}
	Pos(food_3->x, food_3->y);
	 boom = food_3;
	printf("●");
}


void notCrossWall()//蛇不能穿墙的函数
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 26)//蛇撞墙
	{
		
		Gameoverstatus = 1;
		endGame();
		life = 0;

	}
}


void snakeMove()//蛇前进,上U,下D,左L,右R
{
	snake * nexthead;
	notCrossWall();

	nexthead = (snake*)malloc(sizeof(snake));
	if (status == U)//向上移动
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1;
		 if (nexthead->x == food->x && nexthead->y == food->y)//如果下一步和食物重合
		{
						nexthead->next = head;
						head = nexthead;
						q = head;
						while (q != NULL)
						{
							Pos(q->x, q->y);
							printf("■");
							q = q->next;
						}
						node++;
						score = score + add;
						createFood();
					}
		else if (nexthead->x == grass->x && nexthead->y == grass->y)//如果下一步和毒草重合
		{
			//nexthead->next = head;
			head = head->next;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			node--;
			score = score + add;
			createpoison();
		}
		   else if (nexthead->x == boom->x && nexthead->y == boom->y)//如果下一步和炸弹重合
				{
					node = node / 2;
					for (int i = 0; i < node / 2; i++)
					{
						Pos(head->x, head->y);
						printf("  ");
						head = head->next;
						score = score - add;
					}
					q = head;
					while (q != NULL)
					{
						Pos(q->x, q->y);
						printf("■");
						q = q->next;
					}
					createBomb();
				}


		else  //如果下一步和食物，炸弹，毒草不重合
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}
	}
	//向下移动

	if (status == D)
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1;
		if (nexthead->x == food->x && nexthead->y == food->y)  //如果下一步和食物重合
		{
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			node++;
			score = score + add;
			createFood();
		}

		else if (nexthead->x == grass->x && nexthead->y == grass->y)//如果下一步和毒草重合
		{
			p = head;
			free(p);
			head = head->next;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			node--;
			score = score + add;
			createpoison();

		}

       else if (nexthead->x == boom->x && nexthead->y == boom->y)//如果下一步和炸弹重合
		  {

			for (int i = 0; i < node / 2; i++)
			{
				Pos(head->x, head->y);
				printf("  ");
				head = head->next;
				score = score - add;
			}
			node = node / 2;
			q = head;
			while (q != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			createBomb();
		}

		 else                 //如果下一步和食物，炸弹，毒草不重合
		  {
			nexthead->next = head;
			head = nexthead;
			q = head;
			while (q->next->next != NULL)
			{
				Pos(q->x, q->y);
				printf("■");
				q = q->next;
			}
			Pos(q->next->x, q->next->y);
			printf("  ");
			free(q->next);
			q->next = NULL;
		}

	}
	//向左移动

		if (status == L)
		{
			nexthead->x = head->x - 2;
			nexthead->y = head->y;
			if (nexthead->x == food->x && nexthead->y == food->y)// 如果下一步和食物重合
			{
				nexthead->next = head;
				head = nexthead;
				q = head;
				while (q != NULL)
				{
					Pos(q->x, q->y);
					printf("■");
					q = q->next;
				}
				node++;
				score = score + add;
				createFood();
			}

			else if (nexthead->x == grass->x && nexthead->y == grass->y)//如果下一步和毒草重合
			{
				nexthead->next = head;
				head = head->next;
				q = head;
				while (q != NULL)
				{
					Pos(q->x, q->y);
					printf("■");
					q = q->next;
				}
				node--;
				score = score + add;
				createpoison();
			}

			else if (nexthead->x == boom->x && nexthead->y == boom->y)//如果下一步和炸弹重合
			{
				node = node / 2;
				for (int i = 0; i < node / 2; i++)
				{
					Pos(head->x, head->y);
					printf("  ");
					head = head->next;
					score = score - add;
				}
				q = head;
				while (q != NULL)
				{
					Pos(q->x, q->y);
					printf("■");
					q = q->next;
				}
				createBomb();
			}

			else     //如果下一步和食物，毒草，炸弹不重合
			{
				nexthead->next = head;
				head = nexthead;
				q = head;
				while (q->next->next != NULL)
				{
					Pos(q->x, q->y);
					printf("■");
					q = q->next;
				}
				Pos(q->next->x, q->next->y);
				printf("  ");
				free(q->next);
				q->next = NULL;
			}

            }
	  //向右移动

		if (status == R)
		{
			nexthead->x = head->x + 2;
			nexthead->y = head->y;
			if (nexthead->x == food->x && nexthead->y == food->y)//如果下一步和食物重合
			{
				nexthead->next = head;
				head = nexthead;
				q = head;
				while (q != NULL)
				{
					Pos(q->x, q->y);
					printf("■");
					q = q->next;
				}
				node++;
				score = score + add;
				createFood();
			}

			else if (nexthead->x == grass->x && nexthead->y == grass->y)//如果下一步和毒草重合
			{
				//nexthead->next = head;
				head = head->next;
				q = head;
				while (q != NULL)
				{
					Pos(q->x, q->y);
					printf("■");
					q = q->next;
				}
				node--;
				score = score + add;
				createpoison();
			}

			else if (nexthead->x == boom->x && nexthead->y == boom->y)//如果下一步和炸弹重合
			{
				node = node / 2;
				for (int i = 0; i < node / 2; i++)
				{
					Pos(head->x, head->y);
					printf("  ");
					head = head->next;
					score = score - add;
				}
				q = head;
				while (q != NULL)
				{
					Pos(q->x, q->y);
					printf("■");
					q = q->next;
				}
				createBomb();
			}

			else     //如果下一步和食物，毒草，炸弹不重合
			{
				nexthead->next = head;
				head = nexthead;
				q = head;
				while (q->next->next != NULL)
				{
					Pos(q->x, q->y);
					printf("■");
					q = q->next;
				}
				Pos(q->next->x, q->next->y);
				printf("  ");
				free(q->next);
				q->next = NULL;
			}
       }

    if (biteSelf() == 1)       //判断是否会咬到自己
		{
			Gameoverstatus = 2;
			endGame();
		}
	}


void pause()//暂停函数            
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}

	}
}


void controlGame()//控制游戏        
{
	Pos(64, 7);
	printf("■是食物");
	Pos(64, 8);
	printf("◆是毒草");
	Pos(64, 9);
	printf("●是地雷");
	Pos(64, 10);
	printf("注意：蛇不能穿墙，也不能咬到自己");
	Pos(64, 11);
	printf("用↑.↓.←.→分别控制蛇的移动.\n");
	Pos(64, 12);
	printf("F1 为加速，F2 为减速");
	Pos(64, 13);
	printf("退出游戏请按ESC键.");
	Pos(64, 14);
	printf("暂停游戏请按空格键.");
	Pos(64, 15);
	printf("请开始你的表演");
	Pos(64, 16);
	printf("祝您游戏愉快！");
	printf("\n");
	status = R;
	while (1)
	{
		node = 2;
		Pos(64, 5);
		printf("得分：%d  ", score);
		Pos(64, 6);
		printf("每个食物得分：%d分", add);
		if (GetAsyncKeyState(VK_UP) && status != D)//保证蛇不能立即回头
		{
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U)
		{
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R)
		{
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L)
		{
			status = R;
		}
		else if (GetAsyncKeyState(VK_SPACE))//按暂停键
		{
			pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE))//按ESC退出去
		{
			Gameoverstatus = 3;
			break;
		}
		else if (GetAsyncKeyState(VK_F1))//F1加速
		{
			if (sleeptime >= 50)
			{
				sleeptime = sleeptime - 30;
				add = add + 2;
				if (sleeptime == 320)
				{
					add = 2;//防止减到1之后再加回来有错
				}
			}
		}
		else if (GetAsyncKeyState(VK_F2))//F2减速
		{
			if (sleeptime<350)
			{
				sleeptime = sleeptime + 30;
				add = add - 2;
				if (sleeptime == 350)
				{
					add = 1;  //保证最低分为1
				}
			}
		}
		Sleep(sleeptime);
		snakeMove();
		if (score >= 60)
		{
			break;
	}
		if (life == 0)
			endGame();
	
	}
}


void initGame()//游戏开始界面
{
	Pos(40, 10);
	printf("欢迎来到彭永俊的贪吃蛇游戏！");
	Pos(40, 13);
	printf("天才的你一定可以的！\n");
	Pos(40, 16);
	printf("快来挑战吧！\n");
	printf("\n\n\n\n\n\n\n\n\n\n\n");
	system("pause");
	system("cls");
	Pos(25, 10);
	printf("用↑.↓.←.→分别控制蛇的移动 \n");
	Pos(25, 12);
	printf("F1 为加速，F2 为减速\n");
	Pos(25, 14);
	printf("加速能获得更高的分数\n");
	Pos(25, 15);
	printf("祝您游戏愉快！\n");
    printf("\n\n\n\n\n\n\n\n\n\n");
	system("pause");
	system("cls");
}


void endGame()//结束游戏
{
	system("pause");
	system("cls");
	Pos(24, 3);
	if (Gameoverstatus == 1)
	{
		printf("撞到墙了哦。游戏结束.");
	}
	else if (Gameoverstatus == 2)
	{
		printf("咬到自己哦。游戏结束.");
	}
	else if (Gameoverstatus == 3)
	{
		printf("游戏结束。");

	}
	Pos(24, 2);
	printf("您的得分是%d\n", score);
	Paihangbang();
}


void gameStart()//游戏初始化
{
	system("mode con cols=100 lines=30");
	initGame();
	creatMap();
    initSnake();
	 createFood();
	 createpoison();
	 createBomb();
}


void Paihangbang()//排行榜
{
	FILE *fp;
	struct namescore
	{
		char name[50];
		int score;
	}top5[5];
	fopen_s(&fp, "排行榜.txt", "r");
	int i, n = 5;
	char c;
	for (i = 0; i < 5; i++)
	{
		fscanf_s(fp, "%s%d", top5[i].name, 50, &top5[i].score);
		fscanf_s(fp, "%c", &c, 2);
	}
	fclose(fp);
	for (i = 4; i < n; i--)
	{
		if (i >= 0)
		{
			if (score > top5[i].score)
				n--;
			else
				break;
		}
		else
			break;
	}
	if (n != 5)
	{
		fopen_s(&fp, "排行榜.txt", "w");
		char newname[50];
		Pos(18, 10);
		printf("恭喜你的成绩进入排行榜！");
		Pos(18, 12);
		printf("敢问阁下尊姓大名：");
		scanf_s("%s", newname, 50);
		for (i = 4; i > n; i--)
		{
			strcpy_s(top5[i].name, top5[i - 1].name);
			top5[i].score = top5[i - 1].score;
		}
		strcpy_s(top5[n].name, newname);
		top5[n].score = score;
		for (i = 0; i < 5; i++)
		{
			Pos(5, 14);
			printf("恭喜您表现优异进入了排行榜");
			Pos(18, 16 + i);
			printf("%s : %d", top5[i].name, top5[i].score);
		}
		fclose(fp);
		fopen_s(&fp, "排行榜.txt", "w");
		for (i = 0; i < 5; i++)
		{
			fprintf(fp, "%s %d", top5[i].name, top5[i].score);
			fprintf(fp, "\n");
		}
		fclose(fp);
	}
	else if (n == 5)
	{
		Pos(18, 10);
		printf("哎，很遗憾你还没有进入排行榜");
		Pos(18, 12);
		printf("加油！下次你一定可以进入排行榜的");
	}
}


int main()
{
	system("color 0D");
	gameStart();
	controlGame();
	endGame();
	
	system("cls");
	
	Map_1();
	score= 0;
	initSnake();
	createFood();
	createpoison();
	createBomb();
	controlGame();
	endGame();
	system("cls");

	Map_2();
	score = 0;
	initSnake();
	createFood();
	createpoison();
	createBomb();
	controlGame();
	endGame();
	Paihangbang();
	return 0;
}

