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

using namespace std;

int map[9][9];
int FirstRow[8] = { 1,2,3,4,6,7,8,9 };		//学号后两位是13，终局左上角应为5,第一行剩下8个数字


void GenerateFirstRow()
{
	map[0][0] = 5;
	for (int i = 0; i < 8; i++)
	{
		map[0][i + 1] = FirstRow[i];
	}
	if (next_permutation(FirstRow, FirstRow + 8))
	{
		for (int i = 0; i < 8; i++)
			FirstRow[8] = i;
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
	srand((unsigned)time(NULL));
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

void Output()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			cout << map[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}



int main()
{
	clock_t startTime, endTime;
	startTime = clock();
	FILE *fp = fopen("sudoku.txt", "w+");
	for (int count = 1; count <= 3; count++)
	{
		GenerateFirstRow();
		Generate(1, 0);
		//Output();

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				fprintf(fp, "%d", map[i][j]);
				if (j != 8)
					fputc(' ', fp);
			}
			fputc('\n', fp);

		}
		fputc('\n', fp);
	}
	fclose(fp);
	endTime = clock();
	cout << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	system("pause");
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
