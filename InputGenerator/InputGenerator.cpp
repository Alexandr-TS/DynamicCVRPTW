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


void genInput(int cnt_vehicles, int size_km, int cnt_targets) {
	unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
	mt19937 gen(seed);

	cout << cnt_vehicles << endl << cnt_targets << endl << gen() % (size_km * cnt_targets) << endl;
	cout << gen() % 10 + 1 << endl;

	vector<pair<double, double>> points = { { gen() % (1000 * size_km), gen() % (1000 * size_km)} };
	for (int i = 0; i < cnt_targets; ++i) {
		points.push_back({ gen() % (1000 * size_km), gen() % (1000 * size_km)});
	}

	for (int i = 0; i < (int)points.size() - 1; i++) {
		auto pt = points[i + 1ll];
		cout << fixed << setprecision(2) << pt.first - points[0].first << " " << pt.second - points[0].second << endl;
	}

	cout << endl;

	
	// km/h
	const int basic_speed = 60;
	const int add_minutes = 3;
	exponential_distribution<> e_d_gen(1.5);
	vector<vector<double>> distances(points.size(), vector<double>(points.size(), 0.0));
	for (size_t i = 0; i < distances.size(); ++i) {
		for (size_t j = 0; j < distances.size(); ++j) {
			double dist_m = hypot(points[i].first - points[j].first, points[i].second - points[j].second);
			double dist_minutes = dist_m / 1000. * 60.0 / (basic_speed / (1.0 + e_d_gen(gen)));
			if (i != j) {
				dist_minutes += add_minutes;
			}
			distances[i][j] = static_cast<int>(dist_minutes * 100) / 100.;
		}
	}
	for (size_t k = 0; k < distances.size(); ++k) {
		for (size_t i = 0; i < distances.size(); ++i) {
			for (size_t j = 0; j < distances.size(); ++j) {
				distances[i][j] = min(distances[i][j], distances[i][k] + distances[k][j]);
			}
		}
	}
	for (size_t i = 0; i < distances.size(); ++i) {
		for (auto x : distances[i]) {
			cout << fixed << setprecision(2) << x << " ";
		}
		cout << endl;
	}
	cout << endl;

	int min_hours = 9;
	int max_hours = 18;

	for (int i = 0; i < (int)points.size() - 1; i++) {
		int start_time = gen() % (max_hours - min_hours + 1) + min_hours;
		int end_time = gen() % (max_hours - min_hours + 1) + min_hours;
		if (start_time > end_time) {
			swap(start_time, end_time);
		}
		while (end_time == start_time) {
			end_time = gen() % (max_hours - min_hours + 1) + min_hours;
			if (start_time > end_time) {
				swap(start_time, end_time);
			}
		}
		cout << start_time * 60 << " " << end_time * 60 << endl;
	}

}

void printGeneratedInput(int minDrons, int maxDrons, int minTargets, int maxTargets, int maxCoord, int minTime, int maxTime) {
	unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
	mt19937 gen(seed);
	int drons = gen() % (maxDrons - minDrons + 1) + minDrons;
	int targets = gen() % (maxTargets - minTargets + 1) + minTargets;
	cout << drons << endl << targets << endl << gen() % (maxCoord * targets) << endl;
	vector<pair<double, double> > points = { { gen() % maxCoord, gen() % maxCoord } };
	for (int i = 0; i < targets; ++i) {
		points.push_back({ gen() % maxCoord, gen() % maxCoord });
	}

	for (int i = 0; i < (int)points.size() - 1; i++) {
		auto pt = points[i + 1ll];
		cout << fixed << setprecision(3) << pt.first - points[0].first << " " << pt.second - points[0].second << endl;
	}

	cout << endl;

	for (int i = 0; i < (int)points.size() - 1; i++) {
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



int main(int argc, char** argv) {
	if ((string)argv[1] == "help") {
		cout << "Possible commands:" << endl;
		cout << "1) gen vehiclesCnt citySizeKm targetsCnt [fileName]" << endl;
		return 0;
	} 
	FILE* stream;
	/*
	if ((string)argv[1] == "old") {
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
	else 
		*/
	if ((string)argv[1] == "gen") {
		if (argc == 5) {
			genInput(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		}
		else if (argc == 6) {
			string file_name = argv[5];
			file_name += ".txt";
			FILE* stream;
			freopen_s(&stream, file_name.c_str(), "w", stdout);
			genInput(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		}
	}
	else {
		cout << "unknown function" << endl;
	}
}

