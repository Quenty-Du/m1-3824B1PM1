#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "malloc.h"
#include "locale.h"
#include "time.h"
#include "string.h"

float* insertionSort(float* basicArr, int baseLen) {
	float temp;
	int j;
	int count = 0;
	for (int i = 1; i < baseLen; i++) {
		temp = basicArr[i];
		j = i - 1;
		while (j >= 0 && basicArr[j] >= temp) {
			basicArr[j + 1] = basicArr[j];
			j--;
			count += 2;
		}
		j++;
		count++;
		basicArr[j] = temp;
	}
	printf("Количество сравнений во вставках: %d\n", count);
	return basicArr;
}

float* shellSort(float* basicArr, int baseLen) {
	float temp;
	int k;
	int step = baseLen - 1;
	int count = 0;
	while (step > 0) {
		count++;
		for (int i = step; i < baseLen; i++) {
			temp = basicArr[i];
			k = i - step;
			while (k >= 0 && basicArr[k] >= temp) {
				basicArr[k + step] = basicArr[k];
				k -= step;
				count += 2;
			}
			count++;
			k += step;
			basicArr[k] = temp;
		}
		step /= 2;
	}
	printf("Количество сравнений в Шелла: %d\n", count);
	return basicArr;
}

float* mergeSort(float* basicArr, int baseLen) {
	float* supportArr;
	int count = 0;
	supportArr = (float*)malloc(baseLen * sizeof(float));
	int order = 1, iFirst, iSecond, ending, orderSecond, orderFirst;
	for (int j = 0; j < baseLen; j++) {
		supportArr[j] = basicArr[j];
	}
	while (order < baseLen) {
		count++;
		for (int i = 0; i < ((baseLen + order - 1) / (order * 2)); i++) {
			iFirst = 0;
			iSecond = 0;
			ending = 0;
			orderFirst = order;
			orderSecond = order - ((i * order * 2 + order * 2) / (baseLen + 1)) * (order - (baseLen % order));
			for (int k = i * order * 2; iFirst < orderFirst && iSecond < orderSecond; k++) {
				if (basicArr[i * order * 2 + iFirst] < basicArr[i * order * 2 + order + iSecond]) {
					supportArr[k] = basicArr[i * order * 2 + iFirst];
					iFirst++;
				}
				else {
					supportArr[k] = basicArr[i * order * 2 + order + iSecond];
					iSecond++;
				}
				ending = k + 1;
				count++;
			}
			for (int k = iFirst; k < orderFirst; k++) {
				supportArr[ending] = basicArr[i * order * 2 + k];
				ending++;
			}
			for (int k = iSecond; k < orderSecond; k++) {
				supportArr[ending] = basicArr[i * order * 2 + order + k];
				ending++;
			}
		}
		for (int j = 0; j < baseLen; j++) {
			basicArr[j] = supportArr[j];
		}
		order *= 2;
	}
	printf("Количество сравнений в Merge: %d\n", count);
	return basicArr;
}

int* indexSummCreate(float* basicArr, int baseLen, int step, int element, int* indexSumm) {
	int tempBefore = 0;
	int tempAfter = 0;
	unsigned  char* internalArr;
	memset(indexSumm, 0, 256 * sizeof(int));
	internalArr = (unsigned char*)basicArr;
	for (int i = 0; i < baseLen; i++) {
		indexSumm[internalArr[i * step + element]]++;
	}
	tempBefore = indexSumm[0];
	indexSumm[0] = 0;
	for (int i = 1; i < 256; i++) {
		tempAfter = indexSumm[i];
		indexSumm[i] = indexSumm[i - 1] + tempBefore;
		tempBefore = tempAfter;
	}
	return indexSumm;
}

float* radixSort(float* basicArr, int baseLen) {
	int finalCount = 0;
	int count = 0;
	float* supportArr;
	int* indexSumm = (int*)malloc(256 * sizeof(int));
	unsigned char* internalArr;
	int step = 4;
	int element = 0;
	supportArr = (float*)malloc(baseLen * sizeof(float));
	for (int i = 0; i < 2; i++) {
		internalArr = (unsigned char*)basicArr;
		indexSumm = indexSummCreate(basicArr, baseLen, step, element, indexSumm);
		for (int j = 0; j < baseLen; j++) {
			supportArr[indexSumm[internalArr[j * step + element]]] = basicArr[j];
			count++;
			indexSumm[internalArr[j * step + element]]++;
		}
		element++;
		internalArr = (unsigned char*)supportArr;
		indexSumm = indexSummCreate(supportArr, baseLen, step, element, indexSumm);
		for (int j = 0; j < baseLen; j++) {
			basicArr[indexSumm[internalArr[j * step + element]]] = supportArr[j];
			count++;
			indexSumm[internalArr[j * step + element]]++;
		}
		element++;
	}
	for (int i = baseLen - 1; i > indexSumm[127] - 1; i--) {
		supportArr[finalCount] = basicArr[i];
		count++;
		finalCount++;
	}
	for (int i = 0; i < indexSumm[127]; i++) {
		supportArr[finalCount] = basicArr[i];
		count++;
		finalCount++;
	}
	basicArr = supportArr;
	printf("Количество присваиваний в Radix: %d\n", count);
	return basicArr;
}

