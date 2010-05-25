/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * Simple crossplatform threading support.
 *********************************************************************************************************************/
#ifndef __OTHREAD_H__
#define __OTHREAD_H__

#include "osystem.h"

#ifdef WIN32
#define OM_THREADPROC __stdcall
#define OMThread HANDLE
#define OMMutex CRITICAL_SECTION
#else
#define OM_THREADPROC
#define OMThread pthread_t
#define OMMutex pthread_mutex_t
#endif

/**********************************************************************************************************************
 * othreadCreate
 *  Creates a new thread running the specified procedure.
 */
OMThread othreadCreate(unsigned (OM_THREADPROC* thread_func)(void*), void* userData);

/**********************************************************************************************************************
 * othreadDestroy
 *  Kills an existing thread.
 */
void othreadKill(OMThread thread);

/**********************************************************************************************************************
 */
void othreadMutexInit(OMMutex* m);

/**********************************************************************************************************************
 */
void othreadMutexDelete(OMMutex* m);

/**********************************************************************************************************************
 */
void othreadMutexLock(OMMutex* m);

/**********************************************************************************************************************
 */
void othreadMutexUnlock(OMMutex* m);

/**********************************************************************************************************************
 */
int othreadMutexTestLock(OMMutex* m);

#endif