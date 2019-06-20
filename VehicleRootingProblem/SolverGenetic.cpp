#include "SolverGenetic.h"
#include "SolverClarkeWright.h"
#include "SolverGreedy.h"
#include "SolverGreedyClusterized.h"
#include "Optimizations.h"

ProblemMode GlobalProblemMode;
int GlobalCntDrons;

struct Chromosome {
	std::vector<int> Seq;
	double Fitness;

	Chromosome() {}
	Chromosome(std::vector<int>& seq, double fitness) :
		Seq(seq),
		Fitness(fitness)
	{}

	static double CalcFitnessForMinMax(std::vector<int>& seq, InputData& input) {
		if (seq.size() != input.TargetsCnt) return 0;
		seq.insert(seq.begin(), { 0 });
		int n = seq.size();

		double* prefLen = new double[n];
		std::fill(prefLen, prefLen + n, 0);

		double tl = 0, tr = INF;
		for (int i = 1; i < n; i++) {
			prefLen[i] = prefLen[i - 1] + input.Distance(seq[i - 1], seq[i]);
			tl = std::max(tl, input.Distance(0, seq[i]) * 2);
		}
		tr = prefLen[n - 1] + input.Distance(seq.back(), 0);

		for (int it = 0; it < 55; it++) {
			double tm = (tl + tr) / 2;
			int cnt = 0;
			int cur = 0;
			while (cur != n - 1) {
				int cur2 = cur + 1;
				while (cur2 + 1 < n && input.Distance(0, seq[cur + 1]) + input.Distance(seq[cur2 + 1], 0) + 
					prefLen[cur2 + 1] - prefLen[cur + 1] < tm)
					cur2++;
				cnt++;
				assert(cur2 <= n - 1 && cur < cur2);
				cur = cur2;
			}
			if (cnt <= GlobalCntDrons) {
				tr = tm;
			}
			else {
				tl = tm;
			}
			
		}

		delete[] prefLen;
		seq.erase(seq.begin());

		return tl;
	}

