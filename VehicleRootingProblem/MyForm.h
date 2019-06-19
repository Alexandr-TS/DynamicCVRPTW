#pragma once

#include "Optimizations.h"
#include "SolverClarkeWright.h"
#include "SolverDP.h"
#include "SolverGreedy.h"
#include "SolverGreedyClusterized.h"
#include "SolverMain.h"
#include "SolverAntColony.h"
#include "Utils.h"
#include "Visualization.h"
#include "SolverGenetic.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <msclr\marshal_cppstd.h>

namespace VehicleRootingProblem {

    bool localDebug = true;

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MyForm : public System::Windows::Forms::Form {
    public:
        MyForm(void) {
            InitializeComponent();
            if (localDebug) {
                this->Width = 1800;
                this->Height = 740;
                this->textBoxResult->Width = 300;
                this->pictureBox1->Visible = true;
                this->textBox1->Visible = true;
                this->progressBar1->Visible = true;
                this->button1->Visible = true;
				this->pictureBox1->BringToFront();
			} 
			this->cbAlgorithm->SelectedIndex = 0;

			if (localDebug) {
				AllocConsole();
				freopen("CONOUT$", "w", stdout);
			}
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~MyForm() {
            if (components) {
                delete components;
            }
        }
    private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::ProgressBar^  progressBar1;
    private: System::Windows::Forms::TextBox^  textBox1;
    private: System::Windows::Forms::PictureBox^  pictureBox1;
    private: System::Windows::Forms::TextBox^  textBoxMatrix;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::Button^  buttonRun;
    private: System::Windows::Forms::RadioButton^  radioButtonMinMaxLen;
    private: System::Windows::Forms::RadioButton^  radioButtonMinSum;
    private: System::Windows::Forms::TextBox^  textBoxMaxDist;
    private: System::Windows::Forms::TextBox^  textBoxCntTargets;
    private: System::Windows::Forms::TextBox^  textBoxCntDrons;
    private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  labelFileName;
    private: System::Windows::Forms::Button^  buttonFileOpen;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
    private: System::Windows::Forms::GroupBox^  groupBox2;
    private: System::Windows::Forms::Button^  buttonSave;
    private: System::Windows::Forms::TextBox^  textBoxResult;
    private: System::Windows::Forms::TextBox^  textBoxResMaxLen;

    private: System::Windows::Forms::Label^  label8;
    private: System::Windows::Forms::TextBox^  textBoxResSumLen;

    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
    private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::ComboBox^  cbAlgorithm;
	private: System::Windows::Forms::DataGridView^  dataGridViewResult;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;










    protected:

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void) {
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->textBoxMatrix = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->cbAlgorithm = (gcnew System::Windows::Forms::ComboBox());
			this->buttonRun = (gcnew System::Windows::Forms::Button());
			this->radioButtonMinMaxLen = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonMinSum = (gcnew System::Windows::Forms::RadioButton());
			this->textBoxMaxDist = (gcnew System::Windows::Forms::TextBox());
			this->textBoxCntTargets = (gcnew System::Windows::Forms::TextBox());
			this->textBoxCntDrons = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->labelFileName = (gcnew System::Windows::Forms::Label());
			this->buttonFileOpen = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->dataGridViewResult = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->buttonSave = (gcnew System::Windows::Forms::Button());
			this->textBoxResult = (gcnew System::Windows::Forms::TextBox());
			this->textBoxResMaxLen = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->textBoxResSumLen = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewResult))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(523, 555);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(127, 68);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Run";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Visible = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(240, 546);
			this->progressBar1->Margin = System::Windows::Forms::Padding(2);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(270, 42);
			this->progressBar1->TabIndex = 2;
			this->progressBar1->Visible = false;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(655, 555);
			this->textBox1->Margin = System::Windows::Forms::Padding(2);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(120, 20);
			this->textBox1->TabIndex = 3;
			this->textBox1->Visible = false;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(0, 17);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(541, 518);
			this->pictureBox1->TabIndex = 4;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Visible = false;
			// 
			// textBoxMatrix
			// 
			this->textBoxMatrix->Location = System::Drawing::Point(8, 113);
			this->textBoxMatrix->Margin = System::Windows::Forms::Padding(2);
			this->textBoxMatrix->MaxLength = 527670;
			this->textBoxMatrix->Multiline = true;
			this->textBoxMatrix->Name = L"textBoxMatrix";
			this->textBoxMatrix->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBoxMatrix->Size = System::Drawing::Size(486, 324);
			this->textBoxMatrix->TabIndex = 5;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label10);
			this->groupBox1->Controls->Add(this->cbAlgorithm);
			this->groupBox1->Controls->Add(this->buttonRun);
			this->groupBox1->Controls->Add(this->radioButtonMinMaxLen);
			this->groupBox1->Controls->Add(this->radioButtonMinSum);
			this->groupBox1->Controls->Add(this->textBoxMaxDist);
			this->groupBox1->Controls->Add(this->textBoxCntTargets);
			this->groupBox1->Controls->Add(this->textBoxCntDrons);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->labelFileName);
			this->groupBox1->Controls->Add(this->buttonFileOpen);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->textBoxMatrix);
			this->groupBox1->Location = System::Drawing::Point(9, 11);
			this->groupBox1->Margin = System::Windows::Forms::Padding(2);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(2);
			this->groupBox1->Size = System::Drawing::Size(498, 543);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Входные данные";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(6, 472);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(59, 13);
			this->label10->TabIndex = 21;
			this->label10->Text = L"Алгоритм:";
			// 
			// cbAlgorithm
			// 
			this->cbAlgorithm->FormattingEnabled = true;
			this->cbAlgorithm->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Точный", L"Приближенный" });
			this->cbAlgorithm->Location = System::Drawing::Point(120, 469);
			this->cbAlgorithm->Name = L"cbAlgorithm";
			this->cbAlgorithm->Size = System::Drawing::Size(162, 21);
			this->cbAlgorithm->TabIndex = 20;
			// 
			// buttonRun
			// 
			this->buttonRun->Location = System::Drawing::Point(338, 485);
			this->buttonRun->Margin = System::Windows::Forms::Padding(2);
			this->buttonRun->Name = L"buttonRun";
			this->buttonRun->Size = System::Drawing::Size(145, 39);
			this->buttonRun->TabIndex = 19;
			this->buttonRun->Text = L"Запустить";
			this->buttonRun->UseVisualStyleBackColor = true;
			this->buttonRun->Click += gcnew System::EventHandler(this, &MyForm::buttonRun_Click);
			// 
			// radioButtonMinMaxLen
			// 
			this->radioButtonMinMaxLen->AutoSize = true;
			this->radioButtonMinMaxLen->Location = System::Drawing::Point(8, 518);
			this->radioButtonMinMaxLen->Margin = System::Windows::Forms::Padding(2);
			this->radioButtonMinMaxLen->Name = L"radioButtonMinMaxLen";
			this->radioButtonMinMaxLen->Size = System::Drawing::Size(148, 17);
			this->radioButtonMinMaxLen->TabIndex = 18;
			this->radioButtonMinMaxLen->Text = L"Минимизировать время";
			this->radioButtonMinMaxLen->UseVisualStyleBackColor = true;
			// 
			// radioButtonMinSum
			// 
			this->radioButtonMinSum->AutoSize = true;
			this->radioButtonMinSum->Checked = true;
			this->radioButtonMinSum->Location = System::Drawing::Point(8, 496);
			this->radioButtonMinSum->Margin = System::Windows::Forms::Padding(2);
			this->radioButtonMinSum->Name = L"radioButtonMinSum";
			this->radioButtonMinSum->Size = System::Drawing::Size(206, 17);
			this->radioButtonMinSum->TabIndex = 17;
			this->radioButtonMinSum->TabStop = true;
			this->radioButtonMinSum->Text = L"Минимизировать суммарную длину";
			this->radioButtonMinSum->UseVisualStyleBackColor = true;
			// 
			// textBoxMaxDist
			// 
			this->textBoxMaxDist->Location = System::Drawing::Point(281, 66);
			this->textBoxMaxDist->Margin = System::Windows::Forms::Padding(2);
			this->textBoxMaxDist->Name = L"textBoxMaxDist";
			this->textBoxMaxDist->Size = System::Drawing::Size(121, 20);
			this->textBoxMaxDist->TabIndex = 16;
			// 
			// textBoxCntTargets
			// 
			this->textBoxCntTargets->Location = System::Drawing::Point(281, 41);
			this->textBoxCntTargets->Margin = System::Windows::Forms::Padding(2);
			this->textBoxCntTargets->Name = L"textBoxCntTargets";
			this->textBoxCntTargets->Size = System::Drawing::Size(121, 20);
			this->textBoxCntTargets->TabIndex = 15;
			this->textBoxCntTargets->TextChanged += gcnew System::EventHandler(this, &MyForm::textBoxCntTargets_TextChanged);
			// 
			// textBoxCntDrons
			// 
			this->textBoxCntDrons->Location = System::Drawing::Point(281, 17);
			this->textBoxCntDrons->Margin = System::Windows::Forms::Padding(2);
			this->textBoxCntDrons->Name = L"textBoxCntDrons";
			this->textBoxCntDrons->Size = System::Drawing::Size(121, 20);
			this->textBoxCntDrons->TabIndex = 14;
			this->textBoxCntDrons->TextChanged += gcnew System::EventHandler(this, &MyForm::textBoxCntDrons_TextChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(101, 68);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(181, 13);
			this->label7->TabIndex = 13;
			this->label7->Text = L"Максимальная дальность полёта:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(101, 44);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(102, 13);
			this->label6->TabIndex = 12;
			this->label6->Text = L"Количество целей:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(101, 20);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(108, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Количество дронов:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(101, 93);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(116, 13);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Матрица расстояний:";
			// 
			// labelFileName
			// 
			this->labelFileName->AutoSize = true;
			this->labelFileName->Location = System::Drawing::Point(229, 445);
			this->labelFileName->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->labelFileName->Name = L"labelFileName";
			this->labelFileName->Size = System::Drawing::Size(92, 13);
			this->labelFileName->TabIndex = 9;
			this->labelFileName->Text = L"Файл не выбран";
			// 
			// buttonFileOpen
			// 
			this->buttonFileOpen->Location = System::Drawing::Point(120, 441);
			this->buttonFileOpen->Margin = System::Windows::Forms::Padding(2);
			this->buttonFileOpen->Name = L"buttonFileOpen";
			this->buttonFileOpen->Size = System::Drawing::Size(104, 22);
			this->buttonFileOpen->TabIndex = 8;
			this->buttonFileOpen->Text = L"Выберите файл";
			this->buttonFileOpen->UseVisualStyleBackColor = true;
			this->buttonFileOpen->Click += gcnew System::EventHandler(this, &MyForm::buttonFileOpen_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 445);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(113, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Или загрузите файл:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 20);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(93, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Введите данные:";
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->dataGridViewResult);
			this->groupBox2->Controls->Add(this->label9);
			this->groupBox2->Controls->Add(this->buttonSave);
			this->groupBox2->Controls->Add(this->textBoxResult);
			this->groupBox2->Controls->Add(this->textBoxResMaxLen);
			this->groupBox2->Controls->Add(this->label8);
			this->groupBox2->Controls->Add(this->textBoxResSumLen);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->pictureBox1);
			this->groupBox2->Location = System::Drawing::Point(512, 11);
			this->groupBox2->Margin = System::Windows::Forms::Padding(2);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Padding = System::Windows::Forms::Padding(2);
			this->groupBox2->Size = System::Drawing::Size(541, 543);
			this->groupBox2->TabIndex = 7;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Результат";
			// 
			// dataGridViewResult
			// 
			this->dataGridViewResult->AllowUserToAddRows = false;
			this->dataGridViewResult->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridViewResult->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {
				this->Column1,
					this->Column2
			});
			this->dataGridViewResult->Location = System::Drawing::Point(8, 113);
			this->dataGridViewResult->Name = L"dataGridViewResult";
			this->dataGridViewResult->ReadOnly = true;
			this->dataGridViewResult->Size = System::Drawing::Size(528, 324);
			this->dataGridViewResult->TabIndex = 26;
			// 
			// Column1
			// 
			this->Column1->Frozen = true;
			this->Column1->HeaderText = L"Пути";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column1->Width = 325;
			// 
			// Column2
			// 
			this->Column2->Frozen = true;
			this->Column2->HeaderText = L"Длины";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->Width = 155;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(9, 76);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(78, 13);
			this->label9->TabIndex = 24;
			this->label9->Text = L"Список путей:";
			// 
			// buttonSave
			// 
			this->buttonSave->Location = System::Drawing::Point(11, 449);
			this->buttonSave->Margin = System::Windows::Forms::Padding(2);
			this->buttonSave->Name = L"buttonSave";
			this->buttonSave->Size = System::Drawing::Size(110, 22);
			this->buttonSave->TabIndex = 20;
			this->buttonSave->Text = L"Сохранить в файл";
			this->buttonSave->UseVisualStyleBackColor = true;
			this->buttonSave->Click += gcnew System::EventHandler(this, &MyForm::buttonSave_Click);
			// 
			// textBoxResult
			// 
			this->textBoxResult->Enabled = false;
			this->textBoxResult->Location = System::Drawing::Point(8, 113);
			this->textBoxResult->Margin = System::Windows::Forms::Padding(2);
			this->textBoxResult->Multiline = true;
			this->textBoxResult->Name = L"textBoxResult";
			this->textBoxResult->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBoxResult->Size = System::Drawing::Size(529, 324);
			this->textBoxResult->TabIndex = 20;
			// 
			// textBoxResMaxLen
			// 
			this->textBoxResMaxLen->Location = System::Drawing::Point(133, 48);
			this->textBoxResMaxLen->Margin = System::Windows::Forms::Padding(2);
			this->textBoxResMaxLen->Name = L"textBoxResMaxLen";
			this->textBoxResMaxLen->Size = System::Drawing::Size(121, 20);
			this->textBoxResMaxLen->TabIndex = 23;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(9, 26);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(101, 13);
			this->label8->TabIndex = 20;
			this->label8->Text = L"Суммарная длина:";
			// 
			// textBoxResSumLen
			// 
			this->textBoxResSumLen->Location = System::Drawing::Point(133, 24);
			this->textBoxResSumLen->Margin = System::Windows::Forms::Padding(2);
			this->textBoxResSumLen->Name = L"textBoxResSumLen";
			this->textBoxResSumLen->Size = System::Drawing::Size(121, 20);
			this->textBoxResSumLen->TabIndex = 22;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(9, 50);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(117, 13);
			this->label3->TabIndex = 21;
			this->label3->Text = L"Наидлиннейший путь:";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1057, 561);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"Маршрутизация транспорта";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewResult))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
        //auto input = InputData("input.txt", false);

        std::ofstream fout("output.txt", std::ios::out);
        auto startTime = clock();

        DataGenerator generator;
        int tests = 1;
        this->progressBar1->Maximum = tests;
        this->progressBar1->Value = 0;
        this->progressBar1->Step = 1;
        auto probMode = ProblemMode::MINMAXLEN;
        if (this->radioButtonMinSum->Checked) {
            probMode = ProblemMode::MINSUM;
        }

		//probMode = ProblemMode::MINSUM;

		std::vector<ProblemSolution> solutions;
        for (int i = 0; i < tests; ++i) {
            int t = 100;

			//auto input = generator.GenInputData(1, t / 5, 1, t, 100, 3000, -100, 100);
			auto input = generator.GenInputData(1, t / 5, t - 1, t, 100, 500, -100, 100);
			//auto input = generator.GenInputData(2, 2, 2, 2, 1000, 3000, -100, 100);

			auto sol1 = SolverGenetic::Run(input, probMode, {30, 3000, 1000, 0.5, 0.5, 0});

			//auto sol2 = SolverAntColony::Run(input, probMode, { 5, 5, 0.5, 2, 3, 3, 5, 1, 0 });
			solutions.push_back(sol1);
			//solutions.push_back(sol2);


            fout << "drons: " << input.DronsCnt << ", targets: " << input.TargetsCnt << std::endl;
			for (auto x : solutions) {
				if (!x.SolutionExists) {
					fout << "No \n";
				} else {
					if (probMode == ProblemMode::MINSUM)
						fout << x.SumOfPathLengths << " " << x.Paths.size() << "\n";
					else
						fout << x.MaxPathLength << " " << x.Paths.size() << "\n";
				}
			}
            fout << "maxdist: " << input.MaxDist << std::endl << std::endl << std::endl;
            this->progressBar1->PerformStep();

			this->textBoxCntDrons->Text = input.DronsCnt.ToString();
			this->textBoxCntTargets->Text = input.TargetsCnt.ToString();
        }

        double timeL = ((double)clock() - startTime) / CLOCKS_PER_SEC;

        this->textBox1->Text = System::Double(timeL).ToString();
        auto graphics = this->pictureBox1->CreateGraphics();
        DrawPaths(graphics, solutions.back(), this->pictureBox1->Height, this->pictureBox1->Width);
    }

    private: System::Void buttonSave_Click(System::Object^  sender, System::EventArgs^  e) {
        if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
            return;
        }
        std::string fileName = msclr::interop::marshal_as<std::string>(this->saveFileDialog1->FileName);
        std::ofstream fout(fileName.c_str(), std::ios::out);
        std::string outputString = msclr::interop::marshal_as<std::string>(this->textBoxResult->Text);
        for (int i = 0; i + 1 < (int)outputString.size(); ++i)
            while (i + 1 < (int)outputString.size() && !isdigit(outputString[i]) && outputString[i] != '.' &&
                !isdigit(outputString[i + 1]) && outputString[i + 1] != '.' && outputString[i + 1] != '\n')
                outputString.erase(outputString.begin() + i + 1, outputString.begin() + i + 2);
        fout << outputString;
        fout.close();
    }

    private: System::Void buttonFileOpen_Click(System::Object^  sender, System::EventArgs^  e) {
        this->labelFileName->Text = "Файл не выбран";
		this->textBoxCntDrons->Text = "";
		this->textBoxCntTargets->Text = "";
		this->textBoxMaxDist->Text = "";
        delete formInputData;
        formInputData = nullptr;
        if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) {
            return;
        }
        std::string fileName = msclr::interop::marshal_as<std::string>(this->openFileDialog1->FileName);
        this->labelFileName->Text = System::String(fileName.c_str()).ToString();
        if (formInputData)
            delete formInputData;
        formInputData = new InputData(fileName);
		if (!formInputData->DronsCnt && !formInputData->TargetsCnt) {
			MessageBox::Show("Неверный формат файла. Требуемый формат:\n" +
				"КоличествоДронов КоличествоЦелей ДальностьПолёта\n МатрицаРасстояний");
			formInputData = nullptr;
		}
    }

	private: InputData* formInputData = nullptr;
    private: ProblemMode formProblemMode = ProblemMode::MINMAXLEN;

    private: System::Void buttonRun_Click(System::Object^  sender, System::EventArgs^  e) {
        if (this->radioButtonMinMaxLen->Checked) {
            formProblemMode = ProblemMode::MINMAXLEN;
        } else {
            formProblemMode = ProblemMode::MINSUM;
        }

        bool fromFile = false;

        if (this->textBoxCntDrons->Text->Length == 0 || this->textBoxCntTargets->Text->Length == 0
            || this->textBoxMaxDist->Text->Length == 0) {
            fromFile = true;
        }

        if (!fromFile) {
            int cntDrons = atoi(msclr::interop::marshal_as<std::string>(this->textBoxCntDrons->Text).c_str());
            int cntTargets = atoi(msclr::interop::marshal_as<std::string>(this->textBoxCntTargets->Text).c_str());
            double maxDist = atof(msclr::interop::marshal_as<std::string>(this->textBoxMaxDist->Text).c_str());
            MatrixDouble matrix;
            std::string matrixStr = msclr::interop::marshal_as<std::string>(this->textBoxMatrix->Text) + " ";
            std::vector<double> matrix1;
            std::string cur;
            for (int i = 0; i < (int)matrixStr.size(); ++i) {
                if (!isdigit(matrixStr[i]) && matrixStr[i] != '.') {
                    if (cur.size()) {
                        matrix1.push_back(atof(cur.c_str()));
                        cur = "";
                    }
                } else {
                    cur += matrixStr[i];
                }
            }

            int sz = matrix1.size();

            if (sz != (cntTargets + 1) * (cntTargets + 1)) {
                auto sizeStr = System::Int32(cntTargets + 1).ToString();
                MessageBox::Show("Матрица должна иметь размер " + sizeStr + "x" + sizeStr);
                return;
            }

            matrix.resize(cntTargets + 1);

            for (int i = 0; i < (int)matrix1.size(); ++i) {
                int lineNum = i / (cntTargets + 1);
                assert(lineNum < (int)matrix.size());
                matrix[lineNum].push_back(matrix1[i]);
            }
            if (formInputData)
                delete formInputData;
            formInputData = new InputData(cntDrons, cntTargets, maxDist, matrix);
        }

        if (!formInputData) {
            MessageBox::Show("Загрузите файл или введите все данные в форму");
            return;
        }

		auto solution = SolverMain::Run(*formInputData, formProblemMode, (EAlgorithms)this->cbAlgorithm->SelectedIndex, {});

        this->textBoxResMaxLen->Text = System::Double(solution.MaxPathLength).ToString();
        this->textBoxResSumLen->Text = System::Double(solution.SumOfPathLengths).ToString();
        this->textBoxResult->Clear();

        if (!solution.SolutionExists) {
            this->textBoxResult->Text = System::String("Решение не найдено").ToString();
			MessageBox::Show("Решение не найдено");
            return;
        }

		while (this->dataGridViewResult->Rows->Count < (int)solution.Paths.size())
			this->dataGridViewResult->Rows->Add();
		while (this->dataGridViewResult->Rows->Count > (int)solution.Paths.size())
			this->dataGridViewResult->Rows->RemoveAt(this->dataGridViewResult->Rows->Count - 1);

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
                this->textBoxResult->Text += System::Double(x).ToString() + System::String(" ").ToString();
				_itoa(x, buffer, 10);
				pathStr += ((std::string)buffer);
				pathStr += " ";
            }
			this->dataGridViewResult->Rows[lineNum]->Cells[0]->Value = System::String(pathStr.c_str()).ToString();
			this->dataGridViewResult->Rows[lineNum]->Cells[1]->Value = System::Double(pathDistance);
			this->dataGridViewResult->Update();
            this->textBoxResult->Text += Environment::NewLine;
			++lineNum;
        }
    }

	private: void updateChosenAlgorithm() {
		int cntDrons = atoi(msclr::interop::marshal_as<std::string>(this->textBoxCntDrons->Text).c_str());
		int cntTargets = atoi(msclr::interop::marshal_as<std::string>(this->textBoxCntTargets->Text).c_str());
		if (pow(3, cntTargets) * cntDrons < 1e7)
			this->cbAlgorithm->SelectedIndex = 0;
		else
			this->cbAlgorithm->SelectedIndex = 1;
	}

	private: System::Void textBoxCntTargets_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		updateChosenAlgorithm();
	}
	private: System::Void textBoxCntDrons_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		updateChosenAlgorithm();
	}
};
}
