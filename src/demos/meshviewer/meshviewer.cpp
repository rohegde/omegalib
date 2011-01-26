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
class MeshViewerClient: public ApplicationClient, IUIEventHandler
{
public:
	MeshViewerClient(Application* app): ApplicationClient(app), myGpu(NULL), myActiveNode(NULL) {}

	virtual void initialize();
	virtual bool handleEvent(const InputEvent& evt);
	virtual void handleUIEvent(const UIEvent& evt);
	virtual void update(const UpdateContext& context);
	virtual void draw(const DrawContext& context);

private:
	void addObject(Mesh* m, const Vector3f& position);

private:
	// Managers
	GpuManager* myGpu;
	TextureManager* myTexMng;
	FontManager* myFontMng;
	SceneManager* mySceneManager;
	MeshManager* myMeshManager;
	EffectManager* myEffectManager;
	UIManager myUI;

	// Active object.
	SceneNode* myActiveNode;

	Vector3f myHandlePosition;
	Sphere myStartBSphere;
	bool myMoving;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MeshViewerApplication: public Application
{
public:
	virtual ApplicationClient* createClient() { return new MeshViewerClient(this); }
};

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

	addObject(mesh, Vector3f(0, 0.2, -0.5f));
	addObject(mesh, Vector3f(0, -0.2, -0.5f));
	addObject(mesh, Vector3f(0, 0, -0.5f));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MeshViewerClient::handleEvent(const InputEvent& evt)
{
	switch(evt.serviceType)
	{
	case InputService::Pointer:
		if(evt.type == InputEvent::Down)
		{
			myMoving = true;

			RaySceneQuery query(mySceneManager);

			Vector3f origin;
			Vector3f direction;
			GfxUtils::getViewRay(evt.position[0], evt.position[1], &origin, &direction);

			Ray ray(origin, direction);
			query.setRay(ray);

			const SceneQueryResultList& rl = query.execute();
			if(rl.size() > 0)
			{
				if(myActiveNode != NULL)
				{
					myActiveNode->setBoundingBoxVisible(false);
				}
				myActiveNode = rl.front().node;
				myActiveNode->setBoundingBoxVisible(true);
				myStartBSphere = myActiveNode->getBoundingSphere();
				
				std::pair<bool, float> res = Math::intersects(ray, myStartBSphere);
				oassert(res.first);

				myHandlePosition = ray.getPoint(res.second) - myStartBSphere.getCenter();
			}
			else
			{
				if(myActiveNode != NULL)
				{
					myActiveNode->setBoundingBoxVisible(false);
				}
				myActiveNode = NULL;
			}

			//printf("orig: %f %f %f, dir %f %f %f numresults: %d\n", origin[0], origin[1], origin[2], 
			//	direction[0], direction[1], direction[2], rl.size());
		}
		else if(evt.type == InputEvent::Up)
		{
			if(myActiveNode != NULL)
			{
				myActiveNode->setBoundingBoxVisible(false);
			}
			myActiveNode = NULL;
		}
		else if(evt.type == InputEvent::Move)
		{
			if(myActiveNode != NULL)
			{
				Vector3f origin;
				Vector3f direction;
				GfxUtils::getViewRay(evt.position[0], evt.position[1], &origin, &direction);

				// Interstect the ray with the Z plane where the handle lies, to get
				// the new handle position.
				float tz = myHandlePosition[2] + myStartBSphere.getCenter()[2];
				float l = (tz - origin[2]) / direction[2];
				float tx = origin[0] + l * direction[0];
				float ty = origin[1] + l * direction[1];

				Vector3f newPos = Vector3f(tx, ty, tz);

				if((evt.flags & InputEvent::Left) == InputEvent::Left)
				{
					// Compute the new object position using the handle offset.
					newPos = newPos - myHandlePosition;
					myActiveNode->setPosition(newPos);
				}
				else if((evt.flags & InputEvent::Right) == InputEvent::Right)
				{
					const Vector3f& rot = myActiveNode->getRotation();

					// Compute projection of bounding sphere center to ray.
					Ray ray = Ray(origin, direction);
					Vector3f rproj = ray.projectPoint(myStartBSphere.getCenter());

					// Project point back onto sphere.
					Vector3f sproj = myStartBSphere.projectPoint(rproj);
					sproj.normalize();

					Quaternion q2 = Quaternion(sproj);

					//myActiveNode->setRotation(rot[0] + delta[0], rot[1] + delta[1], rot[2]);
				}
				else if((evt.flags & InputEvent::Middle) == InputEvent::Middle)
				{
					// Compute new scale.
					//const Sphere& bs = myActiveNode->getBoundingSphere();
					//float d = (bs.getCenter() - myStartPosition).length();
					//float r = bs.getRadius();

					//float td = (bs.getCenter() - newPos).length();
					//float scale = (td / d);

					//printf("%f %f %f\n", d, r, scale);

					//myActiveNode->setScale(scale);
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
void MeshViewerClient::addObject(Mesh* m, const Vector3f& position)
{
	SceneNode* node = new SceneNode(mySceneManager);
	node->addDrawable(m);
	node->setPosition(position);
	node->setSelectable(true);
	mySceneManager->getRootNode()->addChild(node);
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
