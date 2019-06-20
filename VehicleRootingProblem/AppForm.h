#pragma once

#include "DataClasses.h"
#include "SolverMain.h"
#include "Visualization.h"

#include <memory>
#include <msclr\marshal_cppstd.h>
#include <cstring>
#include <string>
#include <algorithm>

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
			ProblemMode Mode;
			ProblemSolution Solution;
			
			Launch() {}
			Launch(int algoIndex, int dataSetIndex, std::vector<double> paramsVals, ProblemMode mode, ProblemSolution solution) :
				AlgoIndex(algoIndex),
				DataSetIndex(dataSetIndex),
				ParamsVals(paramsVals),
				Mode(mode),
				Solution(solution)
			{}
		};

		std::vector<InputData> DataSets;
		std::vector<Algo> Algos = {
			Algo("Алгоритм Кларка-Райта", EAlgorithms::ClarkeWright, {}), 
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
			}),
			Algo("Генетический алгоритм", EAlgorithms::Genetic, {
				Param("N", 30, "Размер популяции"),
				Param("Alpha", 3000, "Максимальное количество изменений популяции"),
				Param("Betta", 1000, "Максимальное количество итераций без улучшения лучшего решения"),
				Param("Delta", 0.5, "Минимальная разность фитнес-функций двух решений"),
				Param("P", 0.5, "Вероятность мутации"),
				Param("TimeLimit", 0, "Ограничение по времени. Если не 0, то Alpha и Betta не учитываются")
			})
		};
		std::vector<Launch> Launches;
		InputData LoadedInputData;
		int PrintedLaunchPathsIndex;
	}

	System::String^ ToText(std::string s) {
		return System::String(s.c_str()).ToString();
	}

	System::String^ ToText(int x) {
		return System::Int32(x).ToString();
	}

	System::String^ ToText(double x) {
		return System::Double(x).ToString()->Replace(",", ".");
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
			comboBoxLaunchAlgo->Items->Add("");
			comboBoxResAlgo->Items->Add("");
			listViewLoadedDataSets->Items->Clear();

			for (auto algo : AppFormVars::Algos) {
				comboBoxLaunchAlgo->Items->Add(ToText(algo.Name.c_str()));
				comboBoxResAlgo->Items->Add(ToText(algo.Name.c_str()));
			}

			this->buttonSavePathsToFile->Enabled = false;
			buttonUploadFile->Enabled = false;
			buttonRun->Enabled = false;
			
			AppFormVars::PrintedLaunchPathsIndex = -1;

			Graphics = this->pictureBoxRes->CreateGraphics();
		}
		System::Drawing::Graphics^ Graphics;

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
	private: System::Windows::Forms::TabControl^ tabControlLeft;
	protected:

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




	private: System::Windows::Forms::Button^ buttonFileOpen;
	private: System::Windows::Forms::Label^ labelLoadedFile;

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
	private: System::Windows::Forms::PictureBox^ pictureBoxRes;



	private: System::Windows::Forms::DataGridView^ dataGridViewResults;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog;
	private: System::Windows::Forms::DataGridView^ dataGridViewSelectedRes;


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
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::ListViewItem^ listViewItem1 = (gcnew System::Windows::Forms::ListViewItem(L""));
			this->tabControlLeft = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->groupBoxLaunches = (gcnew System::Windows::Forms::GroupBox());
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
			this->labelLoadedFile = (gcnew System::Windows::Forms::Label());
			this->buttonUploadFile = (gcnew System::Windows::Forms::Button());
			this->buttonFileOpen = (gcnew System::Windows::Forms::Button());
			this->listViewLoadedDataSets = (gcnew System::Windows::Forms::ListView());
			this->colName = (gcnew System::Windows::Forms::ColumnHeader());
			this->colDrons = (gcnew System::Windows::Forms::ColumnHeader());
			this->colTargets = (gcnew System::Windows::Forms::ColumnHeader());
			this->colMaxDist = (gcnew System::Windows::Forms::ColumnHeader());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->dataGridViewSelectedRes = (gcnew System::Windows::Forms::DataGridView());
			this->pictureBoxRes = (gcnew System::Windows::Forms::PictureBox());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBoxResults = (gcnew System::Windows::Forms::GroupBox());
			this->dataGridViewResults = (gcnew System::Windows::Forms::DataGridView());
			this->buttonSavePathsToFile = (gcnew System::Windows::Forms::Button());
			this->labelPathsList = (gcnew System::Windows::Forms::Label());
			this->dataGridViewPaths = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->comboBoxResAlgo = (gcnew System::Windows::Forms::ComboBox());
			this->labelChosenResAlgo = (gcnew System::Windows::Forms::Label());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->tabControlLeft->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBoxLaunches->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewParams))->BeginInit();
			this->groupBoxLoadedDataSets->SuspendLayout();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewSelectedRes))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxRes))->BeginInit();
			this->groupBoxResults->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewResults))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewPaths))->BeginInit();
			this->SuspendLayout();
			// 
			// tabControlLeft
			// 
			this->tabControlLeft->Controls->Add(this->tabPage1);
			this->tabControlLeft->Controls->Add(this->tabPage2);
			this->tabControlLeft->Location = System::Drawing::Point(12, 12);
			this->tabControlLeft->Name = L"tabControlLeft";
			this->tabControlLeft->SelectedIndex = 0;
			this->tabControlLeft->Size = System::Drawing::Size(567, 701);
			this->tabControlLeft->TabIndex = 0;
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
			this->comboBoxLaunchAlgo->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxLaunchAlgo->FormattingEnabled = true;
			this->comboBoxLaunchAlgo->Location = System::Drawing::Point(78, 16);
			this->comboBoxLaunchAlgo->Name = L"comboBoxLaunchAlgo";
			this->comboBoxLaunchAlgo->Size = System::Drawing::Size(226, 21);
			this->comboBoxLaunchAlgo->TabIndex = 3;
			this->comboBoxLaunchAlgo->SelectedIndexChanged += gcnew System::EventHandler(this, &AppForm::ComboBoxLaunchAlgo_SelectedIndexChanged);
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
			dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Window;
			dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(204)));
			dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::ControlText;
			dataGridViewCellStyle1->NullValue = nullptr;
			dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridViewParams->DefaultCellStyle = dataGridViewCellStyle1;
			this->dataGridViewParams->Location = System::Drawing::Point(15, 56);
			this->dataGridViewParams->Name = L"dataGridViewParams";
			this->dataGridViewParams->Size = System::Drawing::Size(524, 223);
			this->dataGridViewParams->TabIndex = 1;
			this->dataGridViewParams->EditingControlShowing += gcnew System::Windows::Forms::DataGridViewEditingControlShowingEventHandler(this, &AppForm::dataGridViewParams_EditingControlShowing);
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
			this->groupBoxLoadedDataSets->Controls->Add(this->labelLoadedFile);
			this->groupBoxLoadedDataSets->Controls->Add(this->buttonUploadFile);
			this->groupBoxLoadedDataSets->Controls->Add(this->buttonFileOpen);
			this->groupBoxLoadedDataSets->Controls->Add(this->listViewLoadedDataSets);
			this->groupBoxLoadedDataSets->Location = System::Drawing::Point(6, 6);
			this->groupBoxLoadedDataSets->Name = L"groupBoxLoadedDataSets";
			this->groupBoxLoadedDataSets->Size = System::Drawing::Size(545, 304);
			this->groupBoxLoadedDataSets->TabIndex = 2;
			this->groupBoxLoadedDataSets->TabStop = false;
			this->groupBoxLoadedDataSets->Text = L"Загрузка входных данных";
			// 
			// labelLoadedFile
			// 
			this->labelLoadedFile->AutoSize = true;
			this->labelLoadedFile->Location = System::Drawing::Point(125, 268);
			this->labelLoadedFile->MaximumSize = System::Drawing::Size(95, 13);
			this->labelLoadedFile->Name = L"labelLoadedFile";
			this->labelLoadedFile->Size = System::Drawing::Size(92, 13);
			this->labelLoadedFile->TabIndex = 11;
			this->labelLoadedFile->Text = L"Файл не выбран";
			// 
			// buttonUploadFile
			// 
			this->buttonUploadFile->Location = System::Drawing::Point(223, 263);
			this->buttonUploadFile->Name = L"buttonUploadFile";
			this->buttonUploadFile->Size = System::Drawing::Size(161, 23);
			this->buttonUploadFile->TabIndex = 10;
			this->buttonUploadFile->Text = L"Загрузить";
			this->buttonUploadFile->UseVisualStyleBackColor = true;
			this->buttonUploadFile->Click += gcnew System::EventHandler(this, &AppForm::ButtonUploadFile_Click);
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
			this->listViewLoadedDataSets->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(1) { listViewItem1 });
			this->listViewLoadedDataSets->Location = System::Drawing::Point(6, 19);
			this->listViewLoadedDataSets->MultiSelect = false;
			this->listViewLoadedDataSets->Name = L"listViewLoadedDataSets";
			this->listViewLoadedDataSets->Size = System::Drawing::Size(533, 238);
			this->listViewLoadedDataSets->TabIndex = 0;
			this->listViewLoadedDataSets->UseCompatibleStateImageBehavior = false;
			this->listViewLoadedDataSets->View = System::Windows::Forms::View::Details;
			this->listViewLoadedDataSets->SelectedIndexChanged += gcnew System::EventHandler(this, &AppForm::ListViewLoadedDataSets_SelectedIndexChanged);
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
			this->tabPage2->Controls->Add(this->dataGridViewSelectedRes);
			this->tabPage2->Controls->Add(this->pictureBoxRes);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(559, 675);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Визуализация";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// dataGridViewSelectedRes
			// 
			this->dataGridViewSelectedRes->AllowUserToAddRows = false;
			this->dataGridViewSelectedRes->AllowUserToDeleteRows = false;
			this->dataGridViewSelectedRes->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridViewSelectedRes->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewSelectedRes->Location = System::Drawing::Point(6, 3);
			this->dataGridViewSelectedRes->Name = L"dataGridViewSelectedRes";
			this->dataGridViewSelectedRes->ReadOnly = true;
			this->dataGridViewSelectedRes->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders;
			this->dataGridViewSelectedRes->Size = System::Drawing::Size(547, 82);
			this->dataGridViewSelectedRes->TabIndex = 32;
			// 
			// pictureBoxRes
			// 
			this->pictureBoxRes->Location = System::Drawing::Point(4, 91);
			this->pictureBoxRes->Name = L"pictureBoxRes";
			this->pictureBoxRes->Size = System::Drawing::Size(549, 578);
			this->pictureBoxRes->TabIndex = 0;
			this->pictureBoxRes->TabStop = false;
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog1";
			// 
			// groupBoxResults
			// 
			this->groupBoxResults->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBoxResults->Controls->Add(this->dataGridViewResults);
			this->groupBoxResults->Controls->Add(this->buttonSavePathsToFile);
			this->groupBoxResults->Controls->Add(this->labelPathsList);
			this->groupBoxResults->Controls->Add(this->dataGridViewPaths);
			this->groupBoxResults->Controls->Add(this->comboBoxResAlgo);
			this->groupBoxResults->Controls->Add(this->labelChosenResAlgo);
			this->groupBoxResults->Location = System::Drawing::Point(591, 0);
			this->groupBoxResults->MaximumSize = System::Drawing::Size(1200, 718);
			this->groupBoxResults->MinimumSize = System::Drawing::Size(568, 718);
			this->groupBoxResults->Name = L"groupBoxResults";
			this->groupBoxResults->Size = System::Drawing::Size(568, 718);
			this->groupBoxResults->TabIndex = 1;
			this->groupBoxResults->TabStop = false;
			this->groupBoxResults->Text = L"Результаты";
			// 
			// dataGridViewResults
			// 
			this->dataGridViewResults->AllowUserToAddRows = false;
			this->dataGridViewResults->AllowUserToDeleteRows = false;
			this->dataGridViewResults->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridViewResults->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewResults->Location = System::Drawing::Point(6, 43);
			this->dataGridViewResults->Name = L"dataGridViewResults";
			this->dataGridViewResults->ReadOnly = true;
			this->dataGridViewResults->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::AutoSizeToAllHeaders;
			this->dataGridViewResults->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridViewResults->Size = System::Drawing::Size(556, 320);
			this->dataGridViewResults->TabIndex = 31;
			this->dataGridViewResults->CellContentDoubleClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &AppForm::DataGridViewResults_CellContentDoubleClick);
			// 
			// buttonSavePathsToFile
			// 
			this->buttonSavePathsToFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->buttonSavePathsToFile->Location = System::Drawing::Point(6, 677);
			this->buttonSavePathsToFile->Name = L"buttonSavePathsToFile";
			this->buttonSavePathsToFile->Size = System::Drawing::Size(134, 29);
			this->buttonSavePathsToFile->TabIndex = 29;
			this->buttonSavePathsToFile->Text = L"Сохранить";
			this->buttonSavePathsToFile->UseVisualStyleBackColor = true;
			this->buttonSavePathsToFile->Click += gcnew System::EventHandler(this, &AppForm::ButtonSavePathsToFile_Click);
			// 
			// labelPathsList
			// 
			this->labelPathsList->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelPathsList->AutoSize = true;
			this->labelPathsList->Location = System::Drawing::Point(6, 366);
			this->labelPathsList->Name = L"labelPathsList";
			this->labelPathsList->Size = System::Drawing::Size(75, 13);
			this->labelPathsList->TabIndex = 28;
			this->labelPathsList->Text = L"Список путей";
			// 
			// dataGridViewPaths
			// 
			this->dataGridViewPaths->AllowUserToAddRows = false;
			this->dataGridViewPaths->AllowUserToDeleteRows = false;
			this->dataGridViewPaths->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridViewPaths->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewPaths->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {
				this->Column1,
					this->Column2
			});
			this->dataGridViewPaths->Location = System::Drawing::Point(6, 384);
			this->dataGridViewPaths->Name = L"dataGridViewPaths";
			this->dataGridViewPaths->ReadOnly = true;
			this->dataGridViewPaths->Size = System::Drawing::Size(555, 287);
			this->dataGridViewPaths->TabIndex = 27;
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
			// comboBoxResAlgo
			// 
			this->comboBoxResAlgo->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->comboBoxResAlgo->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxResAlgo->FormattingEnabled = true;
			this->comboBoxResAlgo->Location = System::Drawing::Point(72, 16);
			this->comboBoxResAlgo->Name = L"comboBoxResAlgo";
			this->comboBoxResAlgo->Size = System::Drawing::Size(226, 21);
			this->comboBoxResAlgo->TabIndex = 25;
			this->comboBoxResAlgo->SelectedIndexChanged += gcnew System::EventHandler(this, &AppForm::ComboBoxResAlgo_SelectedIndexChanged);
			// 
			// labelChosenResAlgo
			// 
			this->labelChosenResAlgo->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->labelChosenResAlgo->AutoSize = true;
			this->labelChosenResAlgo->Location = System::Drawing::Point(6, 16);
			this->labelChosenResAlgo->Name = L"labelChosenResAlgo";
			this->labelChosenResAlgo->Size = System::Drawing::Size(59, 13);
			this->labelChosenResAlgo->TabIndex = 24;
			this->labelChosenResAlgo->Text = L"Алгоритм:";
			// 
			// AppForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1159, 718);
			this->Controls->Add(this->groupBoxResults);
			this->Controls->Add(this->tabControlLeft);
			this->Name = L"AppForm";
			this->Text = L"AppForm";
			this->tabControlLeft->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->groupBoxLaunches->ResumeLayout(false);
			this->groupBoxLaunches->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewParams))->EndInit();
			this->groupBoxLoadedDataSets->ResumeLayout(false);
			this->groupBoxLoadedDataSets->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewSelectedRes))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxRes))->EndInit();
			this->groupBoxResults->ResumeLayout(false);
			this->groupBoxResults->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewResults))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewPaths))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	void UpdateListViewLoaded() {
		for (int i = 0; i < this->listViewLoadedDataSets->Items->Count; ++i) {
			this->listViewLoadedDataSets->Items[i]->BackColor = Color::White;
		}
		if (listViewLoadedDataSets->SelectedIndices->Count == 1)
			this->listViewLoadedDataSets->SelectedItems[0]->BackColor = Color::LightBlue;
	}

	void UpdateRunButton() {
		if (listViewLoadedDataSets->SelectedIndices->Count == 1 && comboBoxLaunchAlgo->SelectedIndex >= 1) {
			this->buttonRun->Enabled = true;
		}
		else {
			this->buttonRun->Enabled = false;
		}
	}

	void UpdateDataGridViewResults() {
		int ind = this->comboBoxResAlgo->SelectedIndex;
		this->dataGridViewResults->Rows->Clear();
		this->dataGridViewResults->Columns->Clear();

		if (ind <= 0) {
			int colInd = 0;
			for (auto col : { "Номер датасета", "Алгоритм", "Минимизируемая величина", "Количество БПЛА",
				"Количество целей", "Ограничение дальности", "Сумма длин", "Макс длина" }) {
				this->dataGridViewResults->Columns->Add(ToText(colInd++), ToText((std::string)col));
			}

			this->dataGridViewResults->Columns->Add("launchInd", ToText("Номер запуска"));
			this->dataGridViewResults->Columns[this->dataGridViewResults->Columns->Count - 1]->Visible = false;

			for (auto& launch : AppFormVars::Launches) {
				int rowInd = this->dataGridViewResults->Rows->Count;
				this->dataGridViewResults->Rows->Add();
				this->dataGridViewResults->Rows[rowInd]->Cells[0]->Value = 
					ToText(launch.DataSetIndex + 1);
				this->dataGridViewResults->Rows[rowInd]->Cells[1]->Value = 
					ToText(AppFormVars::Algos[launch.AlgoIndex].Name);
				this->dataGridViewResults->Rows[rowInd]->Cells[2]->Value = 
					ToText(launch.Mode == ProblemMode::MINMAXLEN ? "Макс длина пути" : "Сумма длин путей");
				this->dataGridViewResults->Rows[rowInd]->Cells[3]->Value = 
					ToText(AppFormVars::DataSets[launch.DataSetIndex].DronsCnt);
				this->dataGridViewResults->Rows[rowInd]->Cells[4]->Value = 
					ToText(AppFormVars::DataSets[launch.DataSetIndex].TargetsCnt);
				this->dataGridViewResults->Rows[rowInd]->Cells[5]->Value = 
					ToText(AppFormVars::DataSets[launch.DataSetIndex].MaxDist);
				this->dataGridViewResults->Rows[rowInd]->Cells[6]->Value = 
					ToText(launch.Solution.SumOfPathLengths);
				this->dataGridViewResults->Rows[rowInd]->Cells[7]->Value = 
					ToText(launch.Solution.MaxPathLength);

				this->dataGridViewResults->Rows[rowInd]->Cells[8]->Value = ToText(rowInd);
			}
			return;
		}
		else {
			int colInd = 0;
			for (auto col : { "Номер датасета", "Минимизируемая величина", "Количество БПЛА",
				"Количество целей", "Ограничение дальности", "Сумма длин", "Макс длина" }) {
				this->dataGridViewResults->Columns->Add(ToText(colInd++), ToText((std::string)col));
			}
			for (auto param : AppFormVars::Algos[ind - 1].Params) {
				this->dataGridViewResults->Columns->Add(ToText(colInd++), ToText((std::string)param.Name));
			}

			this->dataGridViewResults->Columns->Add("launchInd", ToText("Номер запуска"));
			this->dataGridViewResults->Columns[this->dataGridViewResults->Columns->Count - 1]->Visible = false;

			int curLaunchInd = 0;
			for (auto& launch : AppFormVars::Launches) {
				curLaunchInd++;
				if (launch.AlgoIndex != ind - 1) continue;
				int rowInd = this->dataGridViewResults->Rows->Count;
				this->dataGridViewResults->Rows->Add();
				this->dataGridViewResults->Rows[rowInd]->Cells[0]->Value = 
					ToText(launch.DataSetIndex + 1);
				this->dataGridViewResults->Rows[rowInd]->Cells[1]->Value = 
					ToText(launch.Mode == ProblemMode::MINMAXLEN ? "Макс длина пути" : "Сумма длин путей");
				this->dataGridViewResults->Rows[rowInd]->Cells[2]->Value = 
					ToText(AppFormVars::DataSets[launch.DataSetIndex].DronsCnt);
				this->dataGridViewResults->Rows[rowInd]->Cells[3]->Value = 
					ToText(AppFormVars::DataSets[launch.DataSetIndex].TargetsCnt);
				this->dataGridViewResults->Rows[rowInd]->Cells[4]->Value = 
					ToText(AppFormVars::DataSets[launch.DataSetIndex].MaxDist);
				this->dataGridViewResults->Rows[rowInd]->Cells[5]->Value = 
					ToText(launch.Solution.SumOfPathLengths);
				this->dataGridViewResults->Rows[rowInd]->Cells[6]->Value = 
					ToText(launch.Solution.MaxPathLength);
				int curCol = 7;
				for (auto& param : launch.ParamsVals) {
					this->dataGridViewResults->Rows[rowInd]->Cells[curCol++]->Value = ToText(param);
				}

				this->dataGridViewResults->Rows[rowInd]->Cells[curCol++]->Value = ToText(curLaunchInd - 1);
			}
		}

		for (int i = 0; i < this->dataGridViewResults->Columns->Count; i++) {
			this->dataGridViewResults->Columns[i]->AutoSizeMode = DataGridViewAutoSizeColumnMode::DisplayedCells;
			int widthCol = this->dataGridViewResults->Columns[i]->Width - 15;
			this->dataGridViewResults->Columns[i]->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
			this->dataGridViewResults->Columns[i]->Width = widthCol;
		}

		this->dataGridViewResults->ClearSelection();
	}

	void UpdateDataGridViewPaths(ProblemSolution& solution) {
		while (this->dataGridViewPaths->Rows->Count < (int)solution.Paths.size())
			this->dataGridViewPaths->Rows->Add();
		while (this->dataGridViewPaths->Rows->Count > (int)solution.Paths.size())
			this->dataGridViewPaths->Rows->RemoveAt(this->dataGridViewPaths->Rows->Count - 1);

		int lineNum = 0;
		for (auto& path : solution.Paths) {
			double pathDistance = 0;
			if (path.size() > 0)
				pathDistance = solution.Input.Distance(0, path[0]) + solution.Input.Distance(0, path.back()); // from depot and to depot
			std::string pathStr = "";
			char buffer[(1 << 5)];
			for (size_t i = 0; i < path.size(); ++i) {
				int x = path[i];
				if (i + 1 < path.size())
					pathDistance += solution.Input.Distance(x, path[i + 1]);
				_itoa_s(x, buffer, 10);
				pathStr += ((std::string)buffer);
				pathStr += " ";
			}
			this->dataGridViewPaths->Rows[lineNum]->Cells[0]->Value = System::String(pathStr.c_str()).ToString();
			this->dataGridViewPaths->Rows[lineNum]->Cells[1]->Value = System::Double(pathDistance);
			this->dataGridViewPaths->Update();
			++lineNum;
		}

		this->dataGridViewPaths->ClearSelection();
	}

	// rowInd is clicked row in dataGridViewResults
	void UpdateVisualization(int launchInd) {
		this->dataGridViewSelectedRes->Rows->Clear();
		this->dataGridViewSelectedRes->Columns->Clear();

		int colInd = 0;
		for (auto col : { "Номер датасета", "Минимизируемая величина", "Количество БПЛА",
			"Количество целей", "Ограничение дальности", "Сумма длин", "Макс длина" }) {
			this->dataGridViewSelectedRes->Columns->Add(ToText(colInd++), ToText((std::string)col));
		}
		for (auto param : AppFormVars::Algos[AppFormVars::Launches[launchInd].AlgoIndex].Params) {
			this->dataGridViewSelectedRes->Columns->Add(ToText(colInd++), ToText((std::string)param.Name));
		}

		this->dataGridViewSelectedRes->Rows->Add();
		this->dataGridViewSelectedRes->Rows[0]->Cells[0]->Value = 
			ToText(AppFormVars::Launches[launchInd].DataSetIndex + 1);
		this->dataGridViewSelectedRes->Rows[0]->Cells[1]->Value = 
			ToText(AppFormVars::Launches[launchInd].Mode == 
				ProblemMode::MINMAXLEN ? "Макс длина пути" : "Сумма длин путей");
		this->dataGridViewSelectedRes->Rows[0]->Cells[2]->Value = 
			ToText(AppFormVars::DataSets[AppFormVars::Launches[launchInd].DataSetIndex].DronsCnt);
		this->dataGridViewSelectedRes->Rows[0]->Cells[3]->Value = 
			ToText(AppFormVars::DataSets[AppFormVars::Launches[launchInd].DataSetIndex].TargetsCnt);
		this->dataGridViewSelectedRes->Rows[0]->Cells[4]->Value = 
			ToText(AppFormVars::DataSets[AppFormVars::Launches[launchInd].DataSetIndex].MaxDist);
		this->dataGridViewSelectedRes->Rows[0]->Cells[5]->Value = 
			ToText(AppFormVars::Launches[launchInd].Solution.SumOfPathLengths);
		this->dataGridViewSelectedRes->Rows[0]->Cells[6]->Value = 
			ToText(AppFormVars::Launches[launchInd].Solution.MaxPathLength);
		int curCol = 7;
		for (auto& param : AppFormVars::Launches[launchInd].ParamsVals) {
			this->dataGridViewSelectedRes->Rows[0]->Cells[curCol++]->Value = ToText(param);
		}
		
		for (int i = 0; i < this->dataGridViewSelectedRes->Columns->Count; i++) {
			this->dataGridViewSelectedRes->Columns[i]->AutoSizeMode = DataGridViewAutoSizeColumnMode::DisplayedCells;
			int widthCol = this->dataGridViewSelectedRes->Columns[i]->Width - 15;
			this->dataGridViewSelectedRes->Columns[i]->AutoSizeMode = DataGridViewAutoSizeColumnMode::None;
			this->dataGridViewSelectedRes->Columns[i]->Width = widthCol;
		}

		
		this->tabControlLeft->SelectedTab = this->tabControlLeft->TabPages[1];
		this->tabControlLeft->Refresh();
		DrawPaths(Graphics, AppFormVars::Launches[launchInd].Solution, 
			this->pictureBoxRes->Height, this->pictureBoxRes->Width);
	}


	private: System::Void ButtonRun_Click(System::Object^ sender, System::EventArgs^ e) {
		int algoInd = this->comboBoxLaunchAlgo->SelectedIndex - 1;
		int dataSetInd = this->listViewLoadedDataSets->SelectedIndices[0];
		ProblemMode mode = ProblemMode::MINMAXLEN;
		if (this->radioButtonMinSum->Checked)
			mode = ProblemMode::MINSUM;
		std::vector<double> args;
		for (int i = 0; i < this->dataGridViewParams->Rows->Count; ++i) {
			try {
				std::string x = msclr::interop::marshal_as<std::string>(this->dataGridViewParams->Rows[i]->Cells[1]->Value->ToString());
				double z = atof(x.c_str());
				args.push_back(z);
			}
			catch(...) {
				MessageBox::Show("Не удалось считать параметр");
				args.push_back(AppFormVars::Algos[algoInd].Params[i].Recommend);
			}	
		}
		ProblemSolution solution = SolverMain::Run(AppFormVars::DataSets[dataSetInd], mode, AppFormVars::Algos[algoInd].Ealgo, args);

		if (!solution.SolutionExists) {
			MessageBox::Show("Не удалось найти решение");
		}
		else {
			AppFormVars::Launches.push_back(AppFormVars::Launch(algoInd, dataSetInd, args, mode, solution));
			UpdateDataGridViewPaths(AppFormVars::Launches.back().Solution);
			AppFormVars::PrintedLaunchPathsIndex = (int)AppFormVars::Launches.size() - 1;
			UpdateDataGridViewResults();
			this->buttonSavePathsToFile->Enabled = true;
		}
	}

	private: System::Void ButtonFileOpen_Click(System::Object^ sender, System::EventArgs^ e) {
		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
			return;
		}
		std::string fileName = msclr::interop::marshal_as<std::string>(this->openFileDialog->FileName);
		this->labelLoadedFile->Text = System::String(fileName.c_str()).ToString();

		AppFormVars::LoadedInputData = InputData(fileName, false);
		buttonUploadFile->Enabled = true;
	}

	private: System::Void ButtonUploadFile_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!AppFormVars::LoadedInputData.DronsCnt && !AppFormVars::LoadedInputData.TargetsCnt) {
			MessageBox::Show("Неверный формат файла. Требуемый формат:\n" +
				"КоличествоДронов КоличествоЦелей ДальностьПолёта\n Список пар координат");
		}
		else {
			AppFormVars::DataSets.push_back(AppFormVars::LoadedInputData);
			auto newitem = gcnew ListViewItem(ToText((int)AppFormVars::DataSets.size()));
			newitem->SubItems->Add(ToText(AppFormVars::DataSets.back().DronsCnt));
			newitem->SubItems->Add(ToText(AppFormVars::DataSets.back().TargetsCnt));
			newitem->SubItems->Add(ToText(AppFormVars::DataSets.back().MaxDist));
			listViewLoadedDataSets->Items->Add(newitem);
		}
		AppFormVars::LoadedInputData = InputData();
		this->labelLoadedFile->Text = "Файл не выбран";
		buttonUploadFile->Enabled = false;
		UpdateListViewLoaded();
	}

	private: System::Void ComboBoxLaunchAlgo_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int ind = comboBoxLaunchAlgo->SelectedIndex;
		dataGridViewParams->Rows->Clear();
		UpdateRunButton();
		if (ind > 0) {
			for (auto& param : AppFormVars::Algos[ind - 1].Params) {
				int i = dataGridViewParams->Rows->Count;
				dataGridViewParams->Rows->Add();
				dataGridViewParams->Rows[i]->Cells[0]->Value = ToText(param.Name);
				dataGridViewParams->Rows[i]->Cells[1]->Value = ToText(param.Recommend);
				dataGridViewParams->Rows[i]->Cells[2]->Value = ToText(param.Recommend);
				dataGridViewParams->Rows[i]->Cells[3]->Value = ToText(param.Comment);
			}
		}
		dataGridViewParams->Refresh();
	}

	private: System::Void dataGridViewParams_EditingControlShowing(System::Object^ sender, System::Windows::Forms::DataGridViewEditingControlShowingEventArgs^ e) {
		if (dataGridViewParams->CurrentCell->ColumnIndex == 1)
		{
			e->Control->KeyPress += gcnew KeyPressEventHandler(this, &AppForm::tb_KeyPress);
		}
	}

	private: void tb_KeyPress(System::Object^ sender, KeyPressEventArgs^ e) {
		auto s = (sender)->ToString();
		std::string tmps = msclr::interop::marshal_as<std::string>(s);
		int ind = tmps.find("Text");
		tmps = tmps.substr(ind, (int)tmps.size() - ind);
		if (tmps.find(".") != -1 && e->KeyChar == '.') {
			e->Handled = true;
		}
		else 
			if (!(Char::IsDigit(e->KeyChar)) && (e->KeyChar != '.')) {
			if (e->KeyChar != (char)Keys::Back) {
				e->Handled = true;
			}
		}
	}

	private: System::Void ListViewLoadedDataSets_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		UpdateRunButton();
		UpdateListViewLoaded();
	}

	private: System::Void ComboBoxResAlgo_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		UpdateDataGridViewResults();
	}

	private: System::Void ButtonSavePathsToFile_Click(System::Object^ sender, System::EventArgs^ e) {
		if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
			return;
		}
		std::string fileName = msclr::interop::marshal_as<std::string>(this->saveFileDialog->FileName);
		std::ofstream fout(fileName.c_str(), std::ios::out);
		std::string outputString;
		for (int i = 0; i < this->dataGridViewPaths->Rows->Count; ++i) {
			std::string curPath = msclr::interop::marshal_as<std::string>(this->dataGridViewPaths->Rows[i]->Cells[0]->Value->ToString());
			outputString += curPath;
			outputString += "\n";
		}

		fout << outputString;
		fout.close();
	}

	private: System::Void DataGridViewResults_CellContentDoubleClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
		if (e->RowIndex <= -1)
			return;
		std::string x = msclr::interop::marshal_as<std::string>(
			this->dataGridViewResults->Rows[e->RowIndex]->Cells[this->dataGridViewResults->Columns->Count - 1]->Value->ToString());
		int launchInd = atoi(x.c_str());
		UpdateVisualization(launchInd);
		UpdateDataGridViewPaths(AppFormVars::Launches[launchInd].Solution);
	}
};
}
