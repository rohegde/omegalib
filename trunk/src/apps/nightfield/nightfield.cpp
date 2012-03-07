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

#define EVL_DEMO

///////////////////////////////////////////////////////////////////////////////////////////////////
Settings::Settings():
    numAgents(2000),
    areaMin(Vector3f(-0.4f, 0.7f, -1.4f)),
    areaMax(Vector3f(0.8f, 1.9f, -2.6f)),
    center(0.0f, 0.0f, 0.0f),

    minAvoidanceDist(0), maxAvoidanceDist(1),
    minCoordinationDist(0), maxCoordinationDist(1),
    minFriction(0), maxFriction(1)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::load(const Setting& s)
{
    const Setting& sPreset = s["preset"];

    Preset* p = new Preset();
    loadPreset(p, sPreset);
    presets.push_back(p);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::loadPreset(Preset* p, const Setting& s)
{
    p->friction = s["friction"];
    p->avoidanceDist = s["avoidanceDist"];
    p->coordinationDist = s["coordinationDist"];
    p->flockImage = String((const char*)s["flockImage"]);
    if(s.exists("fragmentShader"))
    {
        p->fragmentShader = String((const char*)s["fragmentShader"]);
        p->hasFragmentShader = true;
    }
    else
    {
        p->hasFragmentShader = false;
    }
    p->useAdditiveAlpha = s["useAddictiveAlpha"];
    p->drawSpeedVectors = s["drawSpeedVectors"];

    if(s.exists("pointSize"))
    {
        p->pointSize = s["pointSize"];
    }
    else
    {
        p->pointSize = 32;
    }

    if(s.exists("trailSize"))
    {
        p->trailSize = s["trailSize"];
    }
    else
    {
        p->trailSize = 16;
    }

    p->speedVectorScale = s["speedVectorScale"];

    p->useFog = s["useFog"];


    Setting& sc = s["speedVectorColor"];
    p->speedVectorColor = Color(sc[0], sc[1], sc[2], sc[3]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
AffectorEntity::AffectorEntity(RenderableSceneObject* object, ServerEngine* server):
    myObject(object),
    myServer(server),
    myVisible(false),
    myEnabled(true),
    mySelected(false),
    myRotating(false),
    myRotationSpeed(0.2f)
{
    mySelectionSphere = new BoundingSphere();
    mySelectionSphere->setDrawOnSelected(false);
    mySelectionSphere->setVisible(false);

    mySceneNode = new SceneNode(server);
    mySceneNode->setSelectable(true);
    server->getScene()->addChild(mySceneNode);
    mySceneNode->addObject(myObject);
    mySceneNode->addObject(mySelectionSphere);

    // Create the rendering effect for this entity.
    MultipassEffect* mpfx = new MultipassEffect();
    myObject->setEffect(mpfx);

    Effect* wirefx = new Effect();
    wirefx->setDrawMode(Effect::DrawWireframe);
    wirefx->setLightingEnabled(false);
    wirefx->setColor(Color(1,1,1,0.03f));
    wirefx->setForcedDiffuseColor(true);
    wirefx->setBlendMode(Effect::BlendAdditive);
    wirefx->setDepthTestMode(Effect::DepthTestDisabled);

    Effect* basefx = new Effect();
    basefx->setDrawMode(Effect::DrawSmooth);
    basefx->setLightingEnabled(true);
    basefx->setColor(Color(0.5f, 0.5f, 0.7f, 0.5f));
    basefx->setShininess(16.0f);
    basefx->setBlendMode(Effect::BlendAdditive);
    basefx->setForcedDiffuseColor(true);

    mpfx->addEffect(basefx);
    mpfx->addEffect(wirefx);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
AffectorEntity::~AffectorEntity()
{
    myServer->getScene()->removeChild(mySceneNode);
    delete mySceneNode;
    mySceneNode = NULL;

    delete mySelectionSphere;
    mySelectionSphere = NULL;

    myObject = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::setup(const Setting& setting)
{
    if(setting.exists("isAffector"))
    {
        myEnabled = setting["isAffector"];
    }
    if(setting.exists("position"))
    {
        const Setting& sPos = setting["position"];
        mySceneNode->setPosition(sPos[0], sPos[1], sPos[2]);
    }
    myInteractive = false;
    if(setting.exists("interactive"))
    {
        myInteractive = setting["interactive"];
        myRotating = myInteractive;
    }

    if(!myInteractive) mySceneNode->setSelectable(false);
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
    mySceneNode->setPosition(0, 0, 0.0f);
    mySceneNode->setScale( 1.0 , 1.0 , 1.0 );
    mySceneNode->resetOrientation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::select()
{
    mySelected = true;
    mySelectionSphere->setVisible(true);
    mySceneNode->setSelected(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::deselect()
{
    mySelected = false;
    mySelectionSphere->setVisible(false);
    mySceneNode->setSelected(false);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::updateFlockAffector(FlockAffector* af)
{
    const Sphere& bs = mySceneNode->getBoundingSphere();
    af->x = bs.getCenter().x();
    af->y = bs.getCenter().y();
    af->z = bs.getCenter().z();
    af->rx = bs.getRadius();
    af->f1 = 2.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::update(const UpdateContext& context)
{
    if(myRotating)
    {
        mySceneNode->yaw(myRotationSpeed * context.dt);
    }
    if(mySelected)
    {
        ofmsg("|Affector position: %1%", %mySceneNode->getPosition());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AffectorEntity::handleEvent(const Event& evt)
{
    if( evt.getServiceType() == Service::Keyboard )
    {
        if(evt.isKeyDown('i')) 
        {
            Vector3f p = mySceneNode->getPosition();
            p[2] += 0.1f;
            mySceneNode->setPosition(p);
        }
        else if(evt.isKeyDown('k')) 
        {
            Vector3f p = mySceneNode->getPosition();
            p[2] -= 0.1f;
            mySceneNode->setPosition(p);
        }
        else if(evt.isKeyDown('l')) 
        {
            myRotating = !myRotating;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Nightfield::Nightfield()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Nightfield::initialize()
{
    mySelectedEntity = NULL;

    Config* cfg = getServer()->getSystemManager()->getAppConfig();
    if(cfg->exists("config"))
    {
        Setting& sCfg = cfg->lookup("config");
        mySettings.load(sCfg);
    }

    SceneNode* scene = getServer()->getScene();

    mySceneNode = new SceneNode(getServer());

    myFlock = new Flock();
    
    scene->addChild(mySceneNode);
    mySceneNode->addObject(myFlock);
    myFlock->setup(&mySettings);
    myFlock->initialize();

    myInteractor = new DefaultMouseInteractor();
    //myInteractor = new ControllerManipulator();
	ModuleServices::addModule(myInteractor);

    // Create a reference box around the scene.
    if(cfg->exists("config/referenceBox"))
    {
        myReferenceBox = new ReferenceBox();
        scene->addObject(myReferenceBox);
        myReferenceBox->setSize(Vector3f(4.0f, 4.0f, 4.0f));
        myReferenceBox->setColor(ReferenceBox::Back, Color(0.8f, 0.8f, 0.8f));
        myReferenceBox->setup(cfg->lookup("config/referenceBox"));
    }

    if(cfg->exists("config/entities"))
    {
        Setting& entities = cfg->lookup("config/entities");
        for(int i = 0; i < entities.getLength(); i++)
        {
            Setting& entitySetting = entities[i];

            if(entitySetting.exists("mesh"))
            {
                Mesh* m = new Mesh();

                String meshFilename = String((const char*)entitySetting["mesh"]);

                if(StringUtils::endsWith(meshFilename, "ply"))
                {
                    PlyDataReader* reader = new PlyDataReader();
                    if(!reader->readPlyFile(meshFilename))
                    {
                        ofwarn("Could not load mesh file %1%.", %meshFilename);
                    }
                    else
                    {
                        if(entitySetting.exists("scale"))
                        {
                            reader->scale(entitySetting["scale"]);
                        }
                        AffectorEntity* af = new AffectorEntity(m, getServer());
                        af->setup(entitySetting);
                        myEntities.push_back(af);
                        m->setData(reader);
                    }
                }
            }
        }
    }
    Light::setAmbientLightColor(Color(0.3f, 0.3f, 0.3f));

    Light* light = Light::getLight(0);
    light->setEnabled(true);
    light->setColor(Color(0.6f, 0.6f, 0.6f));
    light->setPosition(Vector3f(0, 3, 3));

    getServer()->getDisplaySystem()->setBackgroundColor(Color::Black);

    // Get the default camera and focus in on the scene root
    Camera* cam = getServer()->getDefaultCamera();
    cam->focusOn(getServer()->getScene());

    myTabletManager = getServer()->getServiceManager()->findService<PortholeTabletService>("PortholeTabletService");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Nightfield::update(const UpdateContext& context)
{
    myTabletManager->update(context);
    int i = 0;
    foreach(AffectorEntity* ae, myEntities)
    {
        ae->update(context);
        if(ae->isEnabled())
        {
            FlockAffector* fa = myFlock->getAffector(i);
            ae->updateFlockAffector(fa);
            i++;
        }
    }
    myFlock->setActiveAffectors(i);
    myFlock->update(context);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Nightfield::handleEvent(const Event& evt)
{
    myTabletManager->handleEvent(evt);
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
            Ray r(evt.getExtraDataVector3(0), evt.getExtraDataVector3(1));
            std::pair<bool, float> pt = r.intersects(Plane(Vector3f(0, 0, 1), 0));
            Vector3f newCenter = r.getPoint(pt.second);
            myFlock->getSettings()->center = newCenter;
        }
#ifdef EVL_DEMO
        if(evt.getSourceId() > 0)
          {
            Ray r(evt.getExtraDataVector3(0), evt.getExtraDataVector3(1));
            std::pair<bool, float> pt = r.intersects(Plane(Vector3f(0, 0, 1), 0));
            Vector3f newCenter = r.getPoint(pt.second);
            myFlock->getSettings()->center = newCenter;
          }
#endif
    }
    if(mySelectedEntity != NULL)
    {
        mySelectedEntity->handleEvent(evt);
        // When an entity is selected, the flock centers itself on it.
        myFlock->getSettings()->center = mySelectedEntity->getSceneNode()->getPosition();
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
    const SceneQueryResultList& sqrl = getServer()->querySceneRay(ray);
    if(sqrl.size() != 0)
    {
        // The ray intersected with something.
        SceneNode* sn = sqrl.front().node;
        AffectorEntity* e = findEntity(sn);

        if(mySelectedEntity != e)
        {
            if(mySelectedEntity != NULL)
            {
                mySelectedEntity->deselect();
            }
            // The selected entity changed.
            myInteractor->setSceneNode(sn);
            mySelectedEntity = e;
            mySelectedEntity->select();
        }
    }
    else
    {
        if(mySelectedEntity != NULL)
        {
            mySelectedEntity->deselect();
            mySelectedEntity = NULL;
            myInteractor->setSceneNode(NULL);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
    OmegaToolkitApplication<Nightfield> app("nightfeld");
    return omain(app, argc, argv);
}
