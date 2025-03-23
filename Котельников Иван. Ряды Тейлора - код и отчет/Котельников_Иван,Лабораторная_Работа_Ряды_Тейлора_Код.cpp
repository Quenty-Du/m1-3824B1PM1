#include "stdio.h"
#include "stdlib.h"
#include "locale.h"
#include "math.h"

double exponent(double element, double previousElement, double prePreviousElement, int counter, double argument) {
	element = previousElement * argument / counter;
	return element;
}

double logarithm(double element, double previousElement, double prePreviousElement, int counter, double argument) {
	element = previousElement * (counter - 1) * (argument - 1) * (-1) / counter;
	return element;
}

double theSine(double element, double previousElement, double prePreviousElement, int counter, double argument) {
	element = prePreviousElement * argument * argument / (counter * (counter - 1)) * (-1);
	return element;
}

double theCosine(double element, double previousElement, double prePreviousElement, int counter, double argument) {
	element = prePreviousElement * argument * argument / (counter * (counter - 1)) * (-1);
	return element;
}

double straight(double (*next)(double, double, double, int, double), double* polynome, int length, double argument) {
	double result = polynome[0] + polynome[1];
	for (int counter = 2; counter < length; counter++) {
		polynome[counter] = next(polynome[counter], polynome[counter - 1], polynome[counter - 2], counter, argument);
		result += polynome[counter];
	}
	return result;
}

double reverse(double* polynome, int length) {
	double result = 0;
	for (int counter = length - 1; counter >= 0; counter--) {
		result += polynome[counter];
	}
	return result;
}

double couple(double* polynome, int length) {
	double result;
	double* innerPolynome;
	int step = 1, memoredCounter;
	innerPolynome = (double*)malloc(length * sizeof(double));
	for (int counter = 0; counter < length; counter++) {
		innerPolynome[counter] = polynome[counter];
	}
	while (step * 2 <= length) {
		step *= 2;
		memoredCounter = 0;
		for (int counter = step - 1; counter < length; counter += step) {
			innerPolynome[counter] += innerPolynome[counter - step / 2];
			innerPolynome[counter - step / 2] = 0;
			if (counter + step >= length)
				memoredCounter = counter;
		}
		if (length - memoredCounter - 1 >= step / 2) {
			innerPolynome[memoredCounter] += innerPolynome[memoredCounter + step / 2];
			innerPolynome[memoredCounter + step / 2] = 0;
		}
	}
	result = innerPolynome[step - 1];
	return result;
}

int main() {
	// Этап нулевой. Тут вводятся переменные
	setlocale(LC_ALL, "Russian");
	int funcName = 0;

	int correctChecker = 1;
	int polynomeLength;

	const char* best[4] = { "Прямое", "Обратное", "Попарное", "Все способы суммирования вернули одинаковую точность или различаются но крайне незначительно" };
	const char* Dx = "";

	double bestError[2] = { 1000, 0 };
	double error[3];
	double* polynome;

	double resultStraight, resultReverse, resultCouple, theIdeal, argument = 0;

	// Этап первый. Тут вводится номер выбранной функции, после чего все это будет использоваться в ветках
	while (correctChecker == 1) {
		printf("Выберите функцию для вычисления из списка, введя число, соответствующее ее номеру:\n1. e^x\n2. ln(x)\n3. sin(x)\n4. cos(x)\n");
		scanf_s("%d", &funcName);
		system("cls");
		if (funcName >= 1 && funcName <= 4) {
			correctChecker = 0;
		}
		else {
			printf("Вы ввели недопустимое значение. Повторите попытку:\n");
		}
	}

	// Этап второй. Тут пользователь вводит необходимое значение аргумента
	if (funcName == 2) {
		Dx = "0 < x <= 2";
	}
	printf("Введите значение аргумента функции: %s\n", Dx);
	scanf_s("%lf", &argument);
	system("cls");

	// Этап третий. Вводится количество членов в полиноме при разложении по Тейлору
	printf("Введите количество членов в разложении по Тейлору:\n");
	scanf_s("%d", &polynomeLength);
	system("cls");

	// Этап четвертый. Передается шаблон функции в указатель, определяется идеальное значение (оно же значение встроенной функции)
	polynome = (double*)malloc(polynomeLength * sizeof(double));
	double (*func)(double, double, double, int, double);
	if (funcName == 1) {
		func = exponent;
		polynome[0] = 1;
		if (polynomeLength > 1) {
			polynome[1] = argument;
		}
		theIdeal = exp(argument);
	}
	else if (funcName == 2) {
		func = logarithm;
		polynome[0] = 0;
		if (polynomeLength > 1) {
			polynome[1] = argument - 1;
		}
		theIdeal = log(argument);
	}
	else if (funcName == 3) {
		func = theSine;
		polynome[0] = 0;
		if (polynomeLength > 1) {
			polynome[1] = argument;
		}
		theIdeal = sin(argument);
	}
	else {
		func = theCosine;
		polynome[0] = 1;
		if (polynomeLength > 1) {
			polynome[1] = 0;
		}
		theIdeal = cos(argument);
	}

	// Этап пятый. Вычисление значения функции путем суммирования всех членов полинома различными способами.
	if (polynomeLength > 2) {
		resultStraight = straight(func, polynome, polynomeLength, argument);
		resultReverse = reverse(polynome, polynomeLength);
		resultCouple = couple(polynome, polynomeLength);
	}
	else {
		resultStraight = polynome[0] + polynome[1];
		resultReverse = resultStraight;
		resultCouple = resultStraight;
	}

	// Этап шестой. Тут вычисляется погрешность и определяется самый точный способ суммирования из имеющихся
	error[0] = theIdeal - resultStraight;
	error[1] = theIdeal - resultReverse;
	error[2] = theIdeal - resultCouple;
	for (int i = 0; i < 3; i++) {
		if (bestError[0] >= fabs(error[i])) {
			bestError[0] = fabs(error[i]);
			bestError[1] = i;
		}
	}
	if (error[0] == error[1] && error[1] == error[2]) {
		bestError[1] = 3;
	}

	// Этап седьмой. Тут происходит вывод информации на экран
	printf("Результат встроенной функции: %.16lf\nРезультат прямого суммирования: %.16lf\n\
Результат обратного суммирования: %.16lf\nРезультат попарного суммирования: %.16lf\nЛучший результат: %s", theIdeal, resultStraight, resultReverse, resultCouple, best[int(bestError[1])]);
}