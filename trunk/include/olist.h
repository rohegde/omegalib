/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Linked list (normal & circular) implementation
 *********************************************************************************************************************/
#include "osystem.h"

#define OM_CLIST_NAME_LEN 20

/**********************************************************************************************************************
*/
struct _OMNode
{
    char name[OM_CLIST_NAME_LEN];
    char flags;
    struct _OMNode* next;
	void* data;
};

typedef struct _OMNode OMNode;

struct _OMList
{
	OMNode* head;
	OMNode* tail;
};

typedef struct _OMList OMList;

/**********************************************************************************************************************
 * oclistCreateNode
 *	Creates a new node.
 */
OMNode* olistCreateNode(void* data, const char* name, char flags);

/**********************************************************************************************************************
 * oclistDestroyNode
 *  Deallocates a list node.
 *  NOTE: this function just frees up node memory, it does not fix list links.
 */
void olistDestroyNode(OMNode* node);

/**********************************************************************************************************************
 * oclistInit
 *	Initialized a linked list with the specified number of elements.
 */
OMNode* olistInit(int numNodes);

/**********************************************************************************************************************
 * oclistDestroy
 */
void olistDestroy(OMNode* head);
