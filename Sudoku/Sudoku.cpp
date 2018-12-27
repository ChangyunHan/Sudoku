// Sudoku.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#pragma warning(disable : 4996)

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stack>
#include <fstream>
#include <cstring>

using namespace std;

int map[9][9];


/*
	生成数独终局部分
*/


int FirstRow[8] = { 1,2,3,4,6,7,8,9 };		//学号后两位是13，终局左上角应为5,第一行剩下8个数字


void GenerateFirstRow()
{
	map[0][0] = 5;
	for (int i = 0; i < 8; i++)
	{
		map[0][i + 1] = FirstRow[i];
	}
	while (next_permutation(FirstRow, FirstRow + 8))
	{
		break;
	} 
}


bool Check(int Row, int Col)
{
	int m = map[Row][Col];
	for (int i = 0; i < Row; i++)
	{
		if (m == map[i][Col])
			return false;
	}

	for (int i = 0; i < Col; i++)
	{
		if (m == map[Row][i])
			return false;
	}

	int RowS = Row / 3 * 3;
	int ColS = Col / 3 * 3;
	int ColE = ColS + 2;
	for (int i = RowS; i <= Row; i++)
		for (int j = ColS; j <= ColE; j++)
		{
			if (i >= Row && j >= Col)
				break;
			if (m == map[i][j])
				return false;
		}
	return true;
}


bool Generate(int Row, int Col)
{
	int NextRow;
	int NextCol;
	stack<int> s;
	int buffer[9];
	for (int i = 0; i < 9; i++) {
		buffer[i] = 1 + rand() % 9;
		for (int j = 0; j < i; j++)
			if (buffer[i] == buffer[j])
			{
				i--;
				break;
			}
	}
	for (int i = 0; i < 9; i++)
		s.push(buffer[i]);
	while (!s.empty())
	{
		map[Row][Col] = s.top();
		s.pop();
		if (!Check(Row, Col))
			continue;
		if (Col == 8)
		{
			if (Row == 8)
				return  true;
			else {
				NextCol = 0;
				NextRow = Row + 1;
			}
		}
		else
		{
			NextRow = Row;
			NextCol = Col + 1;
		}
		bool Next = Generate(NextRow, NextCol);
		if (Next) return true;
	}
	if (s.empty())
		return false;
	return true;
}

void GenerateSudoku(int num)
{
	FILE *fp = fopen("sudoku.txt", "w+");
	for (int count = 1; count <= num; count++)
	{
		GenerateFirstRow();
		Generate(1, 0);
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (j != 8)
					fprintf(fp,"%d ", map[i][j]);
				else
					fprintf(fp, "%d", map[i][j]);
			}
			fputc('\n', fp);

		}
		fputc('\n', fp);
	}
	fclose(fp);
}

/*
	求解数独部分
*/

int N;		//记录需要填数（也就是0）的个数
int address[81][2];		//记录每个空白的位置（行数和列数
bool inf[81][9];		//记录每个空白可以填的数

bool Check_2(int Row, int Col)
{
	int m = map[Row][Col];
	for (int i = 0; i < 9; i++)
	{
		if (map[i][Col] == 0 || i==Row)
			continue;
		if (m == map[i][Col])
			return false;
	}
	for (int i = 0; i < 9; i++)
	{	
		if (map[i][Col] == 0 || i == Col)
			continue;
		if (m == map[Row][i])
			return false;
	}
	int RowS = Row / 3 * 3;
	int ColS = Col / 3 * 3;
	int ColE = ColS + 2;
	int RowE = RowS + 2;
	for (int i = RowS; i <= RowE; i++)
		for (int j = ColS; j <= ColE; j++)
		{
			if (i == Row && j == Col)
				continue;
			if (map[i][j] == 0)
				continue;
			if (m == map[i][j])
				return false;
		}
	return true;
}

void Pretreatment()
{
	N = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (map[i][j] == 0)
			{
		
				address[N][0] = i;
				address[N][1] = j;
				N++;
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			map[address[i][0]][address[i][1]] = j+1;
			if (Check_2(address[i][0], address[i][1]))
				inf[i][j] = true;
			else inf[i][j] = false;
		}
		map[address[i][0]][address[i][1]] = 0;
	}
}

bool Solving(int count)
{
	if (count < 1)
		return true;
	int n = N - count;
	for (int j = 0; j < 9; j++)
	{
		if (inf[n][j])
		{
			map[address[n][0]][address[n][1]] = j + 1;
			if (Check_2(address[n][0], address[n][1]))
			{
				if(Solving(count-1))
				return true;
			}
		}
	}
	return false;
}


void SolvingSudoku(char path[])
{
	FILE *question = fopen(path, "r");
	FILE *answer = fopen("sudoku.txt", "w+");
	while (fscanf(question,"%d",&map[0][0])!=EOF)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (i == 0 && j == 0)
					continue;
				else {
					if (!fscanf(question, "%d", &map[i][j]))
						cout << "读取失败" << endl;
				}
			}
		}
		Pretreatment();
		if (!Solving(N))
			fprintf(answer, "Solving Sudoku failed\n");
		else {
			
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					if (!Check_2(i, j))
					{
						fprintf(answer, "Solving Sudoku failed1\n");
						return;
					}
				}
			}
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					if (j != 8)
						fprintf(answer, "%d ", map[i][j]);
					else
						fprintf(answer, "%d", map[i][j]);
				}
				fputc('\n', answer);

			}
		}
		fputc('\n', answer);
	}
	fclose(answer);
	fclose(question);
}



int main(int argc, char *argv[])
{
	/*clock_t startTime, endTime;
	startTime = clock();
	srand((unsigned)time(NULL));*/




	if (argc != 3)
		cout << "参数错误" << endl;
	else
	{
		if (argv[1][1] == 'c'){
			int N = atoi(argv[2]);
			if (N == 0)
				cout << "参数错误" << endl;
			else
			{
				GenerateSudoku(N);
			}
		}
		else {
			char path[100]={0};
			int l;
			l = strlen(argv[2]);
			int n=0;
			for (int i = 0; i < l; i++)
			{
				if (argv[2][i] == '\\')
				{
					path[n++] = '\\';
					path[n++] = '\\';
				}
				else
				{
					path[n++] = argv[2][i];
				}

			}
			SolvingSudoku(path);
		}
	}
	

	/*char c[] = { "C:\\Users\\韩昌云\\Desktop\\新建文件夹\\problems.txt" };	
	SolvingSudoku(c);*/
	/*GenerateSudoku(10000);*/

	/*endTime = clock();
	cout << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;*/


}