int correctCheck(float* before, float* after, int checkLen) {
	int result = 1;
	for (int i = 0; i < checkLen; i++) {
		for (int j = 0; j < checkLen; j++) {
			if (before[i] == after[j])
				break;
			if (j == checkLen - 1 && before[i] != after[j])
				result = 0;
		}
	}
	for (int i = 1; i < checkLen; i++) {
		if (after[i] < after[i - 1]) {
			result = 0;
			break;
		}
	}
	return result;
}

int main() {
	setlocale(LC_ALL, "Russian");
	float* basicArr;
	float* basicCopy;
	clock_t start, end;
	double sortTime;
	int baseLen, sortNum = 0, isPrint = 0, sortCorrect, inputCheck, doCheck;

	printf("Вам необходимо ввести натуральное число (в пределах разумного). Это будет длина массива, который позже подвергнется казни сортировкой:\n");
	scanf_s("%d", &baseLen);
	system("cls");
	printf("Введете массив самостоятельно, или сгенерируете? (1 - Самостоятельно, 0 - Сгенерирую)\n");
	scanf_s("%d", &inputCheck);
	system("cls");
	basicArr = (float*)malloc(baseLen * sizeof(float)); // Заполнение массива элементами >>>
	basicCopy = (float*)malloc(baseLen * sizeof(float)); // Заполнение массива элементами >>>
	if (inputCheck == 0) {
		for (int i = 0; i < baseLen; i++) {
			basicArr[i] = pow(-1, i) * ((float)rand() / (rand() / 3));
			basicCopy[i] = basicArr[i];
		}
	}
	else {
		for (int i = 0; i < baseLen; i++)
			scanf_s("%f", &basicArr[i]);
	}
	system("cls");// Выбор сортировки, вывод массива после сортировки >>>
	printf("На выбор есть 4 сортировки:\n1. Сортировка вставками\n2. Сортировка Шелла\n3. Merge Sort\n4. Radix Sort\n\nВведите номер желаемой сортировки:\n");
	scanf_s("%d", &sortNum);
	system("cls");
	printf("Хотите ли вы вывести массив в виде до сортировки и после сортировки? Если да - введите 1. Если нет - введите 0:\n");
	scanf_s("%d", &isPrint);
	system("cls");
	switch (sortNum) { // Процесс сортировки >>>
	case 1:
		start = clock();
		basicArr = insertionSort(basicArr, baseLen);
		end = clock();
		break;
	case 2:
		start = clock();
		basicArr = shellSort(basicArr, baseLen);
		end = clock();
		break;
	case 3:
		start = clock();
		basicArr = mergeSort(basicArr, baseLen);
		end = clock();
		break;
	case 4:
		start = clock();
		basicArr = radixSort(basicArr, baseLen);
		end = clock();
	}
	if (isPrint == 1) {
		printf("До сортировки:");
		for (int i = 0; i < baseLen; i++)
			printf("\n%d: %f", i, basicCopy[i]);
		printf("\n\nПосле сортировки:");
		for (int i = 0; i < baseLen; i++)
			printf("\n%d: %f", i, basicArr[i]);
		printf("\n\n");
	}
	sortTime = (double)(end - start) / (CLOCKS_PER_SEC);
	printf("Время выполнения сортировки данным алгоритмом: %lf секунд", sortTime);
	printf("\nХотите проверить корректность сортировки? Введите 1, если да, или 0, если нет:\n");
	scanf_s("%d", &doCheck);
	sortCorrect = correctCheck(basicCopy, basicArr, baseLen);
	if (sortCorrect == 0) {
		printf("Сортировка выполнена НЕКОРРЕКТНО");
	}
	else {
		printf("Сортировка выполнена КОРРЕКТНО");
	}
}