/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Generic linked list implementation
 *********************************************************************************************************************/
#include "olist.h"

/**********************************************************************************************************************
*/
OMNode* olistCreateNode(void* data, const char* name, char flags)
{
	OMNode* node;
	
	node = (OMNode*)malloc(sizeof(OMNode));
	memset(node, 0, sizeof(OMNode));

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
void olistInit(OMList* list, int numNodes, const char* nodeName, enum OMListFlags flags)
{
	char name[OM_NODE_NAME_LEN];

	OMNode* cur = NULL;
	OMNode* head = NULL;
	
	int i = 0;

	name[0] = '\0';

	while(i < numNodes)
	{
		OMNode* prev = cur;

		// Set node name
		if(nodeName != NULL) sprintf(name, nodeName, i);

		cur = olistCreateNode(NULL, name, 0);
		if(!head) head = cur;
		if(prev) prev->next = cur;
		i++;
	}
	list->head = head;
	list->tail = cur;

	// Setup circular list.
	if(flags & OM_LIST_CIRCULAR) list->tail->next = list->head;
}

/**********************************************************************************************************************
*/
OMList* olistCreate()
{
	OMList* list = (OMList*)malloc(sizeof(OMList));
	memset(list, 0, sizeof(OMList));
	return list;
}

/**********************************************************************************************************************
*/
void olistDestroy(OMList** list)
{
	OMNode* head = (*list)->head;
	OMNode* cur = (*list)->head;
	do
	{
		OMNode* tmp = cur->next;
		olistDestroyNode(tmp);
		cur = tmp;
	}
	while(cur && cur != head);

	free(list);
	(*list) = NULL;
}
