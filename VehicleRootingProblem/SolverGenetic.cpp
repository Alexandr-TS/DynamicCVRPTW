#include "SolverGenetic.h"
#include "SolverClarkeWright.h"
#include "SolverGreedy.h"
#include "SolverGreedyClusterized.h"
#include "Optimizations.h"

struct Chromosome {
	std::vector<int> Seq;
	double Fitness;

	Chromosome() {}
	Chromosome(std::vector<int> seq, double fitness) :
		Seq(seq),
		Fitness(fitness)
	{}

	double CalcFitness(std::vector<int> seq, InputData& input) {
		if (seq.size() != input.TargetsCnt) return 0;
		seq.insert(seq.begin(), { 0 });
		int n = seq.size();

		std::vector<double> prefLen(n, 0);
		for (int i = 1; i < n; i++) {
			prefLen[i] = prefLen[i - 1] + input.Distance(seq[i - 1], seq[i]);
		}

		std::vector<double> dp(n, INF);
		dp[0] = 0;
		for (int v = 0; v < n; ++v) {
			if (dp[v] == INF) continue;
			for (int u = v + 1; u < n; ++u) {
				// relaxing using edge (v, u)
				double len = prefLen[u] - prefLen[v + 1] + input.Distance(0, seq[v + 1]) + input.Distance(seq[u], 0);
				if (len <= input.MaxDist + EPS) {
					dp[u] = std::min(dp[u], dp[v] + len);
				}
			}
		}
		return dp[n - 1];
	}

	Chromosome(std::vector<int> seq, InputData& input) : Seq(seq) {
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

	void Del(Chromosome x) {
		for (auto it = Chromosomes.begin(); it != Chromosomes.end(); ++it) {
			if (it->Seq == x.Seq) {
				if (Delta)
					FitByDelta[(int)(x.Fitness / Delta)]--;
				Chromosomes.erase(it);
				break;
			}
		}
	}

	void Del(int index) {
		auto it = Chromosomes.begin() + index;
		if (Delta)
			FitByDelta[(int)(it->Fitness / Delta)]--;
		Chromosomes.erase(it);
	}

	int Size() {
		return Chromosomes.size();
	}
};

Chromosome Crossover(Chromosome parent1, Chromosome parent2, InputData& input) {
	if (DataGenerator::GenInt(0, 1)) {
		std::swap(parent1, parent2);
	}
	int n = parent1.Seq.size();
	int tl = DataGenerator::GenInt(0, n - 1);
	int tr = DataGenerator::GenInt(0, n - 1);
	if (tr < tl) std::swap(tr, tl);
	std::vector<bool> used(n + 1, false);
	std::vector<int> child(n, 0);
	for (int i = tl; i <= tr; ++i) {
		child[i] = parent1.Seq[i];
		used[parent1.Seq[i]] = true;
	}

	int curChildInd = (tr + 1) % n;
	for (int i1 = (tr + 1); i1 < tr + n + 1; i1++) {
		int i = i1 % n;
		if (!used[parent2.Seq[i]]) {
			used[parent2.Seq[i]] = true;
			child[curChildInd] = parent2.Seq[i];
			curChildInd++;
			if (curChildInd >= n) curChildInd -= n;
		}
	}

	return Chromosome(child, input);
}

std::vector<std::vector<int>> SplitPaths(std::vector<int> path, InputData& input) {
	if (path.size() == 0) return {};
	path.insert(path.begin(), { 0 });
	int n = path.size();

	std::vector<double> prefLen(n, 0);
	for (int i = 1; i < n; i++) {
		prefLen[i] = prefLen[i - 1] + input.Distance(path[i - 1], path[i]);
	}

	std::vector<double> dp(n, INF);
	std::vector<int> prev(n, -1);
	dp[0] = 0;
	for (int v = 0; v < n; ++v) {
		if (dp[v] == INF) continue;
		for (int u = v + 1; u < n; ++u) {
			// relaxing using edge (v, u)
			double len = prefLen[u] - prefLen[v + 1] + input.Distance(0, path[v + 1]) + input.Distance(path[u], 0);
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
		for (int j = i; j > prev[i]; --j)
			paths.back().push_back(path[j]);
	}

	return paths;
}

Chromosome Mutation(Chromosome c, double p, ProblemMode problemMode, InputData& input) {
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

	int leftTries = populationSize * populationSize + 100;
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

ProblemSolution RunMinSum(InputData input, ProblemMode problemMode, std::vector<double>args) {

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

		std::cout << productiveIters << " " << nonImproveIters << std::endl;

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

		if (population.IsAddible(mutatedChild)) {
			if (mutatedChild.Fitness < population.Chromosomes[0].Fitness) {
				nonImproveIters = 0;
				std::cout << mutatedChild.Fitness << std::endl;
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

// Args are: {n, alpha, betta, delta, p, timeLimit}
// Suggested args: {30, 30000, 10000, 0.5, 0.05, 0}
ProblemSolution SolverGenetic::Run(InputData input, ProblemMode problemMode, std::vector<double>args) {
	if (problemMode == ProblemMode::MINSUM) {
		return RunMinSum(input, problemMode, args);
	}
	else {
		ProblemSolution bestSol = RunMinSum(input, problemMode, args);
		bool improved = true;
		while (improved) {
			improved = false;
			double maxDist = input.MaxDist;
			if (bestSol.SolutionExists) {
				maxDist = std::min(maxDist, bestSol.MaxPathLength - 0.01);
			}
			input.MaxDist = maxDist;
			auto curSolution = RunMinSum(input, problemMode, args);
			if (curSolution.SolutionExists) {
				assert(bestSol.MaxPathLength > curSolution.MaxPathLength);
				improved = true;
				bestSol = curSolution;
			}
		}
		return bestSol;
	}
}
