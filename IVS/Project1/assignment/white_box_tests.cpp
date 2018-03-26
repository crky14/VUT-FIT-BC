//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code->cpp
// $Author:     Jakub Crkoň <xcrkon00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Jakub Crkoň
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

class oneMatrix : public ::testing::Test{
public:

	Matrix *testedMatrix = NULL;

	virtual void SetUp(){
	};

	virtual void TearDown(){
		if(testedMatrix != NULL){
			delete testedMatrix;
		}
	}

};
class moreMatrixs : public ::testing::Test{
public:
	Matrix *testedMatrix_1 = NULL;
	Matrix *testedMatrix_2 = NULL;
	Matrix *testedMatrix_3 = NULL;

	virtual void SetUp(){
	};

	virtual void TearDown(){
		if(testedMatrix_1 != NULL){
			delete testedMatrix_1;
		}
		if(testedMatrix_2 != NULL){
			delete testedMatrix_2;
		}
		if(testedMatrix_3 != NULL){
			delete testedMatrix_3;
		}
	}
};


//Testuje prazdny konstruktor
TEST_F(oneMatrix, defaulConstructor){
	testedMatrix = new Matrix();
	EXPECT_NE((void*)NULL, testedMatrix);
}
//Testuje konstruktor ktory vytvara maticu vacsiu ako 1x1
TEST_F(oneMatrix, constructor){
	try{
	testedMatrix = new Matrix(0,0);
	}
	catch(std::runtime_error *err){
		EXPECT_NE((void*)NULL, err);
		delete err;
	}
	
	EXPECT_NO_THROW(testedMatrix = new Matrix(2,3));
}

//Testuje nastavenie jednej hodnoty v matici
TEST_F(oneMatrix, setOnevalue){
	testedMatrix = new Matrix();
	EXPECT_TRUE(testedMatrix->set(0,0,5));
	EXPECT_FALSE(testedMatrix->set(1,5,5));
}
//Testuje nastavenie hodnot v celej matici
TEST_F(oneMatrix, setAllvalues){
	testedMatrix = new Matrix(2,3);
	std::vector<std::vector< double > > smallValues(0,std::vector<double>(0,0));
	std::vector<std::vector< double > > goodValues(3,std::vector<double>(4,5));

	EXPECT_FALSE(testedMatrix->set(smallValues));
	EXPECT_TRUE(testedMatrix->set(goodValues));
}
//Test ziskania hodnoty z matice
TEST_F(oneMatrix, get){
	testedMatrix = new Matrix(2,3);
	testedMatrix->set(1,1,5);
	EXPECT_EQ(testedMatrix->get(1,1),5);
	EXPECT_TRUE(std::isnan(testedMatrix->get(5,5)));
}
//Test porovnania zhodnych matic
TEST_F(moreMatrixs, equalMatrix){
	testedMatrix_1 = new Matrix(2,5);
	testedMatrix_2 = new Matrix(2,5);
	EXPECT_TRUE((*testedMatrix_1) == (*testedMatrix_2));
}
//Test porovnania matic ktore nie su zhodne
TEST_F(moreMatrixs, notEqualMatrixThrow){
	testedMatrix_1 = new Matrix();
	testedMatrix_2 = new Matrix(1,2);
	try{
	(*testedMatrix_1) == (*testedMatrix_2);
	}
	catch(std::runtime_error *err){
		EXPECT_NE((void*)NULL, err);
		delete err;
	}
}

TEST_F(moreMatrixs, notEqualMatrix){
	testedMatrix_1 = new Matrix();
	testedMatrix_2 = new Matrix();
	testedMatrix_1->set(0,0,5);
	testedMatrix_2->set(0,0,1);
	EXPECT_FALSE((*testedMatrix_1) == (*testedMatrix_2));
	
}
//Test scitania 2 matic 1x1
TEST_F(moreMatrixs, add_1x1){
	testedMatrix_1 = new Matrix();
	testedMatrix_2 = new Matrix();
	testedMatrix_3 = new Matrix();

	testedMatrix_1->set(0,0,5);
	testedMatrix_2->set(0,0,1);
	testedMatrix_3->set(0,0,6);

	EXPECT_EQ((*testedMatrix_3), (*testedMatrix_1) + (*testedMatrix_2));
}
TEST_F(moreMatrixs, add_1x1_Throw){
	testedMatrix_1 = new Matrix();
	testedMatrix_2 = new Matrix(2,5);

	try{
	(*testedMatrix_1) + (*testedMatrix_2);
	}
	catch(std::runtime_error *err){
		EXPECT_NE((void*)NULL, err);
		delete err;
	}

}

