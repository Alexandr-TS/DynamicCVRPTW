#include "Utils.h"

using namespace std;

static std::mt19937 randGen;

int Math::GenInt(int l, int r) {
	std::uniform_int_distribution<int> uid(l, r);
	return uid(randGen);
}

double Math::GenDouble(double l, double r) {
	std::uniform_real_distribution<double> urd(l, r);
	return urd(randGen);
}

double Math::PowInt(double x, int alpha) {
	double ans = 1;
	for (int i = 0; i < alpha; ++i) {
		ans *= x;
	}
	return ans;
}
