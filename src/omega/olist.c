/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Circular linked list implementation
 *********************************************************************************************************************/
#include "olist.h"

/**********************************************************************************************************************
*/
OMNode* olistCreateNode(void* data, const char* name, char flags)
{
	OMNode* node;
	
	node = (OMNode*)malloc(sizeof(OMNode));

	if(!name) name = "<undefined>";
	strcpy(node->name, name);
	node->flags = flags;
	node->data = data;
	return node;
}

/**********************************************************************************************************************
*/
void olistDestroyNode(OMNode* node)
{
	free(node);
}

/**********************************************************************************************************************
*/
OMNode* olistInit(int numNodes)
{
	OMNode* head = NULL;
	OMNode* cur = NULL;
	while(numNodes != 0)
	{
		OMNode* prev = cur;
		cur = olistCreateNode(NULL, NULL, 0);
		if(!head) head = cur;
		if(!prev) prev->next = cur;
		numNodes--;
	}
	cur->next = head;
	return head;
}

/**********************************************************************************************************************
*/
void olistDestroy(OMNode* head)
{
	OMNode* cur = head;
	do
	{
		OMNode* tmp = cur->next;
		olistDestroyNode(tmp);
		cur = tmp;
	}
	while(cur != head);
}
