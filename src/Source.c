#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h> // динамическое выделение памяти
#include <string.h> // strcpy(x, y)
#include <math.h> // pow(x, y), abs(x)
#include <locale.h> // руссификация
#define MAX_COUNT_CHARS 80 // максимальное кол-во символов в названии входного файла

int* matrixSumOfDet; // матрица для вывода "вычислений" определителя по Лапласу

// Определяет, где больше всего нулей - в строке или столбце
int FindMaxZero(const int* const* const matrix, int n, int* maxI, int* maxJ);
// Рекурсивная формула Лапласа
int Laplas(const int* const* const matrix, int size);
// Вывод матрицы
void PrintMatrix(const int* const* const matrix, int n);
int n = 0;
// Прямой ход Гаусса, возвращает коэффициент для определителя
int Gauss(int** matrix, int n);
// Перестановка строк
void SwapRows(int** matrix, int n, int row1, int row2);
// Умножение строки на число
void MultByNumber(int** matrix, int n, int row, int number);
// Вычитание строк
void SubRows(int** matrix, int n, int row1, int row2, int coefficient);
// Сложение строк
void AddRows(int** matrix, int n, int row1, int row2, int coefficient);

void main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	char file_name[MAX_COUNT_CHARS];
	int** matrix;
	int i = 0, j = 0;
	int resultLaplas = 0;
	int coefficientByGauss = 1; // коэффициент, полученный при умножении строки на ведущий эл-т
	int determinant = 1;
	FILE* fin; // входной файл

	if (argc <= 1)
	{
		printf("Нет аргументов\n");
		return;
	}

	strcpy(file_name, argv[1]);
	if ((fin = fopen(file_name, "r")) == NULL)
	{
		printf("Не удалось открыть файл\n");
		return;
	}
	// Чтение размера
	fscanf(fin, "%d\n", &n);
	matrixSumOfDet = (int*)malloc(n * sizeof(int));
	// Установка матрицы
	matrix = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; ++i)
	{
		matrix[i] = (int*)malloc(n * sizeof(int));
		for (int j = 0; j < n; ++j)
			fscanf(fin, "%d", &matrix[i][j]);
	}
	fclose(fin);

	printf("\tВычисление определителя по рекурсивной формуле:\n\n");
	// Обнуление "вычислений"
	for (int p = 0; p < n; p++)
		matrixSumOfDet[p] = 0;

	PrintMatrix(matrix, n);

	resultLaplas = Laplas(matrix, n);
	printf("Определитель матрицы по рекурсивной формуле:\n");
	// Красивое форматирование вывода
	if (n == 2)
		printf("%d\n", resultLaplas);
	else
	{
		printf("%d", matrixSumOfDet[0]);
		for (int k = 1; k < n; k++)
			if (matrixSumOfDet[k] >= 0)
				printf(" + %d", abs(matrixSumOfDet[k]));
			else
				printf(" - %d", abs(matrixSumOfDet[k]));
		printf(" = %d", resultLaplas);
	}
	free(matrixSumOfDet);

	printf("\n==========================================================\n");
	printf("\tВычисление определителя методом Гаусса:\n\n");

	PrintMatrix(matrix, n);
	printf("\n");

	coefficientByGauss = Gauss(matrix, n);

	// Красивый вывод
	printf("Определитель матрицы по методу Гаусса: \n");
	if (coefficientByGauss < 0)
		printf("1/(%d) * ", coefficientByGauss);
	else
		printf("1/%d * ", coefficientByGauss);
	if (matrix[0][0] < 0)
		printf("(%d)", matrix[0][0]);
	else
		printf("%d", matrix[0][0]);
	determinant *= matrix[0][0];
	for (int i = 1; i < n; i++)
	{
		determinant *= matrix[i][i];
		if (matrix[i][i] < 0)
			printf(" * (%d)", matrix[i][i]);
		else
			printf(" * %d", matrix[i][i]);
	}
	determinant /= coefficientByGauss;
	printf(" = %d", determinant);

	for (int m = 0; m < n; m++)
		free(matrix[m]);
	free(matrix);
}

