#include "SolverClarkeWright.h"

ProblemSolution SolverClarkeWright::RunMinSum(InputData& input, ProblemMode problemMode, std::vector<double> args) {
	
	double startTime = (double)clock() / CLOCKS_PER_SEC;

	input.DronsCnt = std::min(input.DronsCnt, input.TargetsCnt);

	MatrixDouble savingMatrix(input.TargetsCnt, std::vector<double>(input.TargetsCnt, 0));

	std::vector<std::pair<double, std::pair<int, int> > > sortedSavings;
	sortedSavings.reserve(input.TargetsCnt * input.TargetsCnt);
	std::vector<std::vector<int> > edges(input.TargetsCnt); // graph of paths. Edges only between targets (not depo)
	std::vector<double> pathCurrentLength(input.TargetsCnt, 0); // pathCurLen[i] - length of path with component == i
	std::vector<int> component(input.TargetsCnt); // Number of component root. Root is first element in path
	std::vector<std::pair<int, int> > componentEdgesVerts(input.TargetsCnt); // Indices of vertices on the edges of paths

	for (int i = 0; i < input.TargetsCnt; ++i) {
		component[i] = i;
		componentEdgesVerts[i] = { i, i };
	}

	// Calculate savingMatrix and sorted list
	for (int i = 0; i < input.TargetsCnt; ++i) {
		for (int j = i + 1; j < input.TargetsCnt; ++j) {
			savingMatrix[i][j] = input.Distance(i + 1, 0) + input.Distance(0, j + 1) - input.Distance(i + 1, j + 1);
			sortedSavings.push_back({ savingMatrix[i][j], {i, j} });
		}
	}
	sort(sortedSavings.begin(), sortedSavings.end(), std::greater<std::pair<double, std::pair<int, int> > >());

	int numberOfPaths = input.TargetsCnt;

	for (auto& element : sortedSavings) {

		if (problemMode == ProblemMode::MINMAXLEN && numberOfPaths == input.DronsCnt) break;
		double dist;
		int v1, v2;
		std::tie(dist, std::tie(v1, v2)) = element;

		if (edges[v1].size() == 2 || edges[v2].size() == 2) continue;

		int compv1 = component[v1];
		int compv2 = component[v2];
		if (compv1 == compv2) continue;

		double newPathLen = input.Distance(v1 + 1, v2 + 1) + pathCurrentLength[compv1] + pathCurrentLength[compv2]; // without edges (depot, v1') and (depot, v2')
		// v1' and v2' - vertices on the other edges of paths of v1 and v2
		int v1prime = (componentEdgesVerts[compv1].first == v1 ? componentEdgesVerts[compv1].second : componentEdgesVerts[compv1].first);
		int v2prime = (componentEdgesVerts[compv2].first == v2 ? componentEdgesVerts[compv2].second : componentEdgesVerts[compv2].first);
		double newActualPathLen = newPathLen +
			input.Distance(0, 1 + v1prime) +
			input.Distance(0, 1 + v2prime);

		if (newActualPathLen > input.MaxDist)
			continue;

		edges[v1].push_back(v2);
		edges[v2].push_back(v1);
		component[v2] = component[v2prime] = component[v1] = component[v1prime];
		componentEdgesVerts[component[v1prime]] = { v1prime, v2prime };
		pathCurrentLength[component[v1prime]] = newPathLen;

		--numberOfPaths;
	}

	MatrixInt paths;
	std::vector<bool> used(input.TargetsCnt, false);
	for (int i = 0; i < input.TargetsCnt; ++i) {
		if (used[i] || edges[i].size() == 2) continue; // only first vertices in their paths.
		std::vector<int> path;
		int curV = i;
		while (true) {
			path.push_back(curV + 1);
			used[curV] = true;
			if (!edges[curV].size())
				break;
			int nxtV = edges[curV][0];
			if (used[nxtV] && edges[curV].size() == 2)
				nxtV = edges[curV][1];
			if (used[nxtV]) break;
			curV = nxtV;
		}
		paths.push_back(path);
	}

	return ProblemSolution(input, paths);
}

// Solution for minimizing maximum path length
ProblemSolution SolverClarkeWright::RunMinMaxLen(InputData& input, std::vector<double> args) {
	// Use binsearch to find minimum Path length, that exists a solution with sum minimizing
	double st = clock();

	double answerL = 0;
	double answerR = input.MaxDist;
	ProblemSolution bestSolution;
	bestSolution.SolutionExists = false;
	double startMaxDist = input.MaxDist;

	for (int cntIterations = 0; cntIterations < 50; ++cntIterations) {
		double answerM = (answerL + answerR) / 2;
		input.MaxDist = answerM;
		auto solution = RunMinSum(input, ProblemMode::MINMAXLEN, args);
		if (solution.SolutionExists) {
			bestSolution = solution;
			answerR = answerM;
		} else {
			answerL = answerM;
		}
	}

	input.MaxDist = startMaxDist;
	bestSolution.Input.MaxDist = startMaxDist;
	std::cout << ((double)clock()- st) / CLOCKS_PER_SEC << std::endl;

	return bestSolution;
}

ProblemSolution SolverClarkeWright::Run(InputData& input, ProblemMode problemMode, std::vector<double> args) {
	if (problemMode == ProblemMode::MINMAXLEN) {
		return RunMinMaxLen(input, args);
	} else {
		return RunMinSum(input, problemMode, args);
	}
}