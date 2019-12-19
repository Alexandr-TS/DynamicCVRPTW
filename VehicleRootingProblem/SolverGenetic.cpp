#include "SolverGenetic.h"
#include "SolverAntColony.h"
#include "SolverGreedy.h"
#include "Optimizations.h"

struct Chromosome {
	std::vector<int> Seq;
	double Fitness;

	Chromosome() {}
	Chromosome(std::vector<int>& seq, double fitness) :
		Seq(seq),
		Fitness(fitness)
	{}

	static double CalcFitness(std::vector<int>& seq, InputData& input) {
		if (seq.size() != input.TargetsCnt) {
			return 0;
		}
		seq.insert(seq.begin(), { 0 });
		int n = seq.size();

		double* prefLen = new double[n];
		std::fill(prefLen, prefLen + n, 0);

		for (int i = 1; i < n; i++) {
			prefLen[i] = prefLen[i - 1] + input.Distance(seq[i - 1], seq[i]);
		}

		double* dp = new double[n];
		std::fill(dp, dp + n, INF);

		dp[0] = 0;
		for (int v = 0; v + 1 < n; ++v) {
			if (dp[v] == INF) continue;
			double len = 0;
			double cur_time = std::max(input.Distance(0, seq[v + 1]), input.TimeWindows[seq[v + 1]].first);
			for (int u = v + 1; u < n && len <= input.MaxDist + EPS; ++u) {
				// relaxing using edge (v, u)
				len = prefLen[u] - prefLen[v + 1] + input.Distance(0, seq[v + 1]) + input.Distance(seq[u], 0);
				if (u > v + 1) {
					cur_time = std::max(input.TimeWindows[seq[u]].first, cur_time + prefLen[u] - prefLen[u - 1]);
				}
				if (input.TimeWindows[seq[u]].second >= cur_time && len <= input.MaxDist + EPS) {
					dp[u] = std::min(dp[u], dp[v] + len);
				}
				else {
					break;
				}
			}
		}

		delete[] prefLen;
		auto answer = dp[n - 1];
		delete[] dp;
		seq.erase(seq.begin());
		return answer;
	}

	Chromosome(std::vector<int>& seq, InputData& input) : Seq(seq) {
		Fitness = CalcFitness(seq, input);
	}

	Chromosome(MatrixInt& paths, InputData& input) {
		for (auto& path : paths) {
			for (auto x : path) {
				Seq.push_back(x);
			}
		}
		if (Seq.size() != input.TargetsCnt) {
			Fitness = 0;
		}
		else {
			Fitness = CalcFitness(Seq, input);
		}
	}

	Chromosome(ProblemSolution& solution) {
		for (auto& path : solution.Paths) {
			for (auto x : path) {
				Seq.push_back(x);
			}
		}
		if (Seq.size() != solution.Input.TargetsCnt) {
			Fitness = 0;
		}
		else {
			Fitness = CalcFitness(Seq, solution.Input);
		}
	}

	bool IsValid() {
		return *std::max_element(Seq.begin(), Seq.end()) == Seq.size() && Fitness > 0;
	}
};

std::mt19937 tmpGen = std::mt19937(1);

Chromosome GenRandomChromosome(int n, InputData& input) {
	std::vector<int> seq(n);
	for (int i = 1; i <= n; i++) {
		seq[i - 1] = i;
	}
	std::shuffle(seq.begin(), seq.end(), tmpGen);
	return Chromosome(seq, input);
}

struct Population {
	std::vector<Chromosome> Chromosomes;
	std::map<int, int> FitByDelta;
	double Delta;

	Population(double delta) : Delta(delta) {
		Chromosomes = {};
		FitByDelta = {};
	}

	bool Add(Chromosome x) {
		if (Delta == 0) {
			Chromosomes.push_back(x);
			return true;
		}
		if (FitByDelta.count((int)(x.Fitness / Delta)) && FitByDelta[(int)(x.Fitness / Delta)] > 0) {
			return false;
		}
		if (Delta) {
			FitByDelta[(int)(x.Fitness / Delta)]++;
		}
		Chromosomes.push_back(x);
		for (int i = (int)Chromosomes.size() - 1; i >= 1; i--) {
			if (Chromosomes[i].Fitness < Chromosomes[i - 1].Fitness) {
				std::swap(Chromosomes[i], Chromosomes[i - 1]);
			}
		}
		return true;
	}

	bool IsAddible(Chromosome& x) {
		return (Delta == 0 || !FitByDelta.count((int)(x.Fitness / Delta)) || FitByDelta[(int)(x.Fitness / Delta)] == 0);
	}

	void Del(Chromosome& x) {
		for (auto it = Chromosomes.begin(); it != Chromosomes.end(); ++it) {
			if (it->Seq == x.Seq) {
				if (Delta) {
					int arg = (int)(x.Fitness / Delta);
					FitByDelta[arg]--;
					if (FitByDelta[arg] == 0) {
						FitByDelta.erase(arg);
					}
				}

				Chromosomes.erase(it);
				break;
			}
		}
	}

