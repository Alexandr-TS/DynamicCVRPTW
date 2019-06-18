#include "SolverGreedy.h"

ProblemSolution SolverGreedy::Run(InputData input, ProblemMode problemMode, std::vector<double> args) {
	double startTime = (double)clock() / CLOCKS_PER_SEC;

	input.DronsCnt = std::min(input.DronsCnt, input.TargetsCnt);

	std::vector<int> pointIndexOfDron(input.DronsCnt, 0);
	std::vector<double> doneDistance(input.DronsCnt, 0);
	std::vector<int> usedTargets(input.TargetsCnt, false);

	MatrixInt paths(input.DronsCnt);

	for (int iterationNum = 0; iterationNum < input.TargetsCnt; ++iterationNum) {
		int dronIndex = -1;
		int targetIndex = -1;
		double minDistance = std::numeric_limits<double>::max();

		for (int i = 0; i < input.DronsCnt; ++i) {
			int curInd = pointIndexOfDron[i];
			for (int j = 1; j < 1 + input.TargetsCnt; ++j) {
				if (usedTargets[j - 1]) {
					continue;
				}
				if (doneDistance[i] + input.DistanceMatrix[curInd][j] + input.DistanceMatrix[j][0] >
					input.MaxDist + EPS) {
					continue;
				}
				if (problemMode == ProblemMode::MINMAXLEN) {
					if (minDistance > input.DistanceMatrix[curInd][j]) {
						minDistance = input.DistanceMatrix[curInd][j];
						dronIndex = i;
						targetIndex = j;
					}
				} else if (problemMode == ProblemMode::MINSUM) {
					if (minDistance > input.DistanceMatrix[curInd][j] + doneDistance[i]) {
						minDistance = input.DistanceMatrix[curInd][j] + doneDistance[i];
						dronIndex = i;
						targetIndex = j;
					}
				}
			}
		}

		if (dronIndex == -1 || targetIndex == -1) {
			assert(dronIndex == targetIndex);
			return ProblemSolution(input, paths);
		} else {
			usedTargets[targetIndex - 1] = true;
			paths[dronIndex].push_back(targetIndex);
			doneDistance[dronIndex] += input.DistanceMatrix[pointIndexOfDron[dronIndex]][targetIndex];
			pointIndexOfDron[dronIndex] = targetIndex;
		}
	}

	return ProblemSolution(input, paths);
}