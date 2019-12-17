#include "SolverAntColony.h"
#include "Optimizations.h"

using namespace std;

// Args are: {alpha, betta, p, f, g, iterations coef, sigma, candList coef}
// Suggested args: {5, 5, 0.75, 2, 2, 2, 6, 0.25}
// Iterations coef - is a coefficent which will be multiplied by number of targets
// candList coef sets fraction of viewed neares vertices
ProblemSolution SolverAntColony::Run(InputData input, std::vector<double> args) {
	double startClock = clock();

	double timeLimit = args[8];

	// Coefficient powers for calculating probability of each edge
	int alpha = (int)args[0], betta = (int)args[1];
	// Trail persistence, (1-rho) - trail evaporation.
	double rho = args[2];
	// Coefficients for visibility function
	double etaF = (int)args[3];
	double etaG = (int)args[4];
	// Number of iterations
	int iterations = (int)(args[5] * input.TargetsCnt);
	if (timeLimit > EPS)
		iterations = INF;
	// Number of elitist ants
	int sigma = std::min((int)args[6], input.TargetsCnt);
	// Coefficient for candidate list. ListSize = n * candListCoef
	double candListCoef = args[7];

	// Coefficent for reserve value in visibility function (eta)
	double reserve_coef = 3;

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
	// Matrix of time reserves	
	double** time_reserves = new double*[n];
	// Matrix of nearest targets (without depot). {dist, u}
	pair<double, int>** candList = new pair<double, int>*[n];

	pair<double, int>* antPathLen = new pair<double, int>[n - 1];

	for (int i = 0; i < n; ++i) {
		tau[i] = new double[n];
		p[i] = new double[n];
		eta[i] = new double[n];
		dist[i] = new double[n];
		time_reserves[i] = new double[n];
		candList[i] = new pair<double, int>[n - 1];
		fill(tau[i], tau[i] + n, 1);
		fill(p[i], p[i] + n, 0);
		fill(eta[i], eta[i] + n, 0);
		fill(dist[i], dist[i] + n, 0);
		fill(time_reserves[i], time_reserves[i] + n, 0);
	}

	double tmp_max_dist = 0, tmp_max_time_reserve = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			double cur_dist = input.Distance(i, j);
			time_reserves[i][j] = input.TimeWindows[j].second - (input.TimeWindows[i].first + cur_dist);
			tmp_max_time_reserve = max(tmp_max_time_reserve, time_reserves[i][j]);
			if (!i && time_reserves[i][j] < 0) {
				cout << "Vertex #" << j << " is not attainable from depot because of its time window" << endl;
				return ProblemSolution();
			}
			dist[i][j] = (time_reserves[i][j] < 0 ? INF : cur_dist);
			tmp_max_dist = max(tmp_max_dist, cur_dist);
			if (j) {
				candList[i][j - 1] = { dist[i][j], j };
			}
		}
		sort(candList[i], candList[i] + n - 1);
	}

	if (*max_element(dist[0], dist[0] + n) * 2 > input.MaxDist) {
		cout << "There is an element no attainable because of its distance and MaxDronDistance" << endl;
		return ProblemSolution();
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			// if dist[i][j] == INF (edge i-j is invalid) then eta[i][j] = 0
			eta[i][j] = max(0., dist[i][0] + dist[0][j] - etaG * dist[i][j] + etaF * abs(dist[i][0] - dist[0][j]) +
				reserve_coef * min(1.0, (tmp_max_dist / tmp_max_time_reserve)) * time_reserves[i][j]);
		}
	}

	MatrixInt bestSolution = { };
	double objectiveFBest = INF;

	bool* visited = new bool[n];
	double* objectiveF = new double[n];
	// During the algorithm the paths have format {{0, 1, 5}, {0, 2, 3, 4}} - 1st is always depot, all others - targets
	MatrixInt* paths = new MatrixInt[n];

	std::cout << "shuffle taus" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tau[i][j] = Math::GenDouble(0.1, 1);
		}
	}

	double bestSolutionMaxPath = 0;

	for (int iteration = 0; iteration < iterations; ++iteration) {
		if (iteration == iterations / 3 || iteration == iterations / 3 * 2) {
			std::cout << "shuffle taus" << endl;
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					tau[i][j] = Math::GenDouble(0.1, 1);
				}
			}
		}

		if (timeLimit > EPS && clock() - startClock > CLOCKS_PER_SEC * timeLimit) {
			std::cout << "time limit finished. break" << endl;
			break;
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				p[i][j] = Math::PowInt(tau[i][j], alpha) * Math::PowInt(eta[i][j], betta);
			}
		}

		for (int i = 0; i < n; ++i) {
			paths[i].clear();
		}

		fill(objectiveF, objectiveF + n, 0);
		for (int antIndex = 1; antIndex < n; ++antIndex) {
			double cur_time = 0;

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
					if (!visited[u] && dist[v][i] < INF - 1) {
						probDenominator += p[v][u];
						if (probDenominator > EPS && !v && i >= cntToCheck) {
							break;
						}
					}
				}

				double tmp;
				if (probDenominator > EPS)
					tmp = Math::GenDouble(0, probDenominator);
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
				if (nextV != -1 && curPathLen + dist[v][nextV] + dist[nextV][0] <= input.MaxDist && 
						max(cur_time, input.TimeWindows[v].first) + dist[v][nextV] <= input.TimeWindows[nextV].second) {
					visited[nextV] = true;
					curPathLen += dist[v][nextV];
					paths[antIndex].back().emplace_back(nextV);
					cntVisitedTargets++;
					cur_time = max(cur_time + dist[v][nextV], input.TimeWindows[nextV].first);
				}
				else {
					// path should contain not only depot. Its size must be at least 2
					if (paths[antIndex].back().size() <= 1) {
						assert(false);
					}
					assert(paths[antIndex].back().size() > 1);
					curPathLen += dist[v][0];
					objectiveF[antIndex] += curPathLen;
					paths[antIndex].push_back({ 0 });
					curPathLen = 0;
					cur_time = 0;
				}
			}
			curPathLen += dist[paths[antIndex].back().back()][0];
			objectiveF[antIndex] += curPathLen;
		}

		for (int antIndex = 1; antIndex < n; antIndex++) {
			antPathLen[antIndex - 1] = { objectiveF[antIndex], antIndex };
		}
		sort(antPathLen, antPathLen + n - 1);

		for (int i = 0; i < sigma; ++i) {
			antPathLen[i].first = 0;
			for (auto& path : paths[antPathLen[i].second]) {
				LocalSwapOptimization(path, input);
				for (int j = 0; j < (int)path.size() - 1; ++j) {
					assert(dist[path[j]][path[j + 1]] < INF - 1); // After optimization a valid path should become invalid
					antPathLen[i].first += dist[path[j]][path[j + 1]];
				}
				antPathLen[i].first += dist[path.back()][0];
			}
		}

		for (int i = 0; i < sigma; ++i) {
			GlobalSwapOptimization(paths[antPathLen[i].second], input);
			GlobalInsertOptimization(paths[antPathLen[i].second], input);
			antPathLen[i].first = 0;
			for (auto& path : paths[antPathLen[i].second]) {
				LocalSwapOptimization(path, input);
				for (int j = 0; j < (int)path.size() - 1; ++j) {
					assert(dist[path[j]][path[j + 1]] < INF - 1); // After optimization a valid path should become invalid
					antPathLen[i].first += dist[path[j]][path[j + 1]];
				}
				antPathLen[i].first += dist[path.back()][0];
			}
		}

		sort(antPathLen, antPathLen + sigma);
		
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				tau[i][j] *= rho;
			}
		}

		for (int mu = 1; mu < sigma; ++mu) {
			for (auto& path : paths[antPathLen[mu - 1].second]) {
				for (int i = 1; i < (int)path.size(); ++i) {
					tau[path[i - 1]][path[i]] += (sigma - mu) / antPathLen[mu - 1].first;
				}
			}
		}

		if (antPathLen[0].first < objectiveFBest && (int)paths[antPathLen[0].second].size() <= input.DronsCnt) {
			objectiveFBest = antPathLen[0].first;
			bestSolution = paths[antPathLen[0].second];
		}

		if ((iteration % 10 == 0) && (int)paths[antPathLen[0].second].size() > input.DronsCnt) {
			cout << "More drons: " << paths[antPathLen[0].second].size() << endl;
		}
		if (iteration % 10 == 0) {
			cout << "Best objective: " << objectiveFBest << ",  Current best path: " << antPathLen[0].first << 
				",  Current cnt of paths: " << paths[antPathLen[0].second].size() << endl;
		}

		for (auto& path : bestSolution) {
			for (int i = 1; i < (int)path.size(); ++i) {
				tau[path[i - 1]][path[i]] += sigma / objectiveFBest;
			}
		}

		if (*max_element(dist[0], dist[0] + n) * 2 > input.MaxDist) {
			break;
		}

		if (iteration % 10 == 0) {
			cout << iteration << ", best: " << fixed << setprecision(6) << objectiveFBest << " " << endl;
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

