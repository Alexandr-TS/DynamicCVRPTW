#include "SolverAntColony.h"
#include "Optimizations.h"

using namespace std;

double FindMaxPathLen(MatrixInt& paths, double** dist) {
	double ans = 0;
	if (!paths.size())
		return INF;
	for (auto& path : paths) {
		double curPathLen = dist[path.back()][0];
		for (int i = 0; i < (int)path.size() - 1; i++)
			curPathLen += dist[path[i]][path[i + 1]];
		ans = max(ans, curPathLen);
	}
	return ans;
}


// Args are: {alpha, betta, p, f, g, iterations coef, sigma, candList coef}
// Suggested args: {5, 5, 0.75, 2, 2, 2, 6, 0.25}
// Iterations coef - is a coefficent which will be multiplied by number of targets
// candList coef sets fraction of viewed neares vertices
ProblemSolution SolverAntColony::Run(InputData input, 
	ProblemMode problemMode, std::vector<double> args) {

	double startClock = clock();

	double timeLimit = args[8];

	// 1 for MINSUM and number of reducing steps for MINMAXLEN
	int globalIterations = 1;
	if (problemMode == ProblemMode::MINMAXLEN) {
		globalIterations = 20;
		if (timeLimit > EPS)
			globalIterations = INF;
	}


	// Coefficient powers for calculating probability of each edge
	int alpha = (int)args[0], betta = (int)args[1];
	// Trail persistence, (1-rho) - trail evaporation.
	double rho = args[2];
	// Coefficients for visibility function
	double etaF = (int)args[3];
	double etaG = (int)args[4];
	// Number of iterations
	int iterations = (int)(args[5] * input.TargetsCnt);
	if (problemMode == ProblemMode::MINMAXLEN)
		iterations /= 20;
	else if (timeLimit > EPS)
		iterations = INF;
	// Number of elitist ants
	int sigma = std::min((int)args[6], input.TargetsCnt);
	// Coefficient for candidate list. ListSize = n * candListCoef
	double candListCoef = args[7];

	// Number of vertices in graph
	int n = input.TargetsCnt + 1;

	// Matrix of pheromone trails
	double** tau = new double*[n];
	// Matrix of probabilities of edges
	double** p = new double*[n];
	// Matrix of visibility function
	double** eta = new double*[n];
	// Matrix of distances for fast access
	double** dist = new double*[n];
	// Matrix of nearest targets (without depot). {dist, u}
	pair<double, int>** candList = new pair<double, int>*[n];

	pair<double, int>* antPathLen = new pair<double, int>[n - 1];

	for (int i = 0; i < n; ++i) {
		tau[i] = new double[n];
		p[i] = new double[n];
		eta[i] = new double[n];
		dist[i] = new double[n];
		candList[i] = new pair<double, int>[n - 1];
		fill(tau[i], tau[i] + n, 1);
		fill(p[i], p[i] + n, 0);
		fill(eta[i], eta[i] + n, 0);
		fill(dist[i], dist[i] + n, 0);
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			dist[i][j] = input.Distance(i, j);
			if (j)
				candList[i][j - 1] = {dist[i][j], j};
		}
		sort(candList[i], candList[i] + n - 1);
	}

	if (*max_element(dist[0], dist[0] + n) * 2 > input.MaxDist)
		return ProblemSolution();

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			eta[i][j] = dist[i][0] + dist[0][j] - etaG * dist[i][j] + etaF * abs(dist[i][0] - dist[0][j]);
		}
	}

	MatrixInt bestSolution = { };
	double objectiveFBest = INF;

	bool* visited = new bool[n];
	double* objectiveF = new double[n];
	// During the algorithm the paths have format {{0, 1, 5}, {0, 2, 3, 4}} - 1st is always depot, all others - targets
	MatrixInt* paths = new MatrixInt[n];

	// Will be reduced if we optimize MINMAXLEN
	double solutionMaxPathLen = input.MaxDist;

	for (int globIter = 0; globIter < globalIterations; globIter++) {

		if (timeLimit > EPS && clock() - startClock > CLOCKS_PER_SEC * timeLimit)
			break;

		std::cout << "shuffle taus" << endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				tau[i][j] = DataGenerator::GenDouble(0.1, 1);
		}

		double bestSolutionMaxPath = 0;
		if (bestSolution.size() == 0) {
			solutionMaxPathLen = input.MaxDist;
		}
		else {
			bestSolutionMaxPath = FindMaxPathLen(bestSolution, dist);
			solutionMaxPathLen = min(solutionMaxPathLen, bestSolutionMaxPath - 1e-2);
			if (*max_element(dist[0], dist[0] + n) * 2 > solutionMaxPathLen)
				break;
		}

		for (int iteration = 0; iteration < iterations; ++iteration) {

			if (problemMode == ProblemMode::MINSUM) {
				if (iteration == iterations / 3 || iteration == iterations / 3 * 2) {
					std::cout << "shuffle taus" << endl;
					for (int i = 0; i < n; i++) {
						for (int j = 0; j < n; j++)
							tau[i][j] = DataGenerator::GenDouble(0.1, 1);
					}
				}
			}

			if (timeLimit > EPS && clock() - startClock > CLOCKS_PER_SEC * timeLimit)
				break;

			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					p[i][j] = DataGenerator::PowInt(tau[i][j], alpha) * DataGenerator::PowInt(eta[i][j], betta);
				}
			}

			for (int i = 0; i < n; ++i)
				paths[i].clear();

			fill(objectiveF, objectiveF + n, 0);
			for (int antIndex = 1; antIndex < n; ++antIndex) {
				fill(visited, visited + n, false);
				visited[antIndex] = true;
				visited[0] = true;
				double curPathLen = dist[0][antIndex];
				paths[antIndex] = { {0, antIndex} };
				int cntVisitedTargets = 1;
				int lastVisited = 0;
				while (cntVisitedTargets < n - 1) {
					if (cntVisitedTargets == lastVisited && paths[antIndex].back().back() != 0 || cntVisitedTargets < lastVisited) {
						assert(false);
					}
					lastVisited = cntVisitedTargets;
					int v = paths[antIndex].back().back();
					double probDenominator = 0;

					int checkListSz = max(1, min(n - 1, (int)(n * candListCoef)));
					int cntToCheck = (v == 0 ? n - 1 : checkListSz);

					for (int i = 0; i < cntToCheck; ++i) {
						int u = candList[v][i].second;
						if (!visited[u]) {
							probDenominator += p[v][u];
							if (probDenominator > EPS && !v && i >= cntToCheck)
								break;
						}
					}

					double tmp;
					if (probDenominator > EPS)
						tmp = DataGenerator::GenDouble(0, probDenominator);
					else
						tmp = 0;
					int nextV = -1;
					for (int i = 0; i < cntToCheck && nextV == -1; ++i) {
						int u = candList[v][i].second;
						if (!visited[u]) {
							tmp -= p[v][u];
							if (tmp < EPS) {
								nextV = u;
							}
						}
					}

					// if nextV is found and addidng edge (v, nextV) is feasible
					if (nextV != -1 && curPathLen + dist[v][nextV] + dist[nextV][0] <= solutionMaxPathLen) {
						visited[nextV] = true;
						curPathLen += dist[v][nextV];
						paths[antIndex].back().emplace_back(nextV);
						cntVisitedTargets++;
					}
					else {
						// path should contain not only depot. Its size must be at leat 2
						assert(paths[antIndex].back().size() > 1);
						curPathLen += dist[v][0];
						objectiveF[antIndex] += curPathLen;
						paths[antIndex].push_back({ 0 });
						curPathLen = 0;
					}
				}
				objectiveF[antIndex] += curPathLen;
			}

			for (int antIndex = 1; antIndex < n; antIndex++) {
				antPathLen[antIndex - 1] = { objectiveF[antIndex], antIndex };
			}
			sort(antPathLen, antPathLen + n - 1);

			if (problemMode == ProblemMode::MINMAXLEN) {
				vector<double> maxPaths(n);
				for (int i = 0; i < n - 1; i++)
					maxPaths[i] = FindMaxPathLen(paths[antPathLen[i].second], dist);
				for (int i = 0; i < n - 2; i++) {
					for (int j = 0; j < n - i - 2; j++) {
						int ind1 = antPathLen[j].second;
						int ind2 = antPathLen[j + 1].second;
						bool ok1 = (maxPaths[j] <= solutionMaxPathLen && (int)paths[ind1].size() <= input.DronsCnt);
						bool ok2 = (maxPaths[j + 1] <= solutionMaxPathLen && (int)paths[ind2].size() <= input.DronsCnt);
						if (ok1 ^ ok2) {
							if (ok2) {
								swap(antPathLen[j], antPathLen[j + 1]);
								swap(maxPaths[j], maxPaths[j + 1]);
							}
						}
						else {
							if (paths[ind1].size() > paths[ind2].size()) {
								swap(antPathLen[j], antPathLen[j + 1]);
								swap(maxPaths[j], maxPaths[j + 1]);
							} else if (paths[ind1].size() == paths[ind2].size() && maxPaths[j] > maxPaths[j + 1]) {
								swap(antPathLen[j], antPathLen[j + 1]);
								swap(maxPaths[j], maxPaths[j + 1]);
							}
						}
					}
				}
			}

			for (int i = 0; i < sigma; ++i) {
				antPathLen[i].first = 0;
				for (auto& path : paths[antPathLen[i].second]) {
					LocalReverseOptimization(path, input, true);
					for (int j = 0; j < (int)path.size() - 1; ++j)
						antPathLen[i].first += dist[path[j]][path[j + 1]];
					antPathLen[i].first += dist[path.back()][0];
				}
			}

			for (int i = 0; i < sigma; ++i) {
				StringCrossOptimization(paths[antPathLen[i].second], input, problemMode);
				antPathLen[i].first = 0;
				for (auto& path : paths[antPathLen[i].second]) {
					LocalReverseOptimization(path, input, true);
					for (int j = 0; j < (int)path.size() - 1; ++j)
						antPathLen[i].first += dist[path[j]][path[j + 1]];
					antPathLen[i].first += dist[path.back()][0];
				}
			}

			if (problemMode == ProblemMode::MINMAXLEN) {
				vector<double> maxPaths(n);
				for (int i = 0; i < n - 1; i++)
					maxPaths[i] = FindMaxPathLen(paths[antPathLen[i].second], dist);
				for (int i = 0; i < n - 2; i++) {
					for (int j = 0; j < n - i - 2; j++) {
						int ind1 = antPathLen[j].second;
						int ind2 = antPathLen[j + 1].second;
						bool ok1 = (maxPaths[j] <= solutionMaxPathLen && (int)paths[ind1].size() <= input.DronsCnt);
						bool ok2 = (maxPaths[j + 1] <= solutionMaxPathLen && (int)paths[ind2].size() <= input.DronsCnt);
						if (ok1 ^ ok2) {
							if (ok2) {
								swap(antPathLen[j], antPathLen[j + 1]);
								swap(maxPaths[j], maxPaths[j + 1]);
							}
						}
						else {
							if (paths[ind1].size() > paths[ind2].size()) {
								swap(antPathLen[j], antPathLen[j + 1]);
								swap(maxPaths[j], maxPaths[j + 1]);
							}
							else if (paths[ind1].size() == paths[ind2].size() && maxPaths[j] > maxPaths[j + 1]) {
								swap(antPathLen[j], antPathLen[j + 1]);
								swap(maxPaths[j], maxPaths[j + 1]);
							}
						}
					}
				}
			}
			else {
				sort(antPathLen, antPathLen + sigma);
			}
			
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					tau[i][j] *= rho;
				}
			}

			for (int mu = 1; mu < sigma; ++mu) {
				if (problemMode == ProblemMode::MINMAXLEN && paths[antPathLen[mu - 1].second].size() <= input.DronsCnt)
					continue;
				for (auto& path : paths[antPathLen[mu - 1].second]) {
					for (int i = 1; i < (int)path.size(); ++i) {
						tau[path[i - 1]][path[i]] += (sigma - mu) / antPathLen[mu - 1].first;
					}
				}
			}

			if ((antPathLen[0].first < objectiveFBest && problemMode == ProblemMode::MINSUM && (int)paths[antPathLen[0].second].size() <= input.DronsCnt) ||
				(problemMode == ProblemMode::MINMAXLEN && (int)paths[antPathLen[0].second].size() <= input.DronsCnt &&
					FindMaxPathLen(bestSolution, dist) > FindMaxPathLen(paths[antPathLen[0].second], dist))) {
				objectiveFBest = antPathLen[0].first;
				bestSolution = paths[antPathLen[0].second];
				if (problemMode == ProblemMode::MINMAXLEN) {
					solutionMaxPathLen = FindMaxPathLen(bestSolution, dist) - 0.01;
				}
			}

			if ((iteration % 10 == 0) && (int)paths[antPathLen[0].second].size() > input.DronsCnt)
				cout << "More drons: " << paths[antPathLen[0].second].size() << endl;
			if (iteration % 10 == 0)
				cout << FindMaxPathLen(paths[antPathLen[0].second], dist) << " " << paths[antPathLen[0].second].size() << endl;

			for (auto& path : bestSolution) {
				for (int i = 1; i < (int)path.size(); ++i) {
					tau[path[i - 1]][path[i]] += sigma / objectiveFBest;
				}
			}

			if (*max_element(dist[0], dist[0] + n) * 2 > solutionMaxPathLen)
				break;

			if (iteration % 10 == 0) {
				if (problemMode == ProblemMode::MINMAXLEN)
					cout << iteration << " " << fixed << setprecision(6) << FindMaxPathLen(bestSolution, dist) << " " << solutionMaxPathLen << endl;
				else
					cout << iteration << " " << fixed << setprecision(6) << objectiveFBest << " " << solutionMaxPathLen << endl;
			}
		}
	}

	for (auto& path : bestSolution) {
		path.erase(path.begin());
	}

	delete[] visited;
	delete[] objectiveF;
	delete[] paths;
	delete[] tau;
	delete[] p;
	delete[] eta;
	delete[] dist;
	delete[] candList;
	delete[] antPathLen;

	return ProblemSolution(input, bestSolution);
}
