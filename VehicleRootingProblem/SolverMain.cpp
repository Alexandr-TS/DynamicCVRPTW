#include "SolverMain.h"
#include "SolverClarkeWright.h"
#include "SolverDP.h"
#include "SolverAntColony.h"

ProblemSolution SolverMain::Run(InputData input, double timeLimit, 
	ProblemMode problemMode, EAlgorithms algorithm) {
	if (algorithm == EAlgorithms::DP) {
		return SolverDP::Run(input, timeLimit, problemMode);
	} else {
		return SolverClarkeWright::Run(input, timeLimit, problemMode);
	}
}

ProblemSolution SolverMain::Run(InputData input, double timeLimit, ProblemMode problemMode, 
	EAlgorithms algorithm, std::vector<double> args) {
	if (algorithm == EAlgorithms::AntColony) {
		return SolverAntColony::Run(input, timeLimit, problemMode, args);
	} 
}