//Test nasobenia 2 matic 1x1
TEST_F(moreMatrixs, multiply_1x1){
	testedMatrix_1 = new Matrix();
	testedMatrix_2 = new Matrix();
	testedMatrix_3 = new Matrix();

	testedMatrix_1->set(0,0,5);
	testedMatrix_2->set(0,0,1);
	testedMatrix_3->set(0,0,5);

	EXPECT_EQ((*testedMatrix_3), (*testedMatrix_1) * (*testedMatrix_2));
}
TEST_F(moreMatrixs, multiply_1x1_Throw){
	testedMatrix_1 = new Matrix();
	testedMatrix_2 = new Matrix(2,5);

	try{
	(*testedMatrix_1) * (*testedMatrix_2);
	}
	catch(std::runtime_error *err){
		EXPECT_NE((void*)NULL, err);
		delete err;
	}
}

//Test scitania 2 matic 
TEST_F(moreMatrixs, Add){
	testedMatrix_1 = new Matrix(1,2);
	testedMatrix_2 = new Matrix(1,2);
	testedMatrix_3 = new Matrix(1,2);

	testedMatrix_1->set(0,0,3);
	testedMatrix_1->set(0,1,4);
	testedMatrix_2->set(0,0,5);
	testedMatrix_2->set(0,1,6);

	testedMatrix_3->set(0,0,8);
	testedMatrix_3->set(0,1,10);
	EXPECT_EQ((*testedMatrix_3), (*testedMatrix_1) + (*testedMatrix_2));
}
//Testuje nasobenie 2 matic 
TEST_F(moreMatrixs, multiply){
	testedMatrix_1 = new Matrix(2,2);
	testedMatrix_2 = new Matrix(2,2);
	testedMatrix_3 = new Matrix(2,2);
	
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			testedMatrix_1->set(i,j,i+j);
			testedMatrix_2->set(i,j,i+j);
		}
	testedMatrix_3->set(0,0,1);
	testedMatrix_3->set(0,1,2);
	testedMatrix_3->set(1,0,2);
	testedMatrix_3->set(1,1,5);	
	}
	EXPECT_EQ((*testedMatrix_3), (*testedMatrix_1) * (*testedMatrix_2));
}
//Test nasobenia matice konstantov
TEST_F(moreMatrixs, multiplyByConst){
	testedMatrix_1 = new Matrix(2,2);
	testedMatrix_2 = new Matrix(2,2);

	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			testedMatrix_1->set(i,j,i+j);
			testedMatrix_2->set(i,j,(i+j)*5);
		}
	}
	EXPECT_EQ((*testedMatrix_2), (*testedMatrix_1) * 5);	
}
//Test chyb pri rieseni matic
TEST_F(moreMatrixs, solveThrows){
	testedMatrix_1 = new Matrix(2,5);
	testedMatrix_2 = new Matrix(3,2);
	testedMatrix_3 = new Matrix(2,2);
	std::vector<double> rightSide(2,5);
	//prava strana neodpoveda riadkom matice
	try{
	testedMatrix_1->solveEquation(rightSide);
	}
	catch(std::runtime_error *err){
		EXPECT_NE((void*)NULL, err);
		delete err;
	}
	//matica nie je stvrcova
	try{
	testedMatrix_2->solveEquation(rightSide);
	}
	catch(std::runtime_error *err){
		EXPECT_NE((void*)NULL, err);
		delete err;
	}
	// singularna matica
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			if(i == 0){
				testedMatrix_3->set(i,j,0);
			}
			else{
				testedMatrix_3->set(i,j,i+j);
			}
		}
	}

	try{
	testedMatrix_3->solveEquation(rightSide);
	}
	catch(std::runtime_error *err){
		EXPECT_NE((void*)NULL, err);
		delete err;
	}
	
}
//Test vyriesenia matice 1x1

