#include "SolverMain.h"
#include "SolverClarkeWright.h"
#include "SolverDP.h"
#include "SolverAntColony.h"
#include "SolverGenetic.h"

ProblemSolution SolverMain::Run(InputData input, ProblemMode problemMode, 
	EAlgorithms algorithm, std::vector<double> args) {
	if (algorithm == EAlgorithms::DP) {
		return SolverDP::Run(input, problemMode, args);
	}
	else if (algorithm == EAlgorithms::ClarkeWright) {
		return SolverClarkeWright::Run(input, problemMode, args);
	} 
	else if (algorithm == EAlgorithms::AntColony) {
		return SolverAntColony::Run(input, problemMode, args);
	}
	else if (algorithm == EAlgorithms::Genetic) {
		return SolverGenetic::Run(input, problemMode, args);
	}
}
