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

			mm->loadMesh(meshName, meshFilename, MeshManager::MeshFormatPly, true);
			Mesh* mesh = mm->getMesh(meshName);
			myEntities.push_back(new Entity(meshLabel, myEngine->getSceneManager(), mesh));
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
void MeshViewerClient::processPointerEvent(const Event& evt, DrawContext& context)
{
	int vx1 = context.viewport[0][0];
	int vy1 = context.viewport[0][1];
	int vx2 = context.viewport[0][0] + context.viewport[1][0];
	int vy2 = context.viewport[0][1] + context.viewport[1][1];

	if(evt.position[0] > vx1 &&
		evt.position[0] < vx2 &&
		evt.position[1] > vy1 &&
		evt.position[1] < vy2)
	{
		// Select objects.
		Ray ray = unproject(Vector2f(evt.position[0], evt.position[1]), context);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3fv(ray.getOrigin().begin());
		glEnd();

		printf("%.2f %.2f %2.f -> %.2f %.2f %.2f\n", 
			ray.getOrigin()[0], ray.getOrigin()[1], ray.getOrigin()[2],
			ray.getDirection()[0], ray.getDirection()[1], ray.getDirection()[2]);

		//glColor4f(0.0f, 1.0f, 0.0f, 0.2f);
		//glRecti(vx1, vy1, vx2, vy2);
		//glRecti(0, 0, context.viewport[1][0], context.viewport[1][1]);

		if(evt.type == Event::Down)
		{

			if(myVisibleEntity != NULL)
			{
				Vector3f handlePos;
				if(myVisibleEntity->hit(ray, &handlePos))
				{
					myVisibleEntity->activate(handlePos);
				}
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
		else if(evt.type == Event::Move)
		{
			// Manipulate object, if one is active.
			if(myVisibleEntity != NULL && myVisibleEntity->isActive())
			{
				Ray ray = unproject(Vector2f(evt.position[0], evt.position[1]), context);

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
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const Event& evt, UpdateContext& context)
{
	myEngine->handleEvent(evt);

	switch(evt.serviceType)
	{
	case Service::Mocap:
		// Update observer
		if(evt.sourceId == 1)
		{
			if(evt.position.length() > 0.1f)
			{
				getDisplaySystem()->getObserver(0)->update(evt.position, evt.orientation);
			}
			return true;
		}
	break;
	}
	return false;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const Event& evt, DrawContext& context)
{
	//int vx1 = context.viewport[0][0];
	//int vy1 = context.viewport[0][1];
	//int vx2 = context.viewport[0][0] + context.viewport[1][0];
	//int vy2 = context.viewport[0][1] + context.viewport[1][1];
	//printf("PT %d %d of %d %d %d %d\n", (int)evt.position[0], (int)evt.position[1], vx1, vy1, vx2, vy2);

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