	void Del(int index) {
		auto it = Chromosomes.begin() + index;
		if (Delta) {
			int arg = (int)(it->Fitness / Delta);
			FitByDelta[arg]--;
			if (FitByDelta[arg] == 0) {
				FitByDelta.erase(arg);
			}
		}
		Chromosomes.erase(it);
	}

	int Size() {
		return Chromosomes.size();
	}
};

const int N = 201;

bool used[N];
double prefLen[N];
double dp[N];
int prev[N];

Chromosome Crossover(Chromosome& parent1, Chromosome& parent2, InputData& input) {
	auto par1 = &parent1;
	auto par2 = &parent2;
	if (Math::GenInt(0, 1)) {
		std::swap(par1, par2);
	}
	int n = par1->Seq.size();
	int tl = Math::GenInt(0, n - 1);
	int tr = Math::GenInt(0, n - 1);

	if (Math::GenInt(0, 1)) {
		tl = 0;
	}
	else {
		tr = n - 1;
	}

	if (tr < tl) {
		std::swap(tr, tl);
	}
	
	std::fill(used, used + n + 1, false);
	std::vector<int> child(n, 0);

	for (int i = tl; i <= tr; ++i) {
		child[i] = par1->Seq[i];
		used[par1->Seq[i]] = true;
	}

	int curChildInd = (tr + 1) % n;
	for (int i1 = (tr + 1); i1 < tr + n + 1; i1++) {
		int i = i1;
		while (i >= n) {
			i -= n;
		}
		if (!used[par2->Seq[i]]) {
			used[par2->Seq[i]] = true;
			child[curChildInd] = par2->Seq[i];
			curChildInd++;
			if (curChildInd >= n) {
				curChildInd -= n;
			}
		}
	}

	return Chromosome(child, input);
}

MatrixInt SplitPaths(std::vector<int> path, InputData& input) {
	if (path.empty()) {
		return {};
	}
	path.insert(path.begin(), { 0 });
	int n = path.size();

	std::fill(prefLen, prefLen + n, 0);
	for (int i = 1; i < n; i++) {
		prefLen[i] = prefLen[i - 1] + input.Distance(path[i - 1], path[i]);
	}

	std::fill(dp, dp + n, INF);
	std::fill(prev, prev + n, -1);
	dp[0] = 0;
	
	for (int v = 0; v + 1 < n; ++v) {
		if (dp[v] == INF) {
			continue;
		}
		double len = 0;
		double cur_time = std::max(input.Distance(0, path[v + 1]), input.TimeWindows[path[v + 1]].first);
		for (int u = v + 1; u < n && len <= input.MaxDist + EPS; ++u) {
			// relaxing using edge (v, u)
			len = prefLen[u] - prefLen[v + 1] + input.Distance(0, path[v + 1]) + input.Distance(path[u], 0);
			if (u > v + 1) {
				cur_time = std::max(input.TimeWindows[path[u]].first, cur_time + prefLen[u] - prefLen[u - 1]);
			}
			if (input.TimeWindows[path[u]].second >= cur_time && len <= input.MaxDist + EPS) {
				if (dp[u] > dp[v] + len) {
					dp[u] = dp[v] + len;
					prev[u] = v;
				}
			}
			else {
				break;
			}
		}
	}

	std::vector<std::vector<int>> paths;
	for (int i = n - 1; i != 0; i = prev[i]) {
		paths.push_back({});
		for (int j = i; j > std::max(-1, prev[i]); --j) {
			paths.back().push_back(path[j]);
		}
		std::reverse(paths.back().begin(), paths.back().end());
	}

	return paths;
}

Chromosome Mutation(Chromosome& c, double p, InputData& input, int cnt_improves = 3) {
	double x = Math::GenDouble(0, 1);
	if (x > p) {
		return c;
	}

	auto seq = c.Seq;
	auto paths = SplitPaths(seq, input);

	bool improved = true;

	// not more than left_improves improves;
	while (improved && cnt_improves > 0) {
		improved = false;
		improved |= GlobalInsertOptimization(paths, input);
		improved |= GlobalSwapOptimization(paths, input);
		for (auto& path : paths) {
			improved |= LocalSwapOptimization(path, input);
		}
		cnt_improves--;
	}

	return Chromosome(paths, input);
}

