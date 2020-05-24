#include "SolverAntColony.h"
#include "Optimizations.h"

using namespace std;

// Args are: {alpha, betta, p, f, g, iterations coef, sigma, cand_list coef}
// Suggested args: { 3, 4, 0.4, 2, 3, 2, 5, 0.5, 0 },
// Iterations coef - is a coefficent which will be multiplied by number of targets
// cand_list coef sets fraction of viewed neares vertices
ProblemSolution SolverAntColony::Run(InputData input, vector<double> args) {
	double start_clock = clock();

	double time_limit = args[8];

	// Coefficient powers for calculating probability of each edge
	int alpha = static_cast<int>(args[0]);
	int betta = static_cast<int>(args[1]);
	// Trail persistence, (1-rho) - trail evaporation.
	double rho = args[2];
	// Coefficients for visibility function
	double eta_f = static_cast<int>(args[3]);
	double eta_g = static_cast<int>(args[4]);
	// Number of iterations
	int iterations = static_cast<int>(args[5] * input.TargetsCnt);
	if (time_limit > EPS) {
		iterations = INF;
	}
	// Number of elitist ants
	int sigma = min(static_cast<int>(args[6]), input.TargetsCnt);
	// Coefficient for candidate list. ListSize = n * cand_list_coef
	double cand_list_coef = args[7];

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
	pair<double, int>** cand_list = new pair<double, int>*[n];

	pair<double, int>* ant_path_len = new pair<double, int>[n - 1];

	for (int i = 0; i < n; ++i) {
		tau[i] = new double[n];
		p[i] = new double[n];
		eta[i] = new double[n];
		dist[i] = new double[n];
		time_reserves[i] = new double[n];
		cand_list[i] = new pair<double, int>[n - 1];
		fill(tau[i], tau[i] + n, 1);
		fill(p[i], p[i] + n, 0);
		fill(eta[i], eta[i] + n, 0);
		fill(dist[i], dist[i] + n, 0);
		fill(time_reserves[i], time_reserves[i] + n, 0);
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			double cur_dist = input.Distance(i, j);
			time_reserves[i][j] = input.TimeWindows[j].second - (input.TimeWindows[i].first + cur_dist);
			if (!i && input.TimeWindows[j].second < cur_dist) {
				cout << "ACO. Vertex #" << j << " is not attainable from depot because of its time window" << endl;
				return ProblemSolution();
			}
			dist[i][j] = (time_reserves[i][j] < 0 ? INF : cur_dist);
			if (j) {
				cand_list[i][j - 1] = { dist[i][j], j };
			}
		}
		sort(cand_list[i], cand_list[i] + n - 1);
	}

	if (*max_element(dist[0], dist[0] + n) * 2 > input.MaxDist) {
		cout << "ACO. There is an element no attainable because of its distance and MaxDronDistance" << endl;
		return ProblemSolution();
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			// if dist[i][j] == INF (edge i-j is invalid) then eta[i][j] = 0
			eta[i][j] = max(0., dist[i][0] + dist[0][j] - 
				eta_g * dist[i][j] + eta_f * abs(dist[i][0] - dist[0][j]));
		}
	}

	MatrixInt best_solution = { };
	double objective_f_best = INF;

	bool* visited = new bool[n];
	double* objective_f = new double[n];
	// During the algorithm the paths have format {{0, 1, 5}, {0, 2, 3, 4}} - 1st is always depot, all others - targets
	MatrixInt* paths = new MatrixInt[n];

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			tau[i][j] = Math::GenDouble(0.1, 1);
		}
	}

	for (int iteration = 0; iteration < iterations; ++iteration) {
		if (input.TargetsCnt < iterations && 
			(iteration == iterations / 3 || iteration == iterations / 3 * 2)) {
			cout << "ACO. Shuffle pheromones" << endl;
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					tau[i][j] = Math::GenDouble(0.1, 1);
				}
			}
		}

		if (time_limit > EPS && clock() - start_clock > CLOCKS_PER_SEC * time_limit) {
			cout << "ACO. Time limit finished. break" << endl;
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

		fill(objective_f, objective_f + n, 0);
		for (int ant_index = 1; ant_index < n; ++ant_index) {
			fill(visited, visited + n, false);
			visited[ant_index] = true;
			visited[0] = true;
			double cur_path_len = dist[0][ant_index];
			paths[ant_index] = { {0, ant_index} };
			double cur_time = max(dist[0][ant_index], input.TimeWindows[ant_index].first);
			int cnt_visited_targets = 1;
			int lastVisited = 0;
			while (cnt_visited_targets < n - 1) {
				if (cnt_visited_targets == lastVisited && 
					paths[ant_index].back().back() != 0 || cnt_visited_targets < lastVisited) {
					assert(false);
				}
				lastVisited = cnt_visited_targets;
				int v = paths[ant_index].back().back();
				double prob_denominator = 0;

				int check_list_sz = max(1, min(n - 1, static_cast<int>(n * cand_list_coef)));
				int cnt_to_check = (v == 0 ? n - 1 : check_list_sz);

				for (int i = 0; i < cnt_to_check; ++i) {
					int u = cand_list[v][i].second;
					if (!visited[u] && dist[v][i] < INF - 1) {
						prob_denominator += p[v][u];
						if (prob_denominator > EPS && !v && i >= cnt_to_check) {
							break;
						}
					}
				}

				double tmp;
				if (prob_denominator > EPS) {
					tmp = Math::GenDouble(0, prob_denominator);
				}
				else {
					tmp = 0;
				}
				int next_v = -1;
				for (int i = 0; i < cnt_to_check && next_v == -1; ++i) {
					int u = cand_list[v][i].second;
					if (!visited[u]) {
						tmp -= p[v][u];
						if (tmp < EPS) {
							next_v = u;
						}
					}
				}

				// if next_v is found and addidng edge (v, next_v) is feasible
				if (next_v != -1 && cur_path_len + dist[v][next_v] + dist[next_v][0] <= input.MaxDist && 
						max(cur_time, input.TimeWindows[v].first) + 
						dist[v][next_v] <= input.TimeWindows[next_v].second) {
					visited[next_v] = true;
					cur_path_len += dist[v][next_v];
					paths[ant_index].back().push_back(next_v);
					++cnt_visited_targets;
					cur_time = max(cur_time + dist[v][next_v], input.TimeWindows[next_v].first);
				}
				else {
					// path should contain not only depot. Its size must be at least 2
					if (paths[ant_index].back().size() <= 1) {
						assert(false);
					}
					assert(paths[ant_index].back().size() > 1);
					cur_path_len += dist[v][0];
					objective_f[ant_index] += cur_path_len;
					paths[ant_index].push_back({ 0 });
					cur_path_len = 0;
					cur_time = 0;
				}
			}
			cur_path_len += dist[paths[ant_index].back().back()][0];
			objective_f[ant_index] += cur_path_len;
		}

		for (int ant_index = 1; ant_index < n; ++ant_index) {
			ant_path_len[ant_index - 1] = { objective_f[ant_index], ant_index };
		}
		sort(ant_path_len, ant_path_len + n - 1);

		for (int i = 0; i < sigma; ++i) {
			ant_path_len[i].first = 0;
			for (auto& path : paths[ant_path_len[i].second]) {
				LocalSwapOptimization(path, input);
				for (int j = 0; j < static_cast<int>(path.size()) - 1; ++j) {
					// After optimization a valid path should become invalid
					assert(dist[path[j]][path[j + 1]] < INF - 1); 
					ant_path_len[i].first += dist[path[j]][path[j + 1]];
				}
				ant_path_len[i].first += dist[path.back()][0];
			}
		}

		for (int i = 0; i < sigma; ++i) {
			GlobalSwapOptimization(paths[ant_path_len[i].second], input);
			GlobalInsertOptimization(paths[ant_path_len[i].second], input);
			ant_path_len[i].first = 0;
			for (auto& path : paths[ant_path_len[i].second]) {
				LocalSwapOptimization(path, input);
				for (int j = 0; j < static_cast<int>(path.size()) - 1; ++j) {
					// After optimization a valid path should become invalid
					assert(dist[path[j]][path[j + 1]] < INF - 1); 
					ant_path_len[i].first += dist[path[j]][path[j + 1]];
				}
				ant_path_len[i].first += dist[path.back()][0];
			}
		}

		sort(ant_path_len, ant_path_len + sigma);
		
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				tau[i][j] *= rho;
			}
		}

		for (int mu = 1; mu < sigma; ++mu) {
			for (auto& path : paths[ant_path_len[mu - 1].second]) {
				for (int i = 1; i < static_cast<int>(path.size()); ++i) {
					tau[path[i - 1]][path[i]] += (sigma - mu) / ant_path_len[mu - 1].first;
				}
			}
		}

		if (ant_path_len[0].first < objective_f_best && 
			static_cast<int>(paths[ant_path_len[0].second].size()) <= input.DronsCnt) {
			objective_f_best = ant_path_len[0].first;
			best_solution = paths[ant_path_len[0].second];
		}

		for (auto& path : best_solution) {
			for (int i = 1; i < static_cast<int>(path.size()); ++i) {
				tau[path[i - 1]][path[i]] += sigma / objective_f_best;
			}
		}

		if (iteration % 20 == 0 && time_limit < EPS) {
			cout << "ACO. Iteration: " << iteration << "/" << iterations << 
				". Best found value: " << fixed << setprecision(6) << objective_f_best << " " << endl;
		}
		else if (iteration % 20 == 0) {
			cout << "ACO. Iterations done: " << iteration << ". Best found value: " 
				<< fixed << setprecision(6) << objective_f_best << " " << endl;
		}
	}

	for (auto& path : best_solution) {
		assert(path[0] == 0);
		path.erase(path.begin());
	}

	delete[] visited;
	delete[] objective_f;
	delete[] paths;
	delete[] tau;
	delete[] p;
	delete[] eta;
	delete[] dist;
	delete[] cand_list;
	delete[] ant_path_len;

	return ProblemSolution(input, best_solution);
}

