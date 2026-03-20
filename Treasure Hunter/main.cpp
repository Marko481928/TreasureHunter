#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main()
{
	int gold = 1;
	const int row = 6, col = 6;
	srand((int)time(0));
	int field[row][col];

	//Each box getting its values
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			field[i][j] = (rand() % 255) + 1;
			printf("%d\n", field[i][j]);
			int* p = &field[i][j];
		}
	}

	//Checking the bits
	for (int i = 0; i <= 1; i++)
	{
		printf("Enter the position on the grid: ");
		scanf_s("%d %d", field[i][i]);
		int* p = &field[i][i];
		printf("Your number is: %d", field[i][i]);
	
	}



	return 0;
}