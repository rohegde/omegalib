/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Generic double linked list implementation
 *********************************************************************************************************************/
#include "osystem.h"

#define OM_NODE_NAME_LEN 20

/**********************************************************************************************************************
*/
enum OMListFlags
{
	OM_LIST_CIRCULAR
};

/**********************************************************************************************************************
*/
typedef struct _OMNode
{
    char name[OM_NODE_NAME_LEN];
    char flags;
    struct _OMNode* next;
    struct _OMNode* prev;
	void* data;
} OMNode;

typedef struct _OMList
{
	OMNode* head;
	OMNode* tail;
} OMList;

/**********************************************************************************************************************
 * olistCreateNode
 *	Creates a new node.
 */
OMNode* olistCreateNode(void* data, const char* name, char flags);

/**********************************************************************************************************************
 * olistDestroyNode
 *  Deallocates a list node.
 *  NOTE: this function just frees up node memory, it does not fix list links.
 */
void olistDestroyNode(OMNode* node);

/**********************************************************************************************************************
 * olistDestroy
 */
OMList* olistCreate();

/**********************************************************************************************************************
 * olistDestroy
 */
void olistDestroy(OMList** list);

/**********************************************************************************************************************
 * olistInit
 *	Initializes a linked list with the specified number of elements.
 */
void olistInit(OMList* list, int numNodes, const char* nodeName, enum OMListFlags flags);

