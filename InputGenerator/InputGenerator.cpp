// InputGenerator.cpp: определяет точку входа для консольного приложения.
//

#define _CRT_SECURE_NO_WARNINGS

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

void printGeneratedInput(int minDrons, int maxDrons, int minTargets, int maxTargets, int maxCoord, int minTime, int maxTime) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937 gen(seed);
	int drons = gen() % (maxDrons - minDrons + 1) + minDrons;
	int targets = gen() % (maxTargets - minTargets + 1) + minTargets;
	cout << drons << endl << targets << endl << gen() % (maxCoord * targets) << endl;
	vector<pair<double, double> > points = { { gen() % maxCoord, gen() % maxCoord } };
	for (int i = 0; i < targets; ++i) {
		points.push_back({ gen() % maxCoord, gen() % maxCoord });
	}

	for (auto& pt : points) {
		cout << fixed << setprecision(3) << pt.first << " " << pt.second << endl;
	}

	cout << endl;

	for (auto& pt : points) {
		int start_time = gen() % (maxTime - minTime + 1) + minTime;
		int end_time = gen() % (maxTime - minTime + 1) + minTime;
		if (start_time > end_time) {
			swap(start_time, end_time);
		}
		cout << start_time << " " << end_time << endl;
	}
}

void printPreciseInput(int drons, int targets, int maxCoord) {
	printGeneratedInput(drons, drons, targets, targets, maxCoord, 0, 3 * maxCoord * targets / drons);
}

void printClusteredInput(int drons, int targets, int maxCoord, int clustersCnt) {
	int minTime = 0, maxTime = 3 * maxCoord * targets / drons;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937 gen(seed);
	
	cout << drons << endl << targets << endl << gen() % (max(1, maxCoord * targets / 4)) << endl;
	vector<pair<double, double> > points = { { gen() % maxCoord, gen() % maxCoord } };

	int clusters = clustersCnt;

	for (int i = 0; i < clusters; ++i) {
		points.push_back({ gen() % maxCoord, gen() % maxCoord });
		while (true) {
			double minDist = 1e9;
			for (int j = 0; j + 1 < (int)points.size(); ++j) {
				minDist = min(minDist, hypot(points.back().first - points[j].first, points.back().second - points[j].second));
			}
			if (minDist < maxCoord / (2 + sqrt(2)))
				points[(int)points.size() - 1] = make_pair(gen() % maxCoord, gen() % maxCoord);
			else
				break;
		}
	}

	int r = max(1, (int)(maxCoord / (clusters + sqrt(2))));

	for (int i = clusters; i < targets; ++i) {
		int clusterInd = rand() % clusters + 1;

		points.push_back({ gen() % r + points[clusterInd].first, gen() % r + points[clusterInd].second });
	}
	
	for (auto& pt : points) {
		cout << fixed << setprecision(3) << pt.first << " " << pt.second << endl;
	}

	cout << endl;

	for (auto& pt : points) {
		int start_time = gen() % (maxTime - minTime + 1) + minTime;
		int end_time = gen() % (maxTime - minTime + 1) + minTime;
		if (start_time > end_time) {
			swap(start_time, end_time);
		}
		cout << start_time << " " << end_time << endl;
	}
}

int main(int argc, char** argv) {
	if ((string)argv[1] == "help") {
		cout << "Possible commands:" << endl;
		cout << "1) rand dronsCnt targetsCnt [fileName]" << endl;
		cout << "2) clusters dronsCnt targetsCnt clustersCnt [fileName]" << endl;
		return 0;
	} 
	if ((string)argv[1] == "rand") {
		if (argc == 4) {
			printPreciseInput(atoi(argv[2]), atoi(argv[3]), 100);
		}
		else if (argc == 5) {
			string fileName = argv[4];
			fileName += ".txt";
			freopen(fileName.c_str(), "w", stdout);
			printPreciseInput(atoi(argv[2]), atoi(argv[3]), 100);
		}
		else {
			cout << "not correct parameters" << endl;
		}
	}
	else if ((string)argv[1] == "clusters") {
		if (argc == 5) {
			printClusteredInput(atoi(argv[2]), atoi(argv[3]), 1000, atoi(argv[4]));
		} else if (argc == 6) {
			string fileName = argv[5];
			fileName += ".txt";
			freopen(fileName.c_str(), "w", stdout);
			printClusteredInput(atoi(argv[2]), atoi(argv[3]), 1000, atoi(argv[4]));
		}
		else {
			cout << "not correct parameters" << endl;
		}
	}
	else {
		cout << "unknown function" << endl;
	}
	
}