// Определяет, где больше всего нулей - в строке или столбце
//возвращает 1 - в строке больше
//возвращает 0 - в столбце 
//к тому же возвращает номер строки и столбца
//(по умолчанию, если нет нулей, возвращается последняя строка и первый столбец)
int FindMaxZero(const int* const* const matrix, int n, int* maxI, int* maxJ)
{
	// по умолчанию
	*maxI = n - 1; int indexRow = n - 1;
	*maxJ = 0; int indexCol = 0;
	// кол-во нулей, максимальное кол-во нулей
	int zeroRow = 0, maxZeroRow = 0;
	int zeroCol = 0, maxZeroCol = 0;
	int flagFind = 0; // флаг проверки на наличие нулей

	for (int i = 0; i < n; i++)
	{
		flagFind = 0;
		zeroRow = 0;
		zeroCol = 0;
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] == 0)
			{
				zeroRow++;
				indexRow = i;
				flagFind = 1;
			}
			if (matrix[j][i] == 0)
			{
				zeroCol++;
				indexCol = i;
				flagFind = 1;
			}
		}
		if (zeroRow >= maxZeroRow && flagFind)
		{
			maxZeroRow = zeroRow;
			*maxI = indexRow;
		}
		if (zeroCol >= maxZeroCol && flagFind)
		{
			maxZeroCol = zeroCol;
			*maxJ = indexCol;
		}
	}
	if (maxZeroRow >= maxZeroCol)
		return 1;
	else
		return 0;
}

void PrintMatrix(const int* const* const matrix, int n)
{
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}

int Laplas(const int* const* const matrix, int size)
{
	if (size == 1)
	{
		matrixSumOfDet[0] = matrix[0][0];
		return matrix[0][0];
	}
	if (size == 2)
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

	int countSum = 0; // переменная кол-ва "вычислений" определителя
	int valueLaplas = 0; // перемення для сокращаения кода
	int result = 0;
	int sign = 1; // для знака + или -
	int extraResult = 0;
	int** extraMatrix;
	int i = 0, j = 0;
	if (FindMaxZero(matrix, size, &i, &j))
	{
		(size == n) ? printf("Разложение по %d строке\n", i + 1) : 0;
		//Цикл сложения доп. миноров
		for (int cols = 0; cols < size; cols++)
		{
			if (matrix[i][cols] == 0)
				continue;
			sign = pow(-1.0, i + cols);
			if (size == n)
				printf("%d\n", sign * matrix[i][cols]);

			// Создание доп. минора
			extraMatrix = (int**)malloc((size - 1) * sizeof(int*));
			for (int m = 0; m < (size - 1); m++)
				extraMatrix[m] = (int*)malloc((size - 1) * sizeof(int));
			int extraI = 0, extraJ = 0;
			for (int eI = 0; eI < size; eI++)
			{
				extraJ = 0;
				int flagNextString = 0;
				for (int eJ = 0; eJ < size; eJ++)
				{
					if (eI != i && eJ != cols)
					{
						extraMatrix[extraI][extraJ++] = matrix[eI][eJ];
						flagNextString = 1;
					}
				}
				if (flagNextString)
					extraI++;
			}
			// Основая рекурсия
			if (size == n)
			{
				PrintMatrix(extraMatrix, size - 1);
				sign = pow(-1, i + cols);
				valueLaplas = Laplas(extraMatrix, size - 1);
				result += sign * matrix[i][cols] * valueLaplas;
				printf(" = %d * %d = %d\n", sign * matrix[i][cols], valueLaplas, sign * valueLaplas * matrix[i][cols]);
				matrixSumOfDet[countSum++] = sign * valueLaplas * matrix[i][cols];

				for (int m = 0; m < (size - 1); m++)
					free(extraMatrix[m]);
				free(extraMatrix);
			}
			else
			{
				valueLaplas = Laplas(extraMatrix, size - 1);
				sign = pow(-1, i + cols);
				extraResult += sign * valueLaplas * matrix[i][cols];

				for (int m = 0; m < (size - 1); m++)
					free(extraMatrix[m]);
				free(extraMatrix);
			}
		}
		if (size < n)
			return extraResult;
		else
			return result;
	}
	else
	{
		(size == n) ? printf("Разложение по %d столбцу\n", j + 1) : 0;
		for (int rows = 0; rows < size; rows++)
		{
			if (matrix[rows][j] == 0)
				continue;
			sign = pow(-1.0, i + rows);
			if (size == n)
				printf("%d\n", sign * matrix[rows][j]);

			// Создание доп. минора
			extraMatrix = (int**)malloc((size - 1) * sizeof(int*));
			for (int m = 0; m < (size - 1); m++)
				extraMatrix[m] = (int*)malloc((size - 1) * sizeof(int));
			int extraI = 0, extraJ = 0;
			for (int eI = 0; eI < size; eI++)
			{
				extraJ = 0;
				int flagNextString = 0;
				for (int eJ = 0; eJ < size; eJ++)
				{
					if (eI != rows && eJ != j)
					{
						extraMatrix[extraI][extraJ++] = matrix[eI][eJ];
						flagNextString = 1;
					}
				}
				if (flagNextString)
					extraI++;
			}
			// Основая рекурсия
			if (size == n)
			{
				PrintMatrix(extraMatrix, size - 1);
				sign = pow(-1, rows + j);
				valueLaplas = Laplas(extraMatrix, size - 1);
				result += sign * matrix[rows][j] * valueLaplas;
				printf(" = %d * %d = %d\n", sign * matrix[rows][j], valueLaplas, sign * valueLaplas * matrix[rows][j]);
				matrixSumOfDet[countSum++] = sign * valueLaplas * matrix[rows][j];

				for (int m = 0; m < (size - 1); m++)
					free(extraMatrix[m]);
				free(extraMatrix);
			}
			else
			{
				valueLaplas = Laplas(extraMatrix, size - 1);
				sign = pow(-1, rows + j);
				extraResult += sign * valueLaplas * matrix[rows][j];

				for (int m = 0; m < (size - 1); m++)
					free(extraMatrix[m]);
				free(extraMatrix);
			}
		}
		if (size < n)
			return extraResult;
		else
			return result;
	}
}

