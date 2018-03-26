//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Jakub Crkoň <xcrkon00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Jakub Crkoň
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

#include "limits.h"

//
class emptyBinaryTreeTest : public ::testing::Test{	
public:
	BinaryTree *TestedTree;
	std::pair<bool, BinaryTree::Node_t *> insRetVal;

	virtual void SetUp(){
		TestedTree = new BinaryTree();
	};

	virtual void TearDown(){
		delete TestedTree;
	}
};

class BinaryTreeTest : public ::testing::Test{	
public:
	BinaryTree *TestedTree;
	std::pair<bool, BinaryTree::Node_t *> insRetVal;
	std::pair<bool, BinaryTree::Node_t *> cmpInsRetVal;
	int values[10] = {INT_MAX,42,INT_MIN,12,-10,-20,100,80,90,70};

	virtual void SetUp(){
		TestedTree = new BinaryTree();

		for(int i = 0; i < 10; i++){
			if(i == 3){
				cmpInsRetVal = TestedTree->InsertNode(values[i]);
			}
			TestedTree->InsertNode(values[i]);
		}
	};

	virtual void TearDown(){
		delete TestedTree;
	}
};

class AxiomsTest : public ::testing::Test{	
public:
	BinaryTree *TestedTree;
	std::vector<BinaryTree::Node_t *> outLeafNodes;
	std::vector<BinaryTree::Node_t *> outAllNodes;
	


	virtual void SetUp(){
		TestedTree = new BinaryTree();
		TestedTree->GetLeafNodes(outLeafNodes);
		TestedTree->GetAllNodes(outAllNodes);
	};

	virtual void TearDown(){
		delete TestedTree;
	}
};

// testuje ci je strom na zaciatku testov prazdny
TEST_F(emptyBinaryTreeTest, isEmpty){

	EXPECT_EQ(NULL, TestedTree->GetRoot());
}
//tetuje vlozenie noveho prvku s kladnou hodnotou
TEST_F(emptyBinaryTreeTest, InsertPositiveNode){

	insRetVal = TestedTree->InsertNode(42);
	EXPECT_NE((void*) NULL, insRetVal.second);
	EXPECT_TRUE(insRetVal.first);
}
//testuje vlozenie noveho prvku so zapornou hodnotou
TEST_F(emptyBinaryTreeTest, InsertnegativeNode){

	insRetVal = TestedTree->InsertNode(-42);
	EXPECT_NE((void*) NULL, insRetVal.second);
	EXPECT_TRUE(insRetVal.first);
}
// testuje vymazanie prvku na prazdnom strome
TEST_F(emptyBinaryTreeTest, DeleteNode){

	EXPECT_EQ(NULL, TestedTree->DeleteNode(5));
	EXPECT_EQ(NULL, TestedTree->DeleteNode(-4));
	EXPECT_EQ(NULL, TestedTree->DeleteNode(0));
}
// testuje vyhladanie prvku na prazdnom strome
TEST_F(emptyBinaryTreeTest, FindNode){

	EXPECT_EQ(NULL, TestedTree->FindNode(5));//
	EXPECT_EQ(NULL, TestedTree->FindNode(-42));//
}
// testuje ci je strom na zaiatku testu nie je prazdny
TEST_F(BinaryTreeTest, isNotEmpty){

	EXPECT_NE((void *) NULL, TestedTree->GetRoot());
}
// testuje vkladanie noveho prvku do stromu ktory uz obsahuje niake prvky
TEST_F(BinaryTreeTest, InsertNode){

	insRetVal = TestedTree->InsertNode(15);	// testy pridania noveho prvku
	EXPECT_TRUE(insRetVal.first);
	EXPECT_NE((void*)NULL, insRetVal.second);

	insRetVal = TestedTree->InsertNode(12);// testy pridania uz existujuceho prvku
	EXPECT_FALSE(insRetVal.first);
	EXPECT_EQ(cmpInsRetVal.second, insRetVal.second);
}
// testuje odstranovanie uzlov neprazdneho stromu
TEST_F(BinaryTreeTest, DeleteNode){

	EXPECT_FALSE(TestedTree->DeleteNode(-200));
	EXPECT_FALSE(TestedTree->DeleteNode(10)); // test neexistujuceho prvku

	for(int i = 0; i < 10; i++){
		EXPECT_TRUE(TestedTree->DeleteNode(values[i]));
	}
	EXPECT_EQ(NULL, TestedTree->GetRoot());
}
// testuje vyhladavanie prvku v neprazdnom strome
TEST_F(BinaryTreeTest, FindNode){
	EXPECT_EQ(NULL, TestedTree->FindNode(15));
	EXPECT_EQ(cmpInsRetVal.second, TestedTree->FindNode(12));
}
// testuje ze su vsetky listy cierne
TEST_F(AxiomsTest, LeafNodesColor){

	for(std::vector<BinaryTree::Node_t *>::iterator LeafIterator = outLeafNodes.begin(); LeafIterator != outLeafNodes.end(); LeafIterator++){
		EXPECT_EQ(BinaryTree::BLACK, (*LeafIterator)->color);
	}
}
// testuje farbu potomkov kazdeho cerveneho uzlu
TEST_F(AxiomsTest, AllNodesColor){

	std::vector<BinaryTree::Node_t *>::iterator AllIterator;
	for(std::vector<BinaryTree::Node_t *>::iterator AllIterator = outAllNodes.begin(); AllIterator != outAllNodes.end(); AllIterator++){
		if(BinaryTree::RED == (*AllIterator)->color){
			EXPECT_EQ(BinaryTree::BLACK, (*AllIterator)->pLeft->color);
			EXPECT_EQ(BinaryTree::BLACK, (*AllIterator)->pRight->color);

		}
	}
}
// testuje vzdialenost listovych uzlov od korena	
TEST_F(AxiomsTest, PathToRoot){
	BinaryTree::Node_t *tmpNode;
	int distances[outLeafNodes.size()];
	int count = 0, j = 0;

	for(int i = 0; i < outLeafNodes.size(); i++){					// vynulovanie pola
		distances[i] = 0;
	}

	for(std::vector<BinaryTree::Node_t *>::iterator LeafIterator = outLeafNodes.begin(); LeafIterator != outLeafNodes.end(); LeafIterator++){            // priechod vektorom pomocou iteratora
		tmpNode = (*LeafIterator);
		while(tmpNode->pParent != NULL){
			if(tmpNode->color == BinaryTree::BLACK){
				count++;
			}
			tmpNode = tmpNode->pParent;
		}
		distances[j] = count;		
	}
	bool check = true;
	for(int i = 0; i < j; i++){
		if(distances[i] != distances[j-1]){
			check = false;
		}
	}
	EXPECT_TRUE(check);
}
