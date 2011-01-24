/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************************************************/
#include "omega/scene/SceneQuery.h"

using namespace omega;
using namespace omega::scene;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneQuery::clearResults()
{
	myResults.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const SceneQueryResultList& RaySceneQuery::execute(uint flags)
{
	bool queryOne = ((flags & SceneQuery::QueryFirst) == SceneQuery::QueryFirst) ? true : false;

	queryNode(myScene->getRootNode(), myResults, queryOne);

	if((flags & SceneQuery::QuerySort) == SceneQuery::QuerySort)
	{
		myResults.sort(SceneQueryResultDistanceCompare);
	}
	return myResults;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RaySceneQuery::queryNode(SceneNode* node, SceneQueryResultList& list, bool queryFirst)
{
	bool selected = false;
	if(node->isSelectable())
	{
		const AxisAlignedBox& bbox = node->getBoundingBox();
		printf("Bounding box min: %f %f %f max: %f %f %f\n", 
			bbox.getMinimum()[0], bbox.getMinimum()[1], bbox.getMinimum()[2],
			bbox.getMaximum()[0], bbox.getMaximum()[1], bbox.getMaximum()[2]);
		std::pair<bool, float> resPair = myRay.intersects(bbox);
		if(resPair.first)
		{
			SceneQueryResult res;
			res.node = node;
			res.distance = resPair.second;
			list.push_back(res);
		}
	}

	// If this node has been selected, and we are returning the first node found, return immediately.
	if(selected && queryFirst) return;

	// Draw children nodes.
	for(int i = 0; i < node->getNumChildren(); i++)
	{
		SceneNode* n = node->getChild(i);
		queryNode(n, list, queryFirst);
	}
}

