#include "Utils.h"

std::mt19937 randGen = std::mt19937(0);

InputData DataGenerator::GenInputData(int minDronsCnt, int maxDronsCnt, int minTargetsCnt,
	int maxTargetsCnt, double minMaxDist, double maxMaxDist, double minCoordinate, double maxCoordinate) {

	int dronsCnt(GenInt(minDronsCnt, maxDronsCnt));
	int targetsCnt(GenInt(minTargetsCnt, maxTargetsCnt));
	double maxDist(GenDouble(minMaxDist, maxMaxDist));

	std::pair<double, double> dronsCoordinate({
		GenDouble(minCoordinate, maxCoordinate),
		GenDouble(minCoordinate, maxCoordinate)
	});

	std::vector<std::pair<double, double> > targets;
	for (int i = 0; i < targetsCnt; ++i) {
		targets.emplace_back(
			GenDouble(minCoordinate, maxCoordinate),
			GenDouble(minCoordinate, maxCoordinate)
		);
	}

	std::vector<std::vector<double> > distanceMatrix(targetsCnt + 1, std::vector<double>(targetsCnt + 1));
	for (size_t i = 0; i < distanceMatrix.size(); ++i) {
		for (size_t j = 0; j < distanceMatrix.size(); ++j) {
			std::pair<double, double> pointi = (!i ? std::make_pair(0, 0) : targets[i - 1]);
			std::pair<double, double> pointj = (!j ? std::make_pair(0, 0) : targets[j - 1]);
			distanceMatrix[i][j] = hypot(pointi.first - pointj.first, pointi.second - pointj.second);
		}
	}
	targets.push_back({ 0, 0 });
	rotate(targets.begin(), targets.begin() + targets.size() - 1, targets.end());
	return InputData(dronsCnt, targetsCnt, maxDist, distanceMatrix, targets);
}

int DataGenerator::GenInt(int l, int r) {
	std::uniform_int_distribution<int> uid(l, r);
	return uid(randGen);
}

double DataGenerator::GenDouble(double l, double r) {
	std::uniform_real_distribution<double> urd(l, r);
	return urd(randGen);
}

double DataGenerator::PowInt(double x, int alpha) {
	double ans = 1;
	for (int i = 0; i < alpha; ++i)
		ans *= x;
	return ans;
}
