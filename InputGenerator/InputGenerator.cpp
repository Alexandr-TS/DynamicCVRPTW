// InputGenerator.cpp: определяет точку входа для консольного приложения.
//

#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <iomanip>
#include <cstring>
#include <chrono>

using namespace std;

void printGeneratedInput(int minDrons, int maxDrons, int minTargets, int maxTargets, int maxCoord) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937 gen(seed);
	int drons = gen() % (maxDrons - minDrons + 1) + minDrons;
	int targets = gen() % (maxTargets - minTargets + 1) + minTargets;
	cout << drons << endl << targets << endl << gen() % (maxCoord * targets) << endl;
	vector<pair<double, double> > points = { {0, 0} };
	for (int i = 0; i < targets; ++i) {
		points.push_back({ gen() % maxCoord, gen() % maxCoord });
	}

	for (size_t i = 0; i < points.size(); ++i) {
		for (size_t j = 0; j < points.size(); ++j)
			cout << fixed << setprecision(3) << hypot(points[i].first - points[j].first, points[i].second - points[j].second) << " ";
		cout << endl;
	}
}

void printPreciseInput(int drons, int targets, int maxCoord) {
	printGeneratedInput(drons, drons, targets, targets, maxCoord);
}

int main(int argc, char** argv)
{
	if (argc == 3) {
		printPreciseInput(atoi(argv[1]), atoi(argv[2]), 100);
	}
	else {
		int num = atoi(argv[3]);
		string fileName = "test";
		fileName += (char)('0' + num / 10);
		fileName += (char)('0' + num % 10);
		fileName += ".txt";
		freopen(fileName.c_str(), "w", stdout);
		printPreciseInput(atoi(argv[1]), atoi(argv[2]), 100);
	}
}

