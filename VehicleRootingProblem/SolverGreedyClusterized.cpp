#include "SolverGreedyClusterized.h"

// returns 1-indexed vector<vector<int>> with targets indexes splitted into classes
MatrixInt GreedyClusterization(InputData& input, ProblemMode problemMode, int countDrons) {
	assert(countDrons <= input.TargetsCnt);

	MatrixInt classes; // 0-indexed targets
	srand(0);
	int firstNum = rand() % input.TargetsCnt;

	// finding basis - greedy countDrons drons with max pairwaise distances
	std::vector<bool> used(input.TargetsCnt, false);
	used[firstNum] = true;
	classes.push_back({firstNum});

	for (int i = 1; i < countDrons; ++i) {
		int bestIndex = -1;
		double maxMinDistance = -INF;
		for (int j = 0; j < input.TargetsCnt; ++j) {
			if (!used[j]) {
				double curMinDistance = INF;
				for (auto& line: classes) {
					curMinDistance = std::min(curMinDistance, input.Distance(j + 1, line[0] + 1));
				}
				if (curMinDistance > maxMinDistance) {
					maxMinDistance = curMinDistance;
					bestIndex = j;
				}
			}
		}
		assert(bestIndex != -1);
		used[bestIndex] = true;
		classes.push_back({bestIndex});
	}

	// finding best classes for remaining vertices.
	for (int i = 0; i < input.TargetsCnt; ++i) {
		if (used[i]) continue;
		used[i] = true;
		int minClassSize = classes[0].size();
		for (auto& line: classes) {
			minClassSize = std::min(minClassSize, (int)line.size());
		}

		int bestClassIndex = -1;
		double minDistance = INF;
		for (size_t j = 0; j < classes.size(); ++j) {
			if ((int)classes[j].size() > minClassSize) continue;
			double curMinDistance = INF;
			for (auto targetIndex: classes[j]) {
				curMinDistance = std::min(curMinDistance, input.Distance(i + 1, targetIndex + 1));
			}
			if (curMinDistance < minDistance) {
				minDistance = curMinDistance;
				bestClassIndex = j;
			}
		}
		assert(bestClassIndex != -1);
		classes[bestClassIndex].push_back(i);
	}

	for (auto& line: classes)
		for (auto& ind: line)
			ind++;
	assert(classes.size() == (size_t)countDrons);

	return classes;
}

void dfs(int v, int parent, std::vector<int>& eulerPath, std::vector<std::vector<int> >& g) {
	eulerPath.push_back(v);
	for (auto u: g[v])
		if (u != parent) {
			dfs(u, v, eulerPath, g);
		}
}

// returns "best" 1-indexed path for salesman problem. Starting and finishing in depot. line - targets from 1 to TargetCnt
std::vector<int> SpanningSalesman(InputData& input, ProblemMode problemMode, std::vector<int>& line) {
	int n = line.size();

	std::vector<int> componentNumber(n, 0);
	for (int i = 0; i < n; i++)
		componentNumber[i] = i;

	std::set<std::pair<double, std::pair<int, int> > > edges; // {dist, {v1, v2}}
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			edges.insert({input.Distance(line[i], line[j]), {i, j}});

	std::vector<std::vector<int> > g(n);

	// building spanning tree
	int edgesLeft = n - 1;
	while (edgesLeft) {
		while (edges.size()) {
			int v1 = edges.begin()->second.first;
			int v2 = edges.begin()->second.second;
			if (componentNumber[v1] == componentNumber[v2]) {
				edges.erase(edges.begin());
			} else {
				int componentNumberV2 = componentNumber[v2];
				for (int i = 0; i < n; ++i)
					if (componentNumber[i] == componentNumberV2)
						componentNumber[i] = componentNumber[v1];
				g[v1].push_back(v2);
				g[v2].push_back(v1);
				edgesLeft--;
				break;
			}
		}
		assert(edges.size() || !edgesLeft);
	}

	int cur = 0; // find nearest target to the depot.
	for (int i = 1; i < n; ++i)
		if (input.Distance(0, line[i]) < input.Distance(0, line[cur])) {
			cur = i;
		}

	std::vector<int> eulerPath = {};
	dfs(cur, -1, eulerPath, g);

	std::vector<int> bestPath = eulerPath;

	for (auto& v: bestPath) {
		v = line[v];
	}

	return bestPath;
}

ProblemSolution SolverGreedyClusterized::Run(InputData input, ProblemMode problemMode, std::vector<double> args) {
	double startTime = (double)clock() / CLOCKS_PER_SEC;
    input.DronsCnt = std::min(input.DronsCnt, input.TargetsCnt);

	// stores paths indexed from 1. Like 5 2 1 3. Without 0 (depot).
	MatrixInt answerPaths;
	double answerValue = INF;

	for (int dronsCnt = 1; dronsCnt <= input.DronsCnt; ++dronsCnt) {
		auto classes = GreedyClusterization(input, problemMode, dronsCnt);
		MatrixInt paths;
		for (auto& line : classes) {
			paths.push_back(SpanningSalesman(input, problemMode, line));
		}
		auto solution = ProblemSolution(input, paths);
		double curValue;
		if (problemMode == ProblemMode::MINMAXLEN) {
			curValue = solution.MaxPathLength;
		} else {
			curValue = solution.SumOfPathLengths;
		}
		
		if (solution.SolutionExists && curValue < answerValue) {
			answerValue = curValue;
			answerPaths = paths;
		}
	}

	return ProblemSolution(input, answerPaths);
}

