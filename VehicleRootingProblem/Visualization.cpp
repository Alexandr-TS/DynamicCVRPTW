#include "Visualization.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

void DrawPaths(System::Drawing::Graphics^ graphics, ProblemSolution solution, int height, int width) {
	int sumPoints = 0;
	for (auto path : solution.Paths)
		sumPoints += path.size();
	if (!sumPoints)
		return;
	double minX = 0, maxX = 0, minY = 0, maxY = 0;
	for (auto pt : solution.Input.Points) {
		minX = std::min(minX, pt.first);
		maxX = std::max(maxX, pt.first);
		minY = std::min(minY, pt.second);
		maxY = std::max(maxY, pt.second);
	}
	int sz2 = std::min(height - 30, width - 30);
	int sz1 = (int)std::max(maxY - minY, maxX - minX);
	sz1 = std::max(sz1, 1);

	auto points = solution.Input.Points;
	for (auto& pt : points)
		pt = std::make_pair((pt.first - minX) * sz2 / sz1 + 15, 15 + (pt.second - minY) * sz2 / sz1);

	graphics->Clear(Color::White);

	for (auto path : solution.Paths) {
		path.push_back(0);
		path.push_back(0);
		rotate(path.begin(), path.begin() + path.size() - 1, path.end());
		auto pen = (gcnew Pen(Color::FromArgb(rand() % 255, rand() % 255, rand() % 255), 3));
		for (int i = 0; i + 1 < (int)path.size(); ++i) {

			graphics->DrawLine(pen, (int)points[path[i]].first, (int)points[path[i]].second,
														(int)points[path[i + 1]].first, (int)points[path[i + 1]].second);
		}
	}

	auto pen = (gcnew Pen(Color::FromArgb(255, 0, 0), 3));
	for (auto pt: points)
		graphics->DrawEllipse(pen, Rectangle((int)pt.first - 2, (int)pt.second - 2, 4, 4));
	pen = (gcnew Pen(Color::FromArgb(0, 255, 0), 3));
	
	graphics->DrawEllipse(pen, Rectangle((int)points[0].first - 2, (int)points[0].second - 2, 4, 4));
}