#include "AppForm.h"
#include "Research.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	bool research_mode = false;

	if (research_mode) {
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, "CONOUT$", "w", stdout);
		RunTests();
		Research::RunResearch();
	}
	else {
		VehicleRootingProblem::AppForm form;
		Application::Run(% form);
	}
	return 0;
}
