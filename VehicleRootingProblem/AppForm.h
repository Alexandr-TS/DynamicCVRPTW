#pragma once

#include "DataClasses.h"
#include "SolverMain.h"

namespace VehicleRootingProblem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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

		struct Launch {
			int AlgoIndex;
			int DataSetIndex;
			std::vector<double> ParamsVals;
			ProblemSolution Solution;
			
			Launch() {}
			Launch(int algoIndex, int dataSetIndex, std::vector<double> paramsVals, ProblemSolution solution) :
				AlgoIndex(algoIndex),
				DataSetIndex(dataSetIndex),
				ParamsVals(paramsVals),
				Solution(solution)
			{}
		};

		std::vector<InputData> DataSets;
		std::vector<Algo> Algos = {
			Algo("Алгоритм Кларка-Райта", EAlgorithms::ClarkeWright, {}), 
			Algo("Муравьиный алгоритм", EAlgorithms::AntColony, {
				Param("Alpha", 5, "Показатель степени стойкости феромонов в формуле вероятности выбора ребра"),
				Param("Betta", 5, "Показатель степени функции видимости в формуле вероятности выбора ребра"),
				Param("Rho", 0.75, "Коэффициент стойкости феромнов"),
				Param("f", 2, "Коэффициент при расстоянии между вершинами в функции видимости"),
				Param("g", 3, "Коэффициент при разности расстояний до депо в функции видимости"),
				Param("Imax", 3, "Коэффициент количества итераций. Число итераций = Imax * n"),
				Param("Sigma", 6, "Число элитарных муравьев"),
				Param("CandList", 0.25, "Доля вершин в списке кандидатов на следующее ребро")
			})
		};
		std::vector<Launch> Launches;
	}

	/// <summary>
	/// Сводка для AppForm
	/// </summary>
	public ref class AppForm : public System::Windows::Forms::Form
	{


	public:
		AppForm(void)
		{
			InitializeComponent();
			comboBoxLaunchAlgo->Items->Clear();
			comboBoxResAlgo->Items->Clear();
			for (auto algo : AppFormVars::Algos) {
				comboBoxLaunchAlgo->Items->Add(System::String(algo.Name.c_str()).ToString());
				comboBoxResAlgo->Items->Add(System::String(algo.Name.c_str()).ToString());
			}
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~AppForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^ tabControl1;
	protected:
	private: System::Windows::Forms::TabPage^ tabPage1;
	private: System::Windows::Forms::ListView^ listViewLoadedDataSets;

	private: System::Windows::Forms::ColumnHeader^ colName;
	private: System::Windows::Forms::ColumnHeader^ colDrons;
	private: System::Windows::Forms::ColumnHeader^ colTargets;
	private: System::Windows::Forms::ColumnHeader^ colMaxDist;
	private: System::Windows::Forms::TabPage^ tabPage2;
	private: System::Windows::Forms::DataGridView^ dataGridViewParams;

	private: System::Windows::Forms::GroupBox^ groupBoxLaunches;




	private: System::Windows::Forms::GroupBox^ groupBoxLoadedDataSets;
	private: System::Windows::Forms::ComboBox^ comboBoxLaunchAlgo;

	private: System::Windows::Forms::Label^ labelChosenAlgo;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ ColParam;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ ColVal;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ ColRecom;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ ColComment;
	private: System::Windows::Forms::Button^ buttonRun;
	private: System::Windows::Forms::RadioButton^ radioButtonMinMaxLen;
	private: System::Windows::Forms::RadioButton^ radioButtonMinSum;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::ProgressBar^ progressBarAlgo;


	private: System::Windows::Forms::Button^ buttonFileOpen;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ buttonUploadFile;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog;
	private: System::Windows::Forms::GroupBox^ groupBoxResults;


	private: System::Windows::Forms::ComboBox^ comboBoxResAlgo;
	private: System::Windows::Forms::Label^ labelChosenResAlgo;
	private: System::Windows::Forms::Label^ labelPathsList;
	private: System::Windows::Forms::DataGridView^ dataGridViewPaths;

	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ Column2;
	private: System::Windows::Forms::Button^ buttonSavePathsToFile;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::ListView^ listViewSelectedResult;
	private: System::Windows::Forms::ListView^ listViewResults;
	private: System::ComponentModel::IContainer^ components;




	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::ListViewItem^ listViewItem2 = (gcnew System::Windows::Forms::ListViewItem(L""));
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxLaunches = (gcnew System::Windows::Forms::GroupBox());
			this->progressBarAlgo = (gcnew System::Windows::Forms::ProgressBar());
			this->buttonRun = (gcnew System::Windows::Forms::Button());
			this->radioButtonMinMaxLen = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonMinSum = (gcnew System::Windows::Forms::RadioButton());
			this->comboBoxLaunchAlgo = (gcnew System::Windows::Forms::ComboBox());
			this->labelChosenAlgo = (gcnew System::Windows::Forms::Label());
			this->dataGridViewParams = (gcnew System::Windows::Forms::DataGridView());
			this->ColParam = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColVal = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColRecom = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColComment = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->groupBoxLoadedDataSets = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->buttonUploadFile = (gcnew System::Windows::Forms::Button());
			this->buttonFileOpen = (gcnew System::Windows::Forms::Button());
			this->listViewLoadedDataSets = (gcnew System::Windows::Forms::ListView());
			this->colName = (gcnew System::Windows::Forms::ColumnHeader());
			this->colDrons = (gcnew System::Windows::Forms::ColumnHeader());
			this->colTargets = (gcnew System::Windows::Forms::ColumnHeader());
			this->colMaxDist = (gcnew System::Windows::Forms::ColumnHeader());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBoxResults = (gcnew System::Windows::Forms::GroupBox());
			this->comboBoxResAlgo = (gcnew System::Windows::Forms::ComboBox());
			this->labelChosenResAlgo = (gcnew System::Windows::Forms::Label());
			this->dataGridViewPaths = (gcnew System::Windows::Forms::DataGridView());
			this->labelPathsList = (gcnew System::Windows::Forms::Label());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->buttonSavePathsToFile = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->listViewResults = (gcnew System::Windows::Forms::ListView());
			this->listViewSelectedResult = (gcnew System::Windows::Forms::ListView());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBoxLaunches->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewParams))->BeginInit();
			this->groupBoxLoadedDataSets->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupBoxResults->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewPaths))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(12, 12);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(567, 701);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->groupBoxLaunches);
			this->tabPage1->Controls->Add(this->groupBoxLoadedDataSets);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(559, 675);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Тестирование";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// groupBoxLaunches
			// 
			this->groupBoxLaunches->Controls->Add(this->progressBarAlgo);
			this->groupBoxLaunches->Controls->Add(this->buttonRun);
			this->groupBoxLaunches->Controls->Add(this->radioButtonMinMaxLen);
			this->groupBoxLaunches->Controls->Add(this->radioButtonMinSum);
			this->groupBoxLaunches->Controls->Add(this->comboBoxLaunchAlgo);
			this->groupBoxLaunches->Controls->Add(this->labelChosenAlgo);
			this->groupBoxLaunches->Controls->Add(this->dataGridViewParams);
			this->groupBoxLaunches->Location = System::Drawing::Point(6, 316);
			this->groupBoxLaunches->Name = L"groupBoxLaunches";
			this->groupBoxLaunches->Size = System::Drawing::Size(545, 357);
			this->groupBoxLaunches->TabIndex = 3;
			this->groupBoxLaunches->TabStop = false;
			this->groupBoxLaunches->Text = L"Запуск алгоритмов";
			// 
			// progressBarAlgo
			// 
			this->progressBarAlgo->Location = System::Drawing::Point(181, 299);
			this->progressBarAlgo->Maximum = 1200;
			this->progressBarAlgo->Name = L"progressBarAlgo";
			this->progressBarAlgo->Size = System::Drawing::Size(358, 23);
			this->progressBarAlgo->Step = 1;
			this->progressBarAlgo->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBarAlgo->TabIndex = 23;
			// 
			// buttonRun
			// 
			this->buttonRun->Location = System::Drawing::Point(15, 299);
			this->buttonRun->Margin = System::Windows::Forms::Padding(2);
			this->buttonRun->Name = L"buttonRun";
			this->buttonRun->Size = System::Drawing::Size(145, 39);
			this->buttonRun->TabIndex = 22;
			this->buttonRun->Text = L"Запустить";
			this->buttonRun->UseVisualStyleBackColor = true;
			this->buttonRun->Click += gcnew System::EventHandler(this, &AppForm::ButtonRun_Click);
			// 
			// radioButtonMinMaxLen
			// 
			this->radioButtonMinMaxLen->AutoSize = true;
			this->radioButtonMinMaxLen->Location = System::Drawing::Point(324, 34);
			this->radioButtonMinMaxLen->Margin = System::Windows::Forms::Padding(2);
			this->radioButtonMinMaxLen->Name = L"radioButtonMinMaxLen";
			this->radioButtonMinMaxLen->Size = System::Drawing::Size(148, 17);
			this->radioButtonMinMaxLen->TabIndex = 21;
			this->radioButtonMinMaxLen->Text = L"Минимизировать время";
			this->radioButtonMinMaxLen->UseVisualStyleBackColor = true;
			// 
			// radioButtonMinSum
			// 
			this->radioButtonMinSum->AutoSize = true;
			this->radioButtonMinSum->Checked = true;
			this->radioButtonMinSum->Location = System::Drawing::Point(324, 12);
			this->radioButtonMinSum->Margin = System::Windows::Forms::Padding(2);
			this->radioButtonMinSum->Name = L"radioButtonMinSum";
			this->radioButtonMinSum->Size = System::Drawing::Size(206, 17);
			this->radioButtonMinSum->TabIndex = 20;
			this->radioButtonMinSum->TabStop = true;
			this->radioButtonMinSum->Text = L"Минимизировать суммарную длину";
			this->radioButtonMinSum->UseVisualStyleBackColor = true;
			// 
			// comboBoxLaunchAlgo
			// 
			this->comboBoxLaunchAlgo->FormattingEnabled = true;
			this->comboBoxLaunchAlgo->Location = System::Drawing::Point(78, 16);
			this->comboBoxLaunchAlgo->Name = L"comboBoxLaunchAlgo";
			this->comboBoxLaunchAlgo->Size = System::Drawing::Size(226, 21);
			this->comboBoxLaunchAlgo->TabIndex = 3;
			// 
			// labelChosenAlgo
			// 
			this->labelChosenAlgo->AutoSize = true;
			this->labelChosenAlgo->Location = System::Drawing::Point(12, 16);
			this->labelChosenAlgo->Name = L"labelChosenAlgo";
			this->labelChosenAlgo->Size = System::Drawing::Size(59, 13);
			this->labelChosenAlgo->TabIndex = 2;
			this->labelChosenAlgo->Text = L"Алгоритм:";
			// 
			// dataGridViewParams
			// 
			this->dataGridViewParams->AllowUserToAddRows = false;
			this->dataGridViewParams->AllowUserToDeleteRows = false;
			this->dataGridViewParams->AllowUserToResizeRows = false;
			this->dataGridViewParams->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewParams->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(4) {
				this->ColParam,
					this->ColVal, this->ColRecom, this->ColComment
			});
			this->dataGridViewParams->Location = System::Drawing::Point(15, 56);
			this->dataGridViewParams->Name = L"dataGridViewParams";
			this->dataGridViewParams->Size = System::Drawing::Size(524, 223);
			this->dataGridViewParams->TabIndex = 1;
			this->dataGridViewParams->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &AppForm::DataGridViewParams_CellContentClick);
			// 
			// ColParam
			// 
			this->ColParam->HeaderText = L"Параметр";
			this->ColParam->Name = L"ColParam";
			this->ColParam->ReadOnly = true;
			this->ColParam->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// ColVal
			// 
			this->ColVal->HeaderText = L"Значение";
			this->ColVal->Name = L"ColVal";
			this->ColVal->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->ColVal->Width = 90;
			// 
			// ColRecom
			// 
			this->ColRecom->HeaderText = L"Рекомендуемое";
			this->ColRecom->Name = L"ColRecom";
			this->ColRecom->ReadOnly = true;
			this->ColRecom->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->ColRecom->Width = 90;
			// 
			// ColComment
			// 
			this->ColComment->HeaderText = L"Комментарий";
			this->ColComment->Name = L"ColComment";
			this->ColComment->ReadOnly = true;
			this->ColComment->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->ColComment->Width = 200;
			// 
			// groupBoxLoadedDataSets
			// 
			this->groupBoxLoadedDataSets->Controls->Add(this->label1);
			this->groupBoxLoadedDataSets->Controls->Add(this->buttonUploadFile);
			this->groupBoxLoadedDataSets->Controls->Add(this->buttonFileOpen);
			this->groupBoxLoadedDataSets->Controls->Add(this->listViewLoadedDataSets);
			this->groupBoxLoadedDataSets->Location = System::Drawing::Point(6, 6);
			this->groupBoxLoadedDataSets->Name = L"groupBoxLoadedDataSets";
			this->groupBoxLoadedDataSets->Size = System::Drawing::Size(545, 304);
			this->groupBoxLoadedDataSets->TabIndex = 2;
			this->groupBoxLoadedDataSets->TabStop = false;
			this->groupBoxLoadedDataSets->Text = L"яф";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(125, 268);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(92, 13);
			this->label1->TabIndex = 11;
			this->label1->Text = L"Файл не выбран";
			// 
			// buttonUploadFile
			// 
			this->buttonUploadFile->Location = System::Drawing::Point(223, 263);
			this->buttonUploadFile->Name = L"buttonUploadFile";
			this->buttonUploadFile->Size = System::Drawing::Size(161, 23);
			this->buttonUploadFile->TabIndex = 10;
			this->buttonUploadFile->Text = L"Загрузить";
			this->buttonUploadFile->UseVisualStyleBackColor = true;
			// 
			// buttonFileOpen
			// 
			this->buttonFileOpen->Location = System::Drawing::Point(5, 264);
			this->buttonFileOpen->Margin = System::Windows::Forms::Padding(2);
			this->buttonFileOpen->Name = L"buttonFileOpen";
			this->buttonFileOpen->Size = System::Drawing::Size(115, 22);
			this->buttonFileOpen->TabIndex = 9;
			this->buttonFileOpen->Text = L"Выберите файл";
			this->buttonFileOpen->UseVisualStyleBackColor = true;
			this->buttonFileOpen->Click += gcnew System::EventHandler(this, &AppForm::ButtonFileOpen_Click);
			// 
			// listViewLoadedDataSets
			// 
			this->listViewLoadedDataSets->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {
				this->colName,
					this->colDrons, this->colTargets, this->colMaxDist
			});
			this->listViewLoadedDataSets->FullRowSelect = true;
			this->listViewLoadedDataSets->GridLines = true;
			this->listViewLoadedDataSets->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(1) { listViewItem2 });
			this->listViewLoadedDataSets->Location = System::Drawing::Point(6, 19);
			this->listViewLoadedDataSets->Name = L"listViewLoadedDataSets";
			this->listViewLoadedDataSets->Size = System::Drawing::Size(533, 238);
			this->listViewLoadedDataSets->TabIndex = 0;
			this->listViewLoadedDataSets->UseCompatibleStateImageBehavior = false;
			this->listViewLoadedDataSets->View = System::Windows::Forms::View::Details;
			// 
			// colName
			// 
			this->colName->Text = L"Номер";
			this->colName->Width = 125;
			// 
			// colDrons
			// 
			this->colDrons->Text = L"Количество БПЛА";
			this->colDrons->Width = 108;
			// 
			// colTargets
			// 
			this->colTargets->Text = L"Количество целей";
			this->colTargets->Width = 109;
			// 
			// colMaxDist
			// 
			this->colMaxDist->Text = L"Максимальная дальность полёта";
			this->colMaxDist->Width = 184;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->listViewSelectedResult);
			this->tabPage2->Controls->Add(this->pictureBox1);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(559, 675);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Визуализация";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &AppForm::Timer1_Tick);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog1";
			// 
			// groupBoxResults
			// 
			this->groupBoxResults->Controls->Add(this->listViewResults);
			this->groupBoxResults->Controls->Add(this->buttonSavePathsToFile);
			this->groupBoxResults->Controls->Add(this->labelPathsList);
			this->groupBoxResults->Controls->Add(this->dataGridViewPaths);
			this->groupBoxResults->Controls->Add(this->comboBoxResAlgo);
			this->groupBoxResults->Controls->Add(this->labelChosenResAlgo);
			this->groupBoxResults->Location = System::Drawing::Point(585, 34);
			this->groupBoxResults->Name = L"groupBoxResults";
			this->groupBoxResults->Size = System::Drawing::Size(567, 679);
			this->groupBoxResults->TabIndex = 1;
			this->groupBoxResults->TabStop = false;
			this->groupBoxResults->Text = L"Результаты";
			// 
			// comboBoxResAlgo
			// 
			this->comboBoxResAlgo->FormattingEnabled = true;
			this->comboBoxResAlgo->Location = System::Drawing::Point(72, 16);
			this->comboBoxResAlgo->Name = L"comboBoxResAlgo";
			this->comboBoxResAlgo->Size = System::Drawing::Size(226, 21);
			this->comboBoxResAlgo->TabIndex = 25;
			// 
			// labelChosenResAlgo
			// 
			this->labelChosenResAlgo->AutoSize = true;
			this->labelChosenResAlgo->Location = System::Drawing::Point(6, 16);
			this->labelChosenResAlgo->Name = L"labelChosenResAlgo";
			this->labelChosenResAlgo->Size = System::Drawing::Size(59, 13);
			this->labelChosenResAlgo->TabIndex = 24;
			this->labelChosenResAlgo->Text = L"Алгоритм:";
			// 
			// dataGridViewPaths
			// 
			this->dataGridViewPaths->AllowUserToAddRows = false;
			this->dataGridViewPaths->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewPaths->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {
				this->Column1,
					this->Column2
			});
			this->dataGridViewPaths->Location = System::Drawing::Point(6, 355);
			this->dataGridViewPaths->Name = L"dataGridViewPaths";
			this->dataGridViewPaths->ReadOnly = true;
			this->dataGridViewPaths->Size = System::Drawing::Size(555, 283);
			this->dataGridViewPaths->TabIndex = 27;
			// 
			// labelPathsList
			// 
			this->labelPathsList->AutoSize = true;
			this->labelPathsList->Location = System::Drawing::Point(9, 336);
			this->labelPathsList->Name = L"labelPathsList";
			this->labelPathsList->Size = System::Drawing::Size(75, 13);
			this->labelPathsList->TabIndex = 28;
			this->labelPathsList->Text = L"Список путей";
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"Пути";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column1->Width = 350;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"Длины";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->Width = 155;
			// 
			// buttonSavePathsToFile
			// 
			this->buttonSavePathsToFile->Location = System::Drawing::Point(6, 644);
			this->buttonSavePathsToFile->Name = L"buttonSavePathsToFile";
			this->buttonSavePathsToFile->Size = System::Drawing::Size(134, 29);
			this->buttonSavePathsToFile->TabIndex = 29;
			this->buttonSavePathsToFile->Text = L"Сохранить";
			this->buttonSavePathsToFile->UseVisualStyleBackColor = true;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(4, 91);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(549, 578);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// listViewResults
			// 
			this->listViewResults->FullRowSelect = true;
			this->listViewResults->GridLines = true;
			this->listViewResults->Location = System::Drawing::Point(6, 43);
			this->listViewResults->Name = L"listViewResults";
			this->listViewResults->Size = System::Drawing::Size(556, 290);
			this->listViewResults->TabIndex = 30;
			this->listViewResults->UseCompatibleStateImageBehavior = false;
			this->listViewResults->View = System::Windows::Forms::View::Details;
			// 
			// listViewSelectedResult
			// 
			this->listViewSelectedResult->FullRowSelect = true;
			this->listViewSelectedResult->GridLines = true;
			this->listViewSelectedResult->Location = System::Drawing::Point(6, 6);
			this->listViewSelectedResult->Name = L"listViewSelectedResult";
			this->listViewSelectedResult->Size = System::Drawing::Size(547, 79);
			this->listViewSelectedResult->TabIndex = 1;
			this->listViewSelectedResult->UseCompatibleStateImageBehavior = false;
			this->listViewSelectedResult->View = System::Windows::Forms::View::Details;
			// 
			// AppForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1159, 718);
			this->Controls->Add(this->groupBoxResults);
			this->Controls->Add(this->tabControl1);
			this->Name = L"AppForm";
			this->Text = L"AppForm";
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->groupBoxLaunches->ResumeLayout(false);
			this->groupBoxLaunches->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewParams))->EndInit();
			this->groupBoxLoadedDataSets->ResumeLayout(false);
			this->groupBoxLoadedDataSets->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->groupBoxResults->ResumeLayout(false);
			this->groupBoxResults->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewPaths))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		this->progressBarAlgo->PerformStep();
	}
private: System::Void ButtonRun_Click(System::Object^ sender, System::EventArgs^ e) {
}

private: System::Void ButtonFileOpen_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void DataGridViewParams_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
}
};
}
