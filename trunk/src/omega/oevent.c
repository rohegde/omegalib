/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * oevent functions implementation. See oinput.h for more details.
 *********************************************************************************************************************/
#include "oevent.h"
#include "olist.h"

/**********************************************************************************************************************
 * Module defines
 */
#define OM_MAX_EVENTS 1024

/**********************************************************************************************************************
 * Module globals
 */
OMList* gEvtList;
OMNode* gEvtHead;
OMNode* gEvtTail;
int gDroppedEvents = 0;
int gAvailableEvents = 0;

/**********************************************************************************************************************
*/
void oeventInit(const OMConfig* cfg)
{
	OMNode* cur;

	// Initialize the event circular list.
	gEvtList = olistCreate();
	olistInit(gEvtList, OM_MAX_EVENTS, "evt%d", OM_LIST_CIRCULAR);

	// Allocate the event structure data for nodes.
	cur = gEvtList->head;
	do
	{
		cur->data = malloc(sizeof(OMEvent));
		memset(cur->data, 0, sizeof(OMEvent));
		cur = cur->next;
	} while(cur != gEvtList->head);
	gEvtHead = gEvtList->head;
	gEvtTail = gEvtHead;
}

/**********************************************************************************************************************
*/
int oeventGetDroppedEvents()
{
	return gDroppedEvents;
}

/**********************************************************************************************************************
*/
int oeventGetAvailableEvents()
{
	return gAvailableEvents;
}

/**********************************************************************************************************************
*/
void oeventResetDroppedEvents()
{
	gDroppedEvents = 0;
}

/**********************************************************************************************************************
*/
void oeventLock()
{
}

/**********************************************************************************************************************
*/
void oeventUnlock()
{
}

/**********************************************************************************************************************
*/
OMEvent* oeventReadTail()
{
	if(gAvailableEvents > 0)
	{
		OMEvent* evt = (OMEvent*)gEvtTail->data;
		gEvtTail = gEvtTail->next;
		gAvailableEvents--;
		return evt;
	}
	return NULL;
}

/**********************************************************************************************************************
*/
OMEvent* oeventReadHead()
{
	if(gAvailableEvents > 0)
	{
		OMEvent* evt = (OMEvent*)gEvtHead->data;
		gEvtHead = gEvtHead->prev;
		gAvailableEvents--;
		return evt;
	}
	return NULL;
}

/**********************************************************************************************************************
*/
OMEvent* oeventWriteHead()
{
	OMEvent* evt = (OMEvent*)gEvtHead->data;
	gEvtHead = gEvtHead->next;

	// This is not totally exact, we would need an event more to actually start dropping..
	if(gAvailableEvents == OM_MAX_EVENTS)
	{
		gDroppedEvents++;
	}
	else
	{
		gAvailableEvents++;
	}

	return evt;
}