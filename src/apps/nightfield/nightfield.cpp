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
#include "nightfield.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
Settings::Settings():
	numAgents(2000),
	areaMin(Vector3f(-0.4f, 0.7f, -1.4f)),
	areaMax(Vector3f(0.8f, 1.9f, -2.6f)),
	center(0.0f, 0.0f, 0.0),

	minAvoidanceDist(0), maxAvoidanceDist(1),
	minCoordinationDist(0), maxCoordinationDist(1),
	minFriction(0), maxFriction(1)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::load(const Setting& s)
{
	const Setting& sPreset = s["preset"];

	Preset* p = new Preset();
	loadPreset(p, sPreset);
	presets.push_back(p);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::loadPreset(Preset* p, const Setting& s)
{
	p->friction = s["friction"];
	p->avoidanceDist = s["avoidanceDist"];
	p->coordinationDist = s["coordinationDist"];
	p->flockImage = String((const char*)s["flockImage"]);
	p->useAdditiveAlpha = s["useAddictiveAlpha"];
	p->drawSpeedVectors = s["drawSpeedVectors"];

	p->speedVectorScale = s["speedVectorScale"];

	p->useFog = s["useFog"];

	Setting& sc = s["speedVectorColor"];
	p->speedVectorColor = Color(sc[0], sc[1], sc[2], sc[3]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
AffectorEntity::AffectorEntity(SceneObject* object, EngineServer* server):
	myObject(object),
	myServer(server),
	myVisible(false)
{
	mySelectionSphere = new BoundingSphere();
	mySelectionSphere->setDrawOnSelected(true);
	mySelectionSphere->setVisible(false);

	mySceneNode = new SceneNode(server);
	mySceneNode->setSelectable(true);
	server->getScene(0)->addChild(mySceneNode);
	mySceneNode->addObject(myObject);
	mySceneNode->addObject(mySelectionSphere);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
AffectorEntity::~AffectorEntity()
{
	myServer->getScene(0)->removeChild(mySceneNode);
	delete mySceneNode;
	mySceneNode = NULL;

	delete mySelectionSphere;
	mySelectionSphere = NULL;

	myObject = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::setVisible(bool value)
{
	myVisible = value;
	if(myObject != NULL)
	{
		mySceneNode->setVisible(value);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::resetTransform()
{
	mySceneNode->setPosition(0, 0, -0.5f);
	mySceneNode->setScale( 1.0 , 1.0 , 1.0 );
	mySceneNode->resetOrientation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::updateFlockAffector(FlockAffector* af)
{
	const Sphere& bs = mySceneNode->getBoundingSphere();
	af->x = mySceneNode->getPosition().x();
	af->y = mySceneNode->getPosition().y();
	af->z = mySceneNode->getPosition().z();
	af->rx = bs.getRadius() * 0.2f;
	af->f1 = 1.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Nightfield::initialize()
{
	mySelectedEntity = NULL;

	EngineServer::initialize();

	Config* cfg = getSystemManager()->getAppConfig();
	if(cfg->exists("config"))
	{
		Setting& sCfg = cfg->lookup("config");
		mySettings.load(sCfg);
	}

	SceneNode* scene = getScene(0);

	mySceneNode = new SceneNode(this);

	myFlock = new Flock();
	
	mySelectionSphere = new BoundingSphere();
	mySelectionSphere->setDrawOnSelected(true);
	mySelectionSphere->setVisible(false);

	scene->addChild(mySceneNode);
	mySceneNode->addObject(myFlock);
	myFlock->setup(&mySettings);
	myFlock->initialize();
	mySceneNode->addObject(mySelectionSphere);

	myMouseInteractor = new DefaultMouseInteractor();
	addActor(myMouseInteractor);
	myNavigationInteractor = new NavigationInteractor();
	addActor(myNavigationInteractor);

	// Create a reference box around the scene.
	if(cfg->exists("config/referenceBox"))
	{
		myReferenceBox = new ReferenceBox();
		scene->addObject(myReferenceBox);
		myReferenceBox->setSize(Vector3f(4.0f, 4.0f, 4.0f));
		myReferenceBox->setColor(ReferenceBox::Back, Color(0.8f, 0.8f, 0.8f));
	}

	// Create affector entity.
	PlyDataReader* sphere = new PlyDataReader();
	sphere->readPlyFile("meshes/sphere.ply");
	sphere->scale(0.2f);
	Mesh* m = new Mesh();

	AffectorEntity* ae = new AffectorEntity(m, this);
	m->setData(sphere);
	myEntities.push_back(ae);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Nightfield::update(const UpdateContext& context)
{
	int i = 0;
	foreach(AffectorEntity* ae, myEntities)
	{
		FlockAffector* fa = myFlock->getAffector(i);
		ae->updateFlockAffector(fa);
		i++;
	}
	//myFlock->setActiveAffectors(i);
	myFlock->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Nightfield::handleEvent(const Event& evt)
{
	EngineServer::handleEvent(evt);
	if(evt.getServiceType() == Service::Pointer)
	{
		if(evt.getType() == Event::Down && evt.isFlagSet(Event::Left) && evt.getExtraDataLength() == 2)
		{
			Ray ray;
			ray.setOrigin(evt.getExtraDataVector3(0));
			ray.setDirection(evt.getExtraDataVector3(1));
			updateSelection(ray);
		}
		else if(evt.getType() == Event::Down && evt.isFlagSet(Event::Right))
		{
			myFlock->getSettings()->center = evt.getExtraDataVector3(0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
AffectorEntity* Nightfield::findEntity(SceneNode* node)
{
	foreach(AffectorEntity* e, myEntities)
	{
		if(e->getSceneNode() == node) return e;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Nightfield::updateSelection(const Ray& ray)
{
	const SceneQueryResultList& sqrl = querySceneRay(0, ray);
	if(sqrl.size() != 0)
	{
		// The ray intersected with something.
		SceneNode* sn = sqrl.front().node;
		AffectorEntity* e = findEntity(sn);

		if(mySelectedEntity != e)
		{
			if(mySelectedEntity != NULL)
			{
				mySelectedEntity->getSceneNode()->setSelected(false);
			}
			// The selected entity changed.
			myMouseInteractor->setSceneNode(sn);
			myNavigationInteractor->setSceneNode(sn);
			sn->setSelected(true);
			mySelectedEntity = e;
		}
	}
	else
	{
		if(mySelectedEntity != NULL)
		{
			mySelectedEntity->getSceneNode()->setSelected(false);
			mySelectedEntity = NULL;
			myMouseInteractor->setSceneNode(NULL);
			myNavigationInteractor->setSceneNode(NULL);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<Nightfield> app;
	omain(
		app, 
		"nightfield.cfg", 
		"nightfield.log", 
		new FilesystemDataSource(OMEGA_DATA_PATH));
	return 0;
}