int Gauss(int** matrix, int n)
{
	int row = 1, col = 0;
	int headerIndElement = 0;
	int result = 1;
	int isElementInCol = 0; // есть ли ненулевой эл-т в столбце, не считая ведующий эл-т

	// Проход по столбцам
	for (col = 0; col < (n - 1); col++)
	{
		isElementInCol = 0;
		// Поиск ненулевого ведущего эл-та
		headerIndElement = row = col;
		if (!matrix[headerIndElement][col])
		{
			int flagSwap = 0;
			for (int p = row + 1; p < n; p++)
				if (matrix[p][col])
				{
					SwapRows(matrix, n, p, headerIndElement);
					result *= -1;
					flagSwap = 1;
					printf("(%d) <-> (%d)\n", p + 1, headerIndElement + 1);
					PrintMatrix(matrix, n);
					printf("\n");
					break;
				}
			if (!flagSwap) // весь столбец из нулей, начиная с ведущего эл-та
				continue;
		}
		for (row = col + 1; row < n; row++)
		{
			// Пропуск нулевых эл-тов
			if (!matrix[row][col])
				continue;

			else
			{
				isElementInCol = 1;
				// Каждый шаг целочисленные эл-ты
				if (matrix[row][col] % matrix[headerIndElement][col])
				{
					printf("(%d) * %d\n", row + 1, matrix[headerIndElement][col]);
					// Умножение строки на ведущий эл-т
					MultByNumber(matrix, n, row, matrix[headerIndElement][col]);
					result *= matrix[headerIndElement][col];
					PrintMatrix(matrix, n);
					printf("\n");
				}
				int coefficient = matrix[row][col] / matrix[headerIndElement][col];
				if (coefficient < 0)
				{
					printf("(%d) + %d*(%d)\n", row + 1, -coefficient, headerIndElement + 1);
					AddRows(matrix, n, row, headerIndElement, -coefficient);
				}
				else
				{
					printf("(%d) - %d*(%d)\n", row + 1, coefficient, headerIndElement + 1);
					SubRows(matrix, n, row, headerIndElement, coefficient);
				}
			}
		}
		if (isElementInCol)
		{
			PrintMatrix(matrix, n);
			printf("\n");
		}
	}
	return result;
}

void SwapRows(int** matrix, int n, int row1, int row2)
{
	int temp;
	for (int j = 0; j < n; j++)
	{
		temp = matrix[row1][j];
		matrix[row1][j] = matrix[row2][j];
		matrix[row2][j] = temp;
	}
}

void MultByNumber(int** matrix, int n, int row, int number)
{
	for (int j = 0; j < n; j++)
		matrix[row][j] *= number;
}

void SubRows(int** matrix, int n, int row1, int row2, int coefficient)
{
	for (int j = 0; j < n; j++)
		matrix[row1][j] -= coefficient * matrix[row2][j];
}

void AddRows(int** matrix, int n, int row1, int row2, int coefficient)
{
	for (int j = 0; j < n; j++)
		matrix[row1][j] += coefficient * matrix[row2][j];
}