Population InitPopulation(InputData& input, int populationSize, double delta) {
	Population population(delta);
	
	// Greedy + greedy with local optimizations
	auto solGreedy = SolverGreedy::Run(input, ProblemMode::MINSUM, {});
	if (solGreedy.SolutionExists) {
		auto chromosomeGreedy = Chromosome(solGreedy);
		if (chromosomeGreedy.IsValid()) {
			population.Add(chromosomeGreedy);
			population.Add(Mutation(chromosomeGreedy, 1, input, 60));
		}
	}

	// Fill args for ACO
	std::vector<std::vector<double>> argss = {
		//{ 3, 4, 0.4, 2, 3, 2, 5, 0.5, 0 },
	};
	// Ant Colony Algo
	for (std::vector<double> args: argss) {
		if (population.Size() == populationSize) {
			break;
		}
		auto sol = SolverAntColony::Run(input, args);
		if (!sol.SolutionExists) {
			continue;
		}
		auto chromo = Chromosome(sol);
		if (chromo.IsValid()) {
			population.Add(chromo);
		}
	}

	int leftTries = populationSize * populationSize + 100;
	while (population.Size() < populationSize) {
		auto chromosomeCur = GenRandomChromosome(input.TargetsCnt, input);

		if (chromosomeCur.IsValid()) {
			if (Math::GenInt(0, 5)) {
				population.Add(Mutation(chromosomeCur, 1, input, 30));
			}
			else {
				population.Add(chromosomeCur);
			}
		}

		leftTries--;
		if (leftTries <= 0) {
			std::cout << "GA. Error: No tries left in init population. Number of done chromosomes: " << population.Size() << std::endl;
			if (input.TargetsCnt < 10) {
				std::cout << "GA. It's fine for small inputs" << std::endl;
			}
			break;
		}
	}

	for (auto& x : population.Chromosomes) {
		assert(x.Fitness < INF - EPS);
	}

	std::cout << "GA. Population initialized" << std::endl;
	return population;
}

int GenGoodIndex(int n) {
	int tmp = Math::GenInt(0, n * (n + 1) * (2 * n + 1) / 6 - 1);
	int cur = n;
	for (int i = 0; i < n; i++) {
		if (tmp < cur * cur) {
			return i;
		}
		tmp -= cur * cur;
		cur--;
	}
	return n - 1;
}

int GenIndexForReplace(int n) {
	int k = 2 * n / 3;
	int tmp = GenGoodIndex(k);
	return n - 1 - tmp;
}

// Args are: {n, alpha, betta, delta, p, timeLimit}
// Suggested args: {30, 3000, 1000, 0.5, 0.05, 0}
ProblemSolution SolverGenetic::Run(InputData input, std::vector<double>args) {
	for (int i = 1; i <= input.TargetsCnt; ++i) {
		if (input.Distance(0, i) * 2 > input.MaxDist - EPS || 
			input.TimeWindows[i].second < input.Distance(0, i)) {
			return ProblemSolution();
		}
	}

	int populationSize = (int)args[0]; // n

	// if timeLimit is not 0, these 2 variables are useless
	int maxProductiveIters = (int)args[1]; // alpha
	int maxNonImproveIters = (int)args[2]; // betta

	// 2 near chromosomes' fitnesses must differ at list on delta
	double delta = args[3];

	double mutationProb = args[4]; // p
	// if timeLimit is 0, then cycle is limited with maxProductiveIters and maxNonImproveIters
	int timeLimit = (int)args[5];

	double startT = clock();

	Population population = InitPopulation(input, populationSize, delta);

	int productiveIters = 0;
	int nonImproveIters = 0;

	int n = population.Size();

	while (timeLimit ? clock() - startT < timeLimit * CLOCKS_PER_SEC :
		productiveIters < maxProductiveIters && nonImproveIters < maxNonImproveIters) {

		assert(population.Size() == n);

		int parent1ind = GenGoodIndex(n);
		int parent2ind = GenGoodIndex(n);
		while (parent2ind == parent1ind) {
			parent2ind = GenGoodIndex(n);
		}
		if (parent2ind < parent1ind) {
			std::swap(parent1ind, parent2ind);
		}

		auto child = Crossover(population.Chromosomes[parent1ind], population.Chromosomes[parent2ind], input);
		auto mutatedChild = Mutation(child, mutationProb, input);

		int replacedInd = GenIndexForReplace(n);

		auto deletedChromosome = population.Chromosomes[replacedInd];

		population.Del(replacedInd);

		if (!population.IsAddible(mutatedChild) && population.IsAddible(child)) {
			mutatedChild = child;
		}

		nonImproveIters++;

		if (productiveIters % 100 == 0) {
			std::cout << "GA. Number of productive iterations: " << productiveIters << 
				". Number of iterations without improving best solution: " << nonImproveIters << std::endl;
		}

		if (population.IsAddible(mutatedChild)) {
			if (mutatedChild.Fitness < population.Chromosomes[0].Fitness) {
				nonImproveIters = 0;
				std::cout << "GA. New best solution found: " << mutatedChild.Fitness << std::endl;
			}
			productiveIters++;

			population.Add(mutatedChild);
		}
		else {
			population.Add(deletedChromosome);
		}
	}
	
	auto final_solution = ProblemSolution(input, SplitPaths(population.Chromosomes[0].Seq, input));
	return final_solution;
}
