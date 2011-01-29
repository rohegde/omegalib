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
#include "omega.h"
#include "omega/scene.h"
#include "omega/ui.h"

#include "omega/Plane.h"
#include "omega/Ray.h"

using namespace omega;
using namespace omega::scene;
using namespace omega::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations.
class Entity;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SelectionSphere: public Drawable
{
public:
	SelectionSphere(Entity* e): myEntity(e) {}

	void draw(SceneNode* node);

	void setVisible(bool value) { myVisible = value; }
	bool isVisisble() { return myVisible; }

private:
	Entity* myEntity;
	bool myVisible;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Entity
{
public:
	enum Operation { Move, Scale, Rotate, Compound };

public:
	Entity(SceneManager* sm, Mesh* m, const Vector3f& position);

	bool hit(const Ray& ray, Vector3f* handlePos);
	void manipulate(Operation op, const Ray& ray1, const Ray& ray2 = Ray(Vector3f::ZERO, Vector3f::ZERO));

	void activate(const Vector3f handlePos);
	void deactivate();
	bool isActive() { return myActive; }

	Vector3f getHandlePosition() { return myHandlePosition; }

private:
	SceneNode* mySceneNode;
	Mesh* myMesh;
	SelectionSphere* mySelectionSphere;

	// Interaction stuff
	Vector3f myHandlePosition;
	Sphere myStartBSphere;
	Quaternion myStartOrientation;
	float myStartScale;
	bool myActive;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerClient: public ApplicationClient, IUIEventHandler
{
public:
	MeshViewerClient(Application* app): ApplicationClient(app), myGpu(NULL), myActiveEntity(NULL) {}

	virtual void initialize();
	virtual bool handleEvent(const InputEvent& evt);
	virtual void handleUIEvent(const UIEvent& evt);
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);

private:
	void addEntity(Mesh* m, const Vector3f& position);

private:
	// Managers
	GpuManager* myGpu;
	TextureManager* myTexMng;
	FontManager* myFontMng;
	SceneManager* mySceneManager;
	MeshManager* myMeshManager;
	EffectManager* myEffectManager;
	UIManager myUI;

	// Entity list
	std::list<Entity*> myEntities;

	// Active entity.
	Entity* myActiveEntity;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new MeshViewerClient(this); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SelectionSphere::draw(SceneNode* node)
{
	if(myVisible)
	{
		float radius = node->getBoundingSphere().getRadius();
		float stp = Math::Pi * 2 / 32;
		float stp2 = Math::Pi * 2 / 16;
		glEnable(GL_BLEND);
		glColor4f(0.8, 0.8, 1.0, 0.2f);
	
		for(float g = 0; g < 2 * Math::Pi; g+= stp2)
		{
			glBegin(GL_LINE_LOOP);
			for(float t = 0; t < 2 * Math::Pi; t+= stp)
			{
				float ptx = Math::sin(t) * Math::sin(g) * radius;
				float pty = Math::cos(t) * Math::sin(g) * radius;
				float ptz = Math::cos(g) * radius;
				glVertex3f(ptx, pty, ptz);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			for(float t = 0; t < 2 * Math::Pi; t+= stp)
			{
				float ptz = Math::sin(t) * Math::sin(g) * radius;
				float pty = Math::cos(t) * Math::sin(g) * radius;
				float ptx = Math::cos(g) * radius;
				glVertex3f(ptx, pty, ptz);
			}
			glEnd();
		}

		// Draw handle positions;
		//glColor4f(1.0, 0.0, 0.0, 1.0);
		//glPointSize(16);
		//glBegin(GL_POINTS);
		//glVertex3fv(myEntity->getHandlePosition().begin());
		//glEnd();
		//glPointSize(1);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Entity::Entity(SceneManager* sm, Mesh* m, const Vector3f& position)
{
	myMesh = m;
	mySelectionSphere = new SelectionSphere(this);
	mySelectionSphere->setVisible(false);

	mySceneNode = new SceneNode(sm);
	mySceneNode->addDrawable(myMesh);
	mySceneNode->addDrawable(mySelectionSphere);
	mySceneNode->setPosition(position);
	mySceneNode->setSelectable(true);
	sm->getRootNode()->addChild(mySceneNode);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::activate(const Vector3f handlePos)
{
	myActive = true;
	mySelectionSphere->setVisible(true);

	myStartBSphere = mySceneNode->getBoundingSphere();
	myStartOrientation = mySceneNode->getOrientation();
	myStartScale = mySceneNode->getScale()[0];
	myHandlePosition =  handlePos; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::deactivate()
{
	myActive = false;
	mySelectionSphere->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::hit(const Ray& ray, Vector3f* handlePos)
{
	const Sphere& s = mySceneNode->getBoundingSphere();
	std::pair<bool, float> h = ray.intersects(s);
	if(h.first)
	{
		(*handlePos) = ray.getPoint(h.second) - s.getCenter();
	}
	return h.first;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Entity::manipulate(Operation op, const Ray& ray1, const Ray& ray2)
{
	if(op == Move)
	{
		Vector3f origin = ray1.getOrigin();
		Vector3f direction = ray1.getDirection();
		// Interstect the ray with the Z plane where the handle lies, to get
		// the new handle position.
		float tz = myHandlePosition[2] + myStartBSphere.getCenter()[2];
		float l = (tz - origin[2]) / direction[2];
		float tx = origin[0] + l * direction[0];
		float ty = origin[1] + l * direction[1];

		Vector3f newPos = Vector3f(tx, ty, tz) - myHandlePosition;
		mySceneNode->setPosition(newPos);
	}
	else if(op == Rotate)
	{
		// Intersect the ray with the bounding sphere. 
		// If the point is outside the bounding sphere, perform no rotation.
		std::pair<bool, float> p = ray1.intersects(myStartBSphere);
		if(p.first)
		{
			Vector3f pt = ray1.getPoint(p.second);
			pt -= myStartBSphere.getCenter();
			Quaternion rot = Math::buildRotation(myHandlePosition, pt);
			mySceneNode->setOrientation(rot * myStartOrientation);
		}
	}
	else if(op == Scale)
	{
		Vector3f origin = ray1.getOrigin();
		Vector3f direction = ray1.getDirection();
		// Interstect the ray with the Z plane where the handle lies, to get
		// the new handle position.
		float tz = myHandlePosition[2] + myStartBSphere.getCenter()[2];
		float l = (tz - origin[2]) / direction[2];
		float tx = origin[0] + l * direction[0];
		float ty = origin[1] + l * direction[1];

		Vector3f newPos = Vector3f(tx, ty, tz);// - myHandlePosition;
		float d = (newPos - myStartBSphere.getCenter()).length();

		float scale = myStartScale * d / myStartBSphere.getRadius();

		mySceneNode->setScale(scale);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::initialize()
{
	myFontMng = new FontManager();
	myTexMng = new TextureManager();

	//myUI = new UIManager();
	//myUI->initialize();

	myGpu = new GpuManager();
	myGpu->initialize();

	mySceneManager = new SceneManager(myGpu);
	myEffectManager = new EffectManager(myGpu);
	myMeshManager = new MeshManager(myGpu, myEffectManager);

	myMeshManager->loadMesh("screwdriver", "../../data/meshes/screwdriver.ply", MeshManager::MeshFormatPly);

	Mesh* mesh = myMeshManager->getMesh("screwdriver");

	mySceneManager->initialize();

	//addObject(mesh, Vector3f(0, 0.2, -0.5f));
	//addObject(mesh, Vector3f(0, -0.2, -0.5f));
	addEntity(mesh, Vector3f(0, 0, -0.2f));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::addEntity(Mesh* m, const Vector3f& position)
{
	Entity* e = new Entity(mySceneManager, m, position);
	myEntities.push_back(e);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const InputEvent& evt)
{
	switch(evt.serviceType)
	{
	case InputService::Pointer:
		if(evt.type == InputEvent::Down)
		{
			Ray ray = GfxUtils::getViewRay(Vector2f(evt.position[0], evt.position[1]));

			VectorIterator<std::list<Entity*> > it(myEntities.begin(), myEntities.end());
			while(it.hasMoreElements())
			{
				Entity* e = it.getNext();
				Vector3f handlePos;
				if(e->hit(ray, &handlePos))
				{
					e->activate(handlePos);
					if(myActiveEntity != NULL) myActiveEntity->deactivate();
					myActiveEntity = e;
					break;
				}
			}
		}
		else if(evt.type == InputEvent::Up)
		{
			if(myActiveEntity != NULL)
			{
				myActiveEntity->deactivate();
			}
			myActiveEntity = NULL;
		}
		else if(evt.type == InputEvent::Move)
		{
			if(myActiveEntity != NULL)
			{
				Ray ray = GfxUtils::getViewRay(Vector2f(evt.position[0], evt.position[1]));

				if((evt.flags & InputEvent::Left) == InputEvent::Left)
				{
					myActiveEntity->manipulate(Entity::Move, ray);
				}
				else if((evt.flags & InputEvent::Right) == InputEvent::Right)
				{
					myActiveEntity->manipulate(Entity::Rotate, ray);
				}
				else if((evt.flags & InputEvent::Middle) == InputEvent::Middle)
				{
					myActiveEntity->manipulate(Entity::Scale, ray);
				}

			}
		}
		break;
	case InputService::Touch:
	break;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::handleUIEvent(const UIEvent& evt)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::update(const UpdateContext& context)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshViewerClient::draw(const DrawContext& context)
{
	if(context.layer == 1)
	{
		// Draw UI.
	}
	else
	{
		// We don't use lighting for this application.
		//glDisable(GL_FOG);

		GfxUtils::setLightingEnabled(false);
		GfxUtils::setLightEnabled(0, true);
		GfxUtils::setLightColor(0, Color(1.0, 1.0, 1.0));
		GfxUtils::setLightPosition(0, Vector3f(0, 1, 0));

		glEnable(GL_DEPTH_TEST);

		mySceneManager->draw();

		//glEnable(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE0);

		//glDisable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
void main(int argc, char** argv)
{
	MeshViewerApplication app;
	omain(app, argv[1], "meshviewer-log.txt");
}