TEST_F(oneMatrix, solve_1x1){
	testedMatrix = new Matrix();
	testedMatrix->set(0,0,5);
	std::vector<double> rightSide;
	std::vector<double> solution;
	rightSide.push_back(5);
	solution.push_back(1);

	EXPECT_EQ(solution, testedMatrix->solveEquation(rightSide));
}
//Test vyriesenia matice 2x2
TEST_F(oneMatrix, solve_2x2){
	testedMatrix = new Matrix(2,2);
	std::vector<double> rightSide;
	std::vector<double> solution;
	int count = 2;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			testedMatrix->set(i,j,count);
			count++;
		}
	}
	rightSide.push_back(12);
	rightSide.push_back(10);
	solution.push_back(-15);
	solution.push_back(14);

	EXPECT_EQ(solution, testedMatrix->solveEquation(rightSide));

}
//Matica 2x2 so zapornymi hodnotami
TEST_F(oneMatrix, solve_2x2_negative){
	testedMatrix = new Matrix(2,2);
	std::vector<double> rightSide;
	std::vector<double> solution;
	int count = -5;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			testedMatrix->set(i,j,count);
			count--;
		}
	}
	rightSide.push_back(12);
	rightSide.push_back(20);
	solution.push_back(-12);
	solution.push_back(8);
	EXPECT_EQ(solution, testedMatrix->solveEquation(rightSide));
}
//Matica 2x2 ktorej vysledok je float
TEST_F(oneMatrix, solve_2x2_float){
	testedMatrix = new Matrix(2,2);
	std::vector<double> rightSide;
	std::vector<double> solution;
	int count = 2;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			testedMatrix->set(i,j,count);
			count--;
		}
	}
	rightSide.push_back(12);
	rightSide.push_back(25);
	solution.push_back((float)15/(float)2);
	solution.push_back(-1);
	EXPECT_EQ(solution, testedMatrix->solveEquation(rightSide));
}

// Testuje vyriesenie matice 3x3
TEST_F(oneMatrix, solve_3x3){
	testedMatrix = new Matrix(3,3);
	std::vector<double> rightSide(3,5);
	std::vector<double> solution;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			testedMatrix->set(i,j,i+j+1);
		}
	}
	testedMatrix->set(0,0,0);
	solution.push_back(0);
	solution.push_back(-5);
	solution.push_back(5);
	EXPECT_EQ(solution, testedMatrix->solveEquation(rightSide));
}
//Test matice 3x3 ktorej vysledok je float
TEST_F(oneMatrix, solve_3x3_float){
	testedMatrix = new Matrix(3,3);
	float values[9] = {-2,-5,-6,-7,-3,4,5,2,3};
	std::vector<double> rightSide;
	rightSide.push_back(5);
	rightSide.push_back(3);
	rightSide.push_back(2);
	std::vector<double> solution;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			testedMatrix->set(i,j,values[i+j]);
		}
	}
	solution.push_back((float)152 / (float)177);
	solution.push_back((float)-377 / (float)177);
	solution.push_back((float)116 / (float)177);
	EXPECT_EQ(solution, testedMatrix->solveEquation(rightSide));
}
//Test matice vacsej ako 4x4
TEST_F(oneMatrix, solve){
	testedMatrix = new Matrix(4,4);
	std::vector<double> rightSide(4,5);
	std::vector<double> solution;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			testedMatrix->set(i,j,i+j+1);
		}
	}
	testedMatrix->set(0,0,0);
	testedMatrix->set(0,1,0);
	testedMatrix->set(1,0,0);
	solution.push_back(0);
	solution.push_back(0);
	solution.push_back(-5);
	solution.push_back(5);

	EXPECT_EQ(solution, testedMatrix->solveEquation(rightSide));
}
//Test matice vacsej ako 4x4 ktorej vysledok je float
TEST_F(oneMatrix, solve_float){
	testedMatrix = new Matrix(4,4);
	float values[16] = {-2,-5,-6,-7,-3,-4,-5,-2,3,10,5,9,6,18,1,14};
	std::vector<double> rightSide;
	std::vector<double> solution;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			testedMatrix->set(i,j,values[i+j]);
		}
	}
	rightSide.push_back(4);
	rightSide.push_back(2);
	rightSide.push_back(3);
	rightSide.push_back(2);
	solution.push_back((float)-1906 / (float)397);
	solution.push_back((float)1333 / (float)397);
	solution.push_back((float)262 / (float)397);
	solution.push_back((float)-859 / (float)397);

	EXPECT_EQ(solution, testedMatrix->solveEquation(rightSide));
}
/*** Konec souboru white_box_tests.cpp ***/
