#include<stdio.h>
#include<stdlib.h>
#include<time.h>

const int ROW = 7, COL = 7;

void randomize(int* pRow, int* pCol, int grid[ROW][COL]);
void mvmnt(int *pRow, int *pCol, int grid[ROW][COL], int gold, int score);

int main()
{
	int gold = 1, i = 1, j = 1;
	srand((int)time(0));
	int field[ROW][COL];
	int score = 0;

	//Each box getting its values
	randomize(&i, &j, field);

	printf("Enter your box postion through \" \": ");
	scanf_s("%d %d", &i, &j);
	
	mvmnt(&i, &j, field, gold, score);
	return 0;
}

void randomize(int* pRow, int* pCol, int grid[ROW][COL])//Function filling up each box with random value on the game grid 
{
	int Rows = *pRow;
	int Columns = *pCol;
	for (Rows = 1; Rows < 7; Rows++)
	{
		for (Columns = 1; Columns < 7; Columns++)
		{
			grid[Rows][Columns] = (rand() % 255) + 1;
			*pRow = Rows;
			*pCol = Columns;
		}
		printf("\n");
	}
}

void mvmnt(int* pRow, int* pCol, int grid[ROW][COL], int gold, int score)
{
	int Rows = *pRow;
	int Columns = *pCol;
			if (grid[Rows][Columns] & gold)
			{
				score += grid[Rows][Columns] >> 4;
				/*Ці printf ти маєш одразу перевести на ось це 6х6 поле з точками, які з кожним інпутом замінюються літерами G чи T
				Score розберешся як і де поставити. Я певен ти розумієш про що я.*/
				//Або ця функція має під'єднуватись до main чи іншої функції де на початку гри вже є цей grid
				printf("Number %d at your position is gold and your score now: %d", grid[Rows][Columns], score);
			}
			else
			{
				printf("Your number %d is a trap!\t", grid[Rows][Columns]);
			}
}