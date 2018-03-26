//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Jakub Crkoň <xcrkon00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Jakub Crkoň 
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

PriorityQueue::PriorityQueue()
{
	this->list_head = NULL;
}

PriorityQueue::~PriorityQueue()
{	
	while(list_head != NULL){
		this->Remove(list_head->value);
	}
	
}

void PriorityQueue::Insert(int value)
{
	Element_t *new_node = new Element_t;
	new_node->value = value;

	if(list_head == NULL){
		list_head = new_node;
		new_node->pNext = NULL;
		new_node->pPrev = NULL;
	}
	else{
		for(Element_t *tmp = list_head; tmp != NULL; tmp = tmp->pNext){
			if(tmp->value >= value){
				if(tmp->pPrev == NULL){
					new_node->pPrev = NULL;
					new_node->pNext = tmp;
					list_head = new_node;
					tmp->pPrev = new_node;
				}
				else{
					tmp->pPrev->pNext = new_node;
					new_node->pPrev = tmp->pPrev;
					new_node->pNext = tmp;
					tmp->pPrev = new_node;
				}
				break;
			}
			if(tmp->pNext == NULL){
				tmp->pNext = new_node;
				new_node->pPrev = tmp;
				new_node->pNext = NULL;
				break;
			}		
		}
	}
}

bool PriorityQueue::Remove(int value)
{
	if(list_head == NULL){
		return false;
	}

	for(Element_t *tmp = list_head; tmp != NULL; tmp = tmp->pNext){
		if(tmp->value == value){
			if(tmp->pNext != NULL && tmp->pPrev != NULL){
				tmp->pPrev->pNext = tmp->pNext;
				tmp->pNext->pPrev = tmp->pPrev;
				delete tmp;
				return true;
			}
			if(tmp->pNext == NULL && tmp->pPrev == NULL){
				delete tmp;
				list_head = NULL;
			}
			else if(tmp->pNext == NULL){
				tmp->pPrev->pNext = NULL;
				delete tmp;
			}
			else if(tmp->pPrev == NULL){
				tmp->pNext->pPrev = NULL;
				list_head = tmp->pNext;
				delete tmp;
			}
			return true;
		}
	}
	return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{

	for(Element_t *tmp = list_head; tmp != NULL; tmp = tmp->pNext){
		if(tmp->value == value){
			return tmp;
		}
	}
    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{	
	return list_head;
}

/*** Konec souboru tdd_code.cpp ***/
