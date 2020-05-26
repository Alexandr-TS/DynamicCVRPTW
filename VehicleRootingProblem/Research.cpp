#include "Research.h"
#include "DataClasses.h"
#include "SolverMain.h"
#include "EventsHandler.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using namespace Research;
using namespace std;

ofstream outlog("C:\\Users\\Admin\\OneDrive\\UNIVER\\11trimestr\\DynamicCVRPTW\\VehicleRootingProblem\\research_logs.txt");

namespace AppFormVars {
	struct Param {
		std::string Name;
		double Recommend;
		std::string Comment;

		Param() {}
		Param(std::string name, double recommend, std::string comment) :
			Name(name),
			Recommend(recommend),
			Comment(comment)
		{}
	};

	struct Algo {
		std::string Name;
		EAlgorithms Ealgo;
		std::vector<Param> Params;

		Algo() {}
		Algo(std::string name, EAlgorithms ealgo, std::vector<Param> params) :
			Name(name),
			Ealgo(ealgo),
			Params(params)
		{}
	};

	std::vector<Algo> Algos = {
		Algo("Муравьиный алгоритм", EAlgorithms::AntColony, {
			Param("Alpha", 3, "Показатель степени стойкости феромонов в формуле вероятности выбора ребра"),
			Param("Betta", 4, "Показатель степени функции видимости в формуле вероятности выбора ребра"),
			Param("Rho", 0.4, "Коэффициент стойкости феромнов"),
			Param("f", 2, "Коэффициент при расстоянии между вершинами в функции видимости"),
			Param("g", 3, "Коэффициент при разности расстояний до депо в функции видимости"),
			Param("Imax", 2, "Коэффициент количества итераций. Число итераций = Imax * n"),
			Param("Sigma", 5, "Число элитарных муравьев"),
			Param("CandList", 0.5, "Доля вершин в списке кандидатов на следующее ребро"),
			Param("TimeLimit", 0, "Ограничение по времени. Если не 0, то Imax не учитывается. Если 0, то Imax не учитывается")
		})
		, Algo("Генетический алгоритм", EAlgorithms::Genetic, {
			Param("N", 30, "Размер популяции"),
			Param("Alpha", 1500, "Максимальное количество изменений популяции"),
			Param("Betta", 900, "Максимальное количество итераций без улучшения лучшего решения"),
			Param("Delta", 0.5, "Минимальная разность фитнес-функций двух решений"),
			Param("P", 0.5, "Вероятность мутации"),
			Param("TimeLimit", 0, "Ограничение по времени. Если не 0, то Alpha и Betta не учитываются")
		})
		, Algo("Полный перебор", EAlgorithms::BruteForce, {})
	};
}


vector<InputData> MakeInputDatas(const string& directory_name) {
	filesystem::directory_iterator dir_iterator;
	try {
		dir_iterator = filesystem::directory_iterator(directory_name);
	}
	catch (const filesystem::filesystem_error&) {
		cout << "Error: No such directory: " << directory_name << ". ";
		cout << "Please check the directory path" << endl;
		return {};
	}

	vector<InputData> result;
	for (const auto& file : dir_iterator) {
		result.emplace_back(InputData(file.path().string()));
		outlog << "finished reading: " << file.path().string() << endl;
	}
	return result;
}

vector<ProblemSolution> BuildSolutions(const vector<InputData>& input_files) {
	using namespace AppFormVars;

	vector<ProblemSolution> result;
	int test_set_idx = 1;
	for (const auto& input: input_files) {
		ProblemMode mode = ProblemMode::MINSUM;
		int algoInd = 1;
		if (input.TargetsCnt <= 9) {
			algoInd = 2;
		}
		else if (input.TargetsCnt > 90) {
			algoInd = 0;
		}
		vector<double> args;
		for (int i = 0; i < static_cast<int>(Algos[algoInd].Params.size()); ++i) {
			args.push_back(Algos[algoInd].Params[i].Recommend);
		}

		result.push_back(SolverMain::Run(input, mode, Algos[algoInd].Ealgo, args));
		outlog << "finished building test #" << test_set_idx++ << " out of " << input_files.size() << endl;
	}
	return result;
}



void Research::RunResearch() {
	auto input_files = MakeInputDatas("C:\\Users\\Admin\\OneDrive\\UNIVER\\11trimestr\\DynamicCVRPTW\\InputGenerator\\tests");
	auto solutions = BuildSolutions(input_files);
}
