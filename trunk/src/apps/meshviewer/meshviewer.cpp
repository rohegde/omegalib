/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#include "meshviewer.h"

// Uncomment to enable mouse move / zoom / rotate
//#define MOUSE_INERACTION

// Uncomment to enable two hand gestures for move / zoom / rotate
#define TWO_HANDS

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::initialize()
{
	myEngine = new EngineClient(this);
	myEngine->initialize();

	Config* cfg = getSystemManager()->getAppConfig();

	if(cfg->exists("config/defaultFont"))
	{
		Setting& fontSetting = cfg->lookup("config/defaultFont");
		myEngine->getFontManager()->createFont("default", fontSetting["filename"], fontSetting["size"]);
	}

	MeshManager* mm = myEngine->getMeshManager();

	// Load meshes specified in config file.
	if(cfg->exists("config/meshes"))
	{
		Setting& meshes = cfg->lookup("config/meshes");
		for(int i = 0; i < meshes.getLength(); i++)
		{
			Setting& meshSetting = meshes[i];
			String meshName = meshSetting.getName();
			String meshFilename = meshSetting["filename"];
			String meshLabel = meshSetting["label"];

			mm->loadMesh(meshName, meshFilename, MeshManager::MeshFormatPly, 0.2f);
			Mesh* mesh = mm->getMesh(meshName);
			Entity* e = new Entity(meshLabel, myEngine->getSceneManager(), mesh);
			myEntities.push_back(e);
		}
	}

	// Create and initialize meshviewer UI
	myUI = new MeshViewerUI();
	myUI->initialize(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::setVisibleEntity(int entityId)
{
	if(myVisibleEntity != NULL)
	{
		myVisibleEntity->setVisible(false);
		myVisibleEntity = NULL;
	}

	Entity* e = myEntities[entityId];
	myVisibleEntity = e;
	myVisibleEntity->resetTransform();
	myVisibleEntity->setVisible(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::setActiveUser(int userId)
{
	myActiveUserId = userId;
	ObserverUpdateService* svc = getServiceManager()->findService<ObserverUpdateService>("ObserverUpdateService");
	svc->setSourceId(userId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::processMocapEvent(const Event& evt, UpdateContext& context)
{
	// Select objects (use a positive z layer since objects in this program usually lie on the projection plane)
	float z = 1.0f;
	//Ray ray = Math::unproject(Vector2f(evt.position[0], evt.position[1]), context.modelview, context.projection, context.viewport, z);
	//Ray ray = Ray(evt.pointSet[RightHand], Vector3f(0, 0, -1.0f));

#ifdef TWO_HANDS
	if(evt.sourceId == myActiveUserId)
	{
		myHandsValid = evt.isValidPoint(RightHand) && evt.isValidPoint(LeftHand);
		if(myHandsValid)
		{
			myLeftHandPosition = evt.pointSet[LeftHand];
			myRightHandPosition = evt.pointSet[RightHand];
			//myRightHandPosition.z() = myRightHandPosition.z() - 0.3f;
			//myLeftHandPosition.z() = myLeftHandPosition.z() - 0.3f;
			if(myVisibleEntity != NULL && myVisibleEntity->isActive())
			{
				Vector3f pos = (myLeftHandPosition + myRightHandPosition) / 2;
				float size = (myLeftHandPosition - myRightHandPosition).norm();
				myVisibleEntity->setPosition(pos);
				myVisibleEntity->setSize(size);

				Vector3f orig = Vector3f(0, 1, 0);
				Vector3f dir = myLeftHandPosition - myRightHandPosition;
				dir.normalize();
				Quaternion q = Math::buildRotation(orig, dir, Vector3f::Zero());
				myVisibleEntity->setOrientation(q);
			}
		}
	}
#else
	if(evt.sourceId == myActiveUserId)
	{
		myHandsValid = evt.isValidPoint(RightHand) && evt.isValidPoint(LeftHand);
		if(myHandsValid)
		{
			//ofmsg("Ray pos: %1%", %ray.getOrigin());

			//float activationZ = 0.6f;

			//if(evt.pointSet[RightHand].z() < activationZ)
			//{
			//	if(myVisibleEntity != NULL && !myVisibleEntity->isActive())
			//	{
			//		Vector3f handlePos;
			//		if(myVisibleEntity->hit(ray, &handlePos))
			//		{
			//			myVisibleEntity->activate(handlePos);
			//		}
			//	}
			//	if(myVisibleEntity != NULL && myVisibleEntity->isActive())
			//	{
			//		float z = 1.0f;
			//		//Ray ray = Math::unproject(Vector2f(evt.position[0], evt.position[1]), context.modelview, context.projection, context.viewport, z);
			//		Ray ray = Ray(evt.pointSet[RightHand], Vector3f(0, 0, -1.0f));

			//		//if(evt.isFlagSet(Event::Left))
			//		//{
			//		//	myVisibleEntity->manipulate(Entity::Move, ray);
			//		//}
			//		//else if(evt.isFlagSet(Event::Right))
			//		//{
			//			//myVisibleEntity->manipulate(Entity::Rotate, ray);
			//		//}
			//		//else if(evt.isFlagSet(Event::Middle))
			//		//{
			//		//	myVisibleEntity->manipulate(Entity::Scale, ray);
			//		//}
			//	}
			//}
			//else if(evt.position.z() > activationZ)
			//{
			//	// Deselect objects.
			//	if(myVisibleEntity != NULL)
			//	{
			//		myVisibleEntity->deactivate();
			//	}
			//}
			//else if(evt.type == Event::Zoom)
			//{
			//	// Manipulate object, if one is active.
			//	if(myVisibleEntity != NULL)
			//	{
			//		float sc;
			//		if(evt.value[0] < 0) sc = 0.9f;
			//		else sc = 1.1f;
			//		myVisibleEntity->scale(sc);
			//	}
			//}
		}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::processPointerEvent(const Event& evt, DrawContext& context)
{
	// Select objects (use a positive z layer since objects in this program usually lie on the projection plane)
	float z = 1.0f;
	Ray ray = Math::unproject(Vector2f(evt.position[0], evt.position[1]), context.modelview, context.projection, context.viewport, z);

	if(evt.type == Event::Down)
	{
		if(myVisibleEntity != NULL)
		{
#ifdef MOUSE_INTERACTION
			Vector3f handlePos;
			if(myVisibleEntity->hit(ray, &handlePos))
			{
				myVisibleEntity->activate(handlePos);
			}
#else
			myVisibleEntity->activate(Vector3f::Zero());
#endif
		}
	}
	else if(evt.type == Event::Up)
	{
		// Deselect objects.
		if(myVisibleEntity != NULL)
		{
			myVisibleEntity->deactivate();
		}
	}
#ifdef MOUSE_INTERACTION
	else if(evt.type == Event::Move)
	{
		// Manipulate object, if one is active.
		if(myVisibleEntity != NULL && myVisibleEntity->isActive())
		{
			float z = 1.0f;
			Ray ray = Math::unproject(Vector2f(evt.position[0], evt.position[1]), context.modelview, context.projection, context.viewport, z);

			if(evt.isFlagSet(Event::Left))
			{
				myVisibleEntity->manipulate(Entity::Move, ray);
			}
			else if(evt.isFlagSet(Event::Right))
			{
				myVisibleEntity->manipulate(Entity::Rotate, ray);
			}
			else if(evt.isFlagSet(Event::Middle))
			{
				myVisibleEntity->manipulate(Entity::Scale, ray);
			}

		}
	}
	else if(evt.type == Event::Zoom)
	{
		// Manipulate object, if one is active.
		if(myVisibleEntity != NULL)
		{
			float sc;
			if(evt.value[0] < 0) sc = 0.9f;
			else sc = 1.1f;
			myVisibleEntity->scale(sc);
		}
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const Event& evt, UpdateContext& context)
{
	myEngine->handleEvent(evt);
	switch(evt.serviceType)
	{
	case Service::Mocap:
		if(evt.type == Event::Trace)
		{
			ofmsg("Tracing user %1%", %evt.sourceId);
			myUI->onTraceUser(evt.sourceId);
		}
		else if(evt.type == Event::Untrace)
		{
			ofmsg("Untracing user %1%", %evt.sourceId);
			myUI->onUntraceUser(evt.sourceId);
		}
		else
		{
			processMocapEvent(evt, context);
		}
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const Event& evt, DrawContext& context)
{
	myEngine->handleEvent(evt);

	switch(evt.serviceType)
	{
	case Service::Pointer:
		processPointerEvent(evt, context);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::update(const UpdateContext& context)
{
	myEngine->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::draw(const DrawContext& context)
{
#ifdef OMEGA_USE_OPENNI
	OpenNIService* svc = getServiceManager()->findService<OpenNIService>("OpenNIService");
	myUI->updateKinectTexture(svc);
#endif

	AlignedBox3 box(Vector3f(-1.0f, -1.5f, -0.5f), Vector3f(1.0f, 1.5f, 3.0f));
	Color backCol(1.0f, 0.2f, 0.2f, 1.0f);
	Color frontCol(1.0f, 0.2f, 0.2f, 1.0f);
	Color leftCol(0.2f, 1.0f, 0.2f, 1.0f);
	Color rightCol(0.2f, 1.0f, 0.2f, 1.0f);
	Color bottomCol(0.2f, 0.2f, 1.0f, 1.0f);
	Color topCol(0.2f, 0.2f, 1.0f, 1.0f);
	Color gridCol(0.0f, 0.0f, 0.0f, 1.0f);

	drawReferenceBox(box, 
		bottomCol, topCol, leftCol, rightCol, frontCol, backCol, 
		gridCol, 10);

	switch(context.layer)
	{
	case 0:
		myEngine->draw(context, EngineClient::DrawScene | EngineClient::DrawUI);
		break;
	case 1:
		myEngine->draw(context, EngineClient::DrawScene);
		break;
	case 2:
		myEngine->draw(context, EngineClient::DrawScene | EngineClient::DrawUI);
		break;
	}

	if(myHandsValid)
	{
		//glColor3f(1.0f, 0.0f, 0.5f);
		//glBegin(GL_LINES);
		//glVertex3fv(myLeftHandPosition.data());
		//glVertex3fv(myRightHandPosition.data());
		//glEnd();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::drawReferenceBox(
		const AlignedBox3& box, 
		const Color& bottomColor, const Color& topColor, const Color& leftColor, const Color& rightColor, const Color& frontColor, const Color& backColor, 
		const Color& gridColor, const int gridLines)
{
	drawReferencePlane(box.getCorner(AlignedBox3::FAR_LEFT_TOP), box.getCorner(AlignedBox3::FAR_RIGHT_BOTTOM), AxisZ, backColor);
	drawReferencePlane(box.getCorner(AlignedBox3::FAR_LEFT_TOP), box.getCorner(AlignedBox3::NEAR_LEFT_BOTTOM), AxisX, leftColor);
	drawReferencePlane(box.getCorner(AlignedBox3::FAR_RIGHT_TOP), box.getCorner(AlignedBox3::NEAR_RIGHT_BOTTOM), AxisX, rightColor);
	drawReferencePlane(box.getCorner(AlignedBox3::FAR_LEFT_BOTTOM), box.getCorner(AlignedBox3::NEAR_RIGHT_BOTTOM), AxisY, bottomColor);
	drawReferencePlane(box.getCorner(AlignedBox3::FAR_LEFT_TOP), box.getCorner(AlignedBox3::NEAR_RIGHT_TOP), AxisY, topColor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::drawReferencePlane(
	const Vector3f& min, const Vector3f& max, Axis normal, const Color& color)
{
	Vector3f v1;
	Vector3f v2;
	Vector3f v3;
	Vector3f v4;

	switch(normal)
	{
	case AxisX:
		v1 = Vector3f(min[0], min[1], min[2]);
		v2 = Vector3f(min[0], max[1], min[2]);
		v3 = Vector3f(min[0], min[1], max[2]);
		v4 = Vector3f(min[0], max[1], max[2]);
		break;
	case AxisY:
		v1 = Vector3f(min[0], min[1], min[2]);
		v2 = Vector3f(min[0], min[1], max[2]);
		v3 = Vector3f(max[0], min[1], min[2]);
		v4 = Vector3f(max[0], min[1], max[2]);
		break;
	case AxisZ:
		v1 = Vector3f(min[0], min[1], min[2]);
		v2 = Vector3f(min[0], max[1], min[2]);
		v3 = Vector3f(max[0], min[1], min[2]);
		v4 = Vector3f(max[0], max[1], min[2]);
		break;
	}

	glColor4fv(color.data());
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3fv(v1.data());
	glVertex3fv(v2.data());
	glVertex3fv(v3.data());
	glVertex3fv(v4.data());
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	MeshViewerApplication app;

	// Read config file name from command line or use default one.
	const char* cfgName = "meshviewer.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "meshviewer.log", new FilesystemDataSource("./../../data/"));

	return 0;
}