	static double CalcFitness(std::vector<int>& seq, InputData& input) {
		if (GlobalProblemMode == ProblemMode::MINMAXLEN)
			return CalcFitnessForMinMax(seq, input);

		if (seq.size() != input.TargetsCnt) return 0;
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
		for (int v = 0; v < n; ++v) {
			if (dp[v] == INF) continue;
			double len = 0;
			for (int u = v + 1; u < n && len <= input.MaxDist + EPS; ++u) {
				// relaxing using edge (v, u)
				len = prefLen[u] - prefLen[v + 1] + input.Distance(0, seq[v + 1]) + input.Distance(seq[u], 0);
				if (len <= input.MaxDist + EPS) {
					dp[u] = std::min(dp[u], dp[v] + len);
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

	Chromosome(std::vector<std::vector<int>>& paths, InputData& input) {
		for (auto& path : paths)
			for (auto x : path)
				Seq.push_back(x);
		if (Seq.size() != input.TargetsCnt)
			Fitness = 0;
		else
			Fitness = CalcFitness(Seq, input);
	}

	Chromosome(ProblemSolution& solution) {
		for (auto& path : solution.Paths)
			for (auto x : path)
				Seq.push_back(x);
		if (Seq.size() != solution.Input.TargetsCnt)
			Fitness = 0;
		else
			Fitness = CalcFitness(Seq, solution.Input);
	}

	bool IsValid() {
		return *std::max_element(Seq.begin(), Seq.end()) == Seq.size() && Fitness > 0;
	}
};

std::mt19937 tmpGen = std::mt19937(1);

Chromosome GenRandomChromosome(int n, InputData& input) {
	std::vector<int> seq(n);
	for (int i = 1; i <= n; i++)
		seq[i - 1] = i;
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
		if (FitByDelta.count((int)(x.Fitness / Delta)) && FitByDelta[(int)(x.Fitness / Delta)] > 0)
			return false;
		if (Delta)
			FitByDelta[(int)(x.Fitness / Delta)]++;
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
					if (FitByDelta[arg] == 0)
						FitByDelta.erase(arg);
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
			if (FitByDelta[arg] == 0)
				FitByDelta.erase(arg);
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
	if (DataGenerator::GenInt(0, 1)) {
		std::swap(par1, par2);
	}
	int n = par1->Seq.size();
	int tl = DataGenerator::GenInt(0, n - 1);
	int tr = DataGenerator::GenInt(0, n - 1);
	if (tr < tl) std::swap(tr, tl);
	
	std::fill(used, used + n + 1, false);
	std::vector<int> child(n, 0);

	for (int i = tl; i <= tr; ++i) {
		child[i] = par1->Seq[i];
		used[par1->Seq[i]] = true;
	}

	int curChildInd = (tr + 1) % n;
	for (int i1 = (tr + 1); i1 < tr + n + 1; i1++) {
		int i = i1;
		while (i >= n)
			i -= n;
		if (!used[par2->Seq[i]]) {
			used[par2->Seq[i]] = true;
			child[curChildInd] = par2->Seq[i];
			curChildInd++;
			if (curChildInd >= n) curChildInd -= n;
		}
	}

	return Chromosome(child, input);
}

std::vector<std::vector<int>> SplitPathsMinMax(std::vector<int>& path, InputData& input) {
	if (path.size() == 0) return {};
	
	double fitness = Chromosome::CalcFitness(path, input);
	
	path.insert(path.begin(), { 0 });
	int n = path.size();

	std::fill(prefLen, prefLen + n, 0);
	for (int i = 1; i < n; i++) {
		prefLen[i] = prefLen[i - 1] + input.Distance(path[i - 1], path[i]);
	}

	std::vector<std::vector<int>> paths;

	int cnt = 0;
	int cur = 0;
	while (cur != n - 1) {
		int cur2 = cur + 1;
		while (cur2 + 1 < n && input.Distance(0, path[cur + 1]) + input.Distance(path[cur2 + 1], 0) + prefLen[cur2 + 1] - prefLen[cur + 1] < fitness + EPS)
			cur2++;
		cnt++;
		paths.push_back({});
		for (int i = cur + 1; i <= cur2; ++i) {
			paths.back().push_back(path[i]);
		}
		assert(cur2 <= n - 1 && cur < cur2);
		cur = cur2;
	}

	path.erase(path.begin());
	return paths;
}

std::vector<std::vector<int>> SplitPaths(std::vector<int> path, InputData& input) {
	if (GlobalProblemMode == ProblemMode::MINMAXLEN)
		return SplitPathsMinMax(path, input);

	if (path.size() == 0) return {};
	path.insert(path.begin(), { 0 });
	int n = path.size();

	std::fill(prefLen, prefLen + n, 0);
	for (int i = 1; i < n; i++) {
		prefLen[i] = prefLen[i - 1] + input.Distance(path[i - 1], path[i]);
	}

	std::fill(dp, dp + n, INF);
	std::fill(prev, prev + n, -1);
	dp[0] = 0;
	for (int v = 0; v < n; ++v) {
		if (dp[v] == INF) continue;
		double len = 0;
		for (int u = v + 1; u < n && len <= input.MaxDist; ++u) {
			// relaxing using edge (v, u)
			len = prefLen[u] - prefLen[v + 1] + input.Distance(0, path[v + 1]) + input.Distance(path[u], 0);
			if (len <= input.MaxDist) {
				if (dp[u] > dp[v] + len) {
					dp[u] = dp[v] + len;
					prev[u] = v;
				}
			}
		}
	}

	std::vector<std::vector<int>> paths;
	for (int i = n - 1; i != 0; i = prev[i]) {
		paths.push_back({});
		for (int j = i; j > std::max(-1, prev[i]); --j)
			paths.back().push_back(path[j]);
	}

	return paths;
}

Chromosome Mutation(Chromosome& c, double p, ProblemMode problemMode, InputData& input) {
	double x = DataGenerator::GenDouble(0, 1);
	if (x > p) return c;

	auto seq = c.Seq;
	auto paths = SplitPaths(seq, input);

	bool improved = true;

	while (improved) {
		improved = false;
		improved |= StringCrossOptimization(paths, input, problemMode, false);
		for (auto& path : paths)
			improved |= LocalReverseOptimization(path, input, false);
	}

	return Chromosome(paths, input);

}

Population InitPopulation(InputData& input, ProblemMode problemMode, int populationSize, double delta) {
	Population population(delta);
	
	// Clarke-Wright
	auto solCW = SolverClarkeWright::Run(input, problemMode, {});
	if (solCW.SolutionExists) {
		auto chromosomeCW = Chromosome(solCW);
		if (chromosomeCW.IsValid()) {
			population.Add(chromosomeCW);
		}
	}

	// Greedy
	auto solGreedy = SolverGreedy::Run(input, problemMode, {});
	if (solGreedy.SolutionExists) {
		auto chromosomeGreedy = Chromosome(solGreedy);
		if (chromosomeGreedy.IsValid()) {
			population.Add(chromosomeGreedy);
		}
	}

	// Greedy clusterized
	auto solGrClust = SolverGreedyClusterized::Run(input, problemMode, {});
	if (solGrClust.SolutionExists) {
		auto chromosomeGrClust = Chromosome(solGrClust);
		if (chromosomeGrClust.IsValid()) {
			population.Add(chromosomeGrClust);
		}
	}

	int leftTries = populationSize * populationSize + 10;
	while (population.Size() < populationSize) {

		auto chromosomeCur = GenRandomChromosome(input.TargetsCnt, input);

		if (chromosomeCur.IsValid()) {
			population.Add(chromosomeCur);
		}

		leftTries--;
		if (leftTries <= 0) {
			break;
		}
	}

	for (auto& x: population.Chromosomes)
		assert(x.Fitness < INF - EPS);

	return population;
}

// Args are: {n, alpha, betta, delta, p, timeLimit}
// Suggested args: {30, 30000, 10000, 0.5, 0.05, 0}
ProblemSolution SolverGenetic::Run(InputData input, ProblemMode problemMode, std::vector<double>args) {
	GlobalProblemMode = problemMode;
	GlobalCntDrons = input.DronsCnt;
	for (int i = 1; i <= input.TargetsCnt; ++i) {
		if (input.Distance(0, i) * 2 > input.MaxDist - 0.001) {
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

	Population population = InitPopulation(input, problemMode, populationSize, delta);

	int productiveIters = 0;
	int nonImproveIters = 0;

	int n = population.Size();

	while (timeLimit ? clock() - startT < timeLimit * CLOCKS_PER_SEC :
		productiveIters < maxProductiveIters && nonImproveIters < maxNonImproveIters) {

		assert(population.Size() == n);

		int parent1ind = std::min(DataGenerator::GenInt(0, n - 1), DataGenerator::GenInt(0, n - 1));
		int parent2ind = std::min(DataGenerator::GenInt(0, n - 1), DataGenerator::GenInt(0, n - 1));
		while (parent2ind == parent1ind) {
			parent2ind = std::min(DataGenerator::GenInt(0, n - 1), DataGenerator::GenInt(0, n - 1));
		}
		if (parent2ind < parent1ind) std::swap(parent1ind, parent2ind);

		auto child = Crossover(population.Chromosomes[parent1ind], population.Chromosomes[parent2ind], input);
		auto mutatedChild = Mutation(child, mutationProb, problemMode, input);

		int replacedInd = DataGenerator::GenInt(n / 2, n - 1);

		auto deletedChromosome = population.Chromosomes[replacedInd];

		population.Del(replacedInd);

		if (!population.IsAddible(mutatedChild) && population.IsAddible(child)) {
			mutatedChild = child;
		}

		nonImproveIters++;

		if (nonImproveIters % 50 == 0)
			std::cout << "iters: " << nonImproveIters << " " << productiveIters << std::endl;

		if (population.IsAddible(mutatedChild)) {
			if (mutatedChild.Fitness < population.Chromosomes[0].Fitness) {
				nonImproveIters = 0;
				std::cout << "New best fitness: " << mutatedChild.Fitness << std::endl;
			}
			productiveIters++;

			population.Add(mutatedChild);
		}
		else {
			population.Add(deletedChromosome);
		}
	}

	return ProblemSolution(input, SplitPaths(population.Chromosomes[0].Seq, input));
}
