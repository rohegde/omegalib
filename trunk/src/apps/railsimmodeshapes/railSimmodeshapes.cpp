/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2011		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *  Dennis Chau				koracas@gmail.com
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
#include <omega.h>
#include <cyclops.h>
#include <cyclops/SceneManager.h>
#include <omegaToolkit.h>
//#include <omegaVtk.h>

#include <iostream>
#include <fstream>

using namespace omega;
using namespace omegaToolkit;
using namespace cyclops;
using namespace std;			//Needed for file reading, but there might be an OmegaLib way
//using namespace omegaVtk;

// The name of the script to launch automatically at startup
String sDefaultScript = "";

///////////////////////////////////////////////////////////////////////////////////////////////////

class OmegaViewer: public ServerModule
{
public:
	//Constructor
	OmegaViewer();

	//Pointer to the SceneManger.  This is impt bc it controlls the scene: camera, entities, etc.
	SceneManager* sceneMngr;						

	//Pointers to the entities in the SceneManger.  These pointers will be assigned later for ease of use.	
	Entity* modeshape;		

	virtual void initialize();
	virtual void update(const UpdateContext& context);
	virtual void handleEvent(const Event& evt);

	//setup methods for camera behavior
	void toggleCameraController( );
	void camTrans( Vector3f pos );
	void camRot( Vector3f pitchYawRoll );
	void camDefault();

	//the data sets
	void initializeData();
	bool loadData( String configAttr , Vector<float> &dataVector , bool pos );

	//updating a specific entity
	void updateEntity( Entity* entity , vector<float> pos , vector<float> rot, int curTimeStep );
	void updateCamera( vector<float> pos , vector<float> rot, int curTimeStep );


	//animation varaibles
	float shapetime;
	int mcount;
	float animationTimer;	//the amount of time the animation will run for in milliseconds
	float animationPadding;	//
	float curTime;			//The current time in milliseconds
	int numTimeSteps;		//The total number of timesteps of data pts in the files
	int curTimeStep;		//The current time step in the current rendering loop as index into array 
	int prevTimeStep;		//The last time step since the rendering loop was called

	//scene flags
	bool isAnimating;		//toggle for if animations is happening or just still image
	bool isKeyFraming;		//toggle that user is "stepping through" the time steps by presssing a key 
	bool isFreeFly;			//Toggle to allow uses to move around using the d-pad and not locked down to a certain view 
	
	//camera control
	int campos;				//camera position toggle
	Vector3f currot;		//camera rotation
	Vector3f curpos;		//camera postion

	//Vector math
	Vector3f Lerp(float t, Vector3f start,Vector3f end);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer::OmegaViewer()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//  read in the data files that are associated with this rigid body simultaion
//	This might be a custom func depending on the file formate given... over time the DSL should give a standard format
void OmegaViewer::initializeData()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//   Load a specific data file
//call loadData by passing in:
//		the data string
//		array to store the data
//		flag to determine if it is position data.  Tuer:pos			False:rot
//returns whether it was successful or not
bool OmegaViewer::loadData( String configAttr , Vector<float> &dataVector , bool pos )
{
	//flag to determine success
	bool successful = false;

	//get a reference of :: /omegaLib/data/railsim.cfg from the SystemManger within OmegaLib
	Config* cfg = SystemManager::instance()->getAppConfig();

	//Check to make sure the "configAttr" passed in is part of this config file 
	if(cfg->exists(configAttr))
	{
		//maps the relative path to the absolute path
		String mapName = String((const char*)cfg->lookup(configAttr));

		//Instantiates a data manager
		DataManager* dm = SystemManager::instance()->getDataManager();
		
		//Uses the dataManager to get the data within the config file bbase on the absolute path 
		DataInfo cfgInfo = dm->getInfo(mapName);

		//if the data is not crap
		if(!cfgInfo.isNull())
		{
			//Parsese the file
			String line;								//this will contain the data read from the file
			ifstream myfile (cfgInfo.path.c_str());		//opening the file.
			if (myfile.is_open())						//if the file is open
			{
				while (! myfile.eof() )					//while the end of file is NOT reached
				{
					getline (myfile,line);				 //get one line from the file
					float x , y , z , w;
					if( pos) sscanf( line.c_str() , "%*s %f %*s %f %*s %f %*s %*s %f" , &x , &y, &z, &w ); //get desired data
					else sscanf( line.c_str() , "%f %f %f %f" , &x , &y, &z, &w ); //get desired data
					dataVector.push_back (x);
					dataVector.push_back (y);
					dataVector.push_back (z);
					dataVector.push_back (w);
					//cout << x << ","<< y << ","<< z << "," << w << endl; //and output it
				}
				myfile.close();							//closing the file
			}
			else 
			{
				ofwarn("Failed to load unable to open file : %1%", %mapName);	//if the file is not open output
				successful = false;
			}
		}
		else
		{
			ofwarn("Failed to load %1% data not found %1%", %mapName);
			successful = false;
		}
		successful = true;
	}
	else
	{
		ofwarn("Failed to load %1% config not found... " , %configAttr);
		successful = false;
	}

	if ( successful )
	{
		//the data set with the smallest number of timesteps determines the scope of the animation
		numTimeSteps = min( numTimeSteps , (int) ((dataVector.size() - 1) / 4.0) );
		ofmsg("Loading data: %1% ... Success!!!", %configAttr);
	}
	return successful;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::initialize()
{
#ifdef OMEGA_USE_PYTHON
	omegaToolkitPythonApiInit();
	cyclopsPythonApiInit();
#endif
	//To deal with clustering
	//Instanciate an instance of ScenenManager
	sceneMngr = SceneManager::createAndInitialize();

	//~~~~~~ Form context to scene nodes that represent the .objs
	//go into the sceneMangr to find the entity 0 as defined by
	// 	/omegalib/data/railsim/test/railSim.scene under the heading <Entities>
	//  Assign the pointers to entities so you do not have to constantly look them up later

//	multimodel shape trial.
	modeshape=sceneMngr->getObject<Entity>("0");
	modeshape->setCurrentModelIndex(0);		
	mcount=0;
	shapetime=0.0;


	//go get the data
	initializeData();

	//Animation var init
	curTime = 0;
	animationTimer = 10.0;
	animationPadding = 1.5;
	
	prevTimeStep = 0;
	curTimeStep = 0;

	//init the camera
	camDefault();
	curpos=Vector3f( 20,-10,20 );
	currot=Vector3f( 20.0, 0.0 , 0.0 );

	//init the lighting
	cyclops::Light* l = new Light(sceneMngr);
	l->setEnabled(true);

	l->setPosition(18, 0, 8);

//	l->setPosition(0, 20, -20);
	l->setColor(Color(1.0f, 1.0f, 1.0f));
	l->setAmbient(Color(0.7f, 0.7f, 0.7f));
/*
	cyclops::Light* l1 = new Light(sceneMngr);
	l->setEnabled(true);

	l->setPosition(18, 0, -8);

//	l->setPosition(0, 20, -20);
	l->setColor(Color(1.0f, 1.0f, 1.0f));
	l->setAmbient(Color(0.7f, 0.7f, 0.7f));

	*/
 	sceneMngr->setMainLight(l);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::handleEvent(const Event& evt)
{
	//if(evt.getServiceType() == Service::UI) 
	//{
	//	handleUIEvent(evt);
	//}
	//else
	//{
	//	EngineClient::handleEvent(evt);
	//}

	if(evt.isKeyDown('A'))
    {
		//isAnimating = !isAnimating;
		//ofwarn("Animation set to :: %1%", %isAnimating);

		isKeyFraming = false;
		//reverse math to figure out where the keyFrame is time wise
		float curRatio = (float)curTimeStep / (float)numTimeSteps;
		curTime = (curRatio * animationTimer) - animationPadding; 
	}

	else if(evt.isKeyDown('c'))
    {
		isFreeFly = !isFreeFly;
		isKeyFraming = false;
	}
	
	//Set to the Hardcoded origin
	else if(evt.isKeyDown('h'))
    {
		camTrans( Vector3f( 15.0 , 15.0 , 27.0 ) );
		camRot( Vector3f( 15.0 , 0.0 , 0.0 ) );
		camDefault();
	}

	//Set to the ride the rail
	else if(evt.isKeyDown('2') || evt.isButtonDown(Event::Button3))
    {
		campos=2;
		sceneMngr->getMainLight()->setPosition(18,0,10.0);
	}

	//Set to the ride the rail
	else if(evt.isKeyDown('3') || evt.isButtonDown(Event::Button4))
    {
		campos=3;
		sceneMngr->getMainLight()->setPosition(32.0,0.0,2.0);
		
	}

		else if(evt.isKeyDown('1') || evt.isButtonDown(Event::Button4))
    {
		campos=1;
		sceneMngr->getMainLight()->setPosition(18,0,10.0);
		
	}

	//Enable Keyframing 
	//Set to the Hardcoded origin
	else if(evt.isKeyDown('o') || evt.isButtonDown(Event::SpecialButton1))
    {
		isKeyFraming = true;
		prevTimeStep = curTimeStep-=1; 
		if(mcount>0){mcount--;} else {mcount=modeshape->getNumModels();}
	}
	//Enable Keyframing 
	else if(evt.isKeyDown('p') || evt.isButtonDown(Event::SpecialButton2))
    {
		isKeyFraming = true;
		prevTimeStep = curTimeStep+=1; 
		if(mcount<modeshape->getNumModels()){mcount++;} else {mcount=0;}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//   The rendering loop that is called thousands of times per sec 
//
void OmegaViewer::update(const UpdateContext& context) 
{
//	sceneMngr->getMainLight()->setPosition(getServer()->getDefaultCamera()->getPosition());
//	cout<<context.time<<endl;
	if(!isFreeFly){
	camTrans(curpos);
	camRot(currot);
	}
	
	switch(campos){
	case 1:
		curpos=Lerp(0.9,curpos,Vector3f( 20,0.0,20));
		currot=Lerp(0.9,currot,  Vector3f( 0.0, 0.0 , 0.0));
		
		break;
	case 2:
		curpos=Lerp(0.9,curpos,Vector3f( 20,-10,20));
		currot=Lerp(0.9,currot,  Vector3f( 20.0, 0.0 , 0.0));
		
		break;
	
	case 3:
		curpos=Lerp(0.9,curpos,Vector3f( 32.0 , 0.0 , 2.0 ));
		currot=Lerp(0.9,currot,  Vector3f( 0.0 , 90.0 , 0.0 ));
		break;
	}

	if( !isKeyFraming ){
	if(context.time>shapetime){
			if(mcount<modeshape->getNumModels()){mcount++;} else {mcount=0;}
			modeshape->setCurrentModelIndex(mcount);
			shapetime=context.time+0.05;
	}
	} else {
	    modeshape->setCurrentModelIndex(mcount);
	}
	//do camrea stuff
	toggleCameraController( );

	//if current time in millisec  is less than the animation time then animate
	if( (curTime < animationTimer && isAnimating) )
	{
		//not keyframing 
		if( !isKeyFraming )
		{
			//determien the ratio of time : current / totat time of animation
			//  % of the animation done in terms of time 
			float ratio = ( curTime + animationPadding) / (animationTimer );
			
			//Determine indea into the array based on this ratio = max num of idex * ratio 
			curTimeStep = (int)(numTimeSteps * ratio);	

			//incr the time based on some dt.
			curTime += context.dt;
		}

		if( curTimeStep > numTimeSteps) curTimeStep = numTimeSteps;

		//printf("\n");
		//printf("\n Time        :: %f " , curTime );
		//printf("\n dt          :: %f " , context.dt );
		//printf("\n curTimeStep :: %d " , curTimeStep );
		
	}
	else if( curTime >= animationTimer ) curTime = 0;	//reset the timer bc the animation should be done
	else if( !isAnimating ) return;						//do nothing cause animation has paused

	prevTimeStep = curTimeStep;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  Updates a given entity based on the position and rot array and the desired index
void OmegaViewer::updateEntity( Entity* entity , vector<float> pos , vector<float> rot, int curTimeStep ) 
{
	//getting the actual vector index bc ea. quad is x, y, z, w for a time step
	int actualVecIndex = curTimeStep * 4;
	
	//grabbign the position quad
	Vector<float> position;
	position.push_back( pos[actualVecIndex+0]);
	position.push_back( pos[actualVecIndex+1]);
	position.push_back( pos[actualVecIndex+2]);
	position.push_back( pos[actualVecIndex+3]);

	//setting the position of the entity
	entity->setPosition( position[0] , position[1] , position[2] );

	//!!!!! HACK !!!!!//	
	//Data set has a weird rotation
	//Quaternion for a 90 deg X-axis rotation
	Quaternion newAxis = AngleAxis(-Math::HalfPi, Vector3f::UnitX()) ;
	Quaternion rotation;
	rotation.x() = rot[actualVecIndex+0];
	rotation.y() = rot[actualVecIndex+1];
	rotation.z() = rot[actualVecIndex+2];
	rotation.w() = rot[actualVecIndex+3];
		
	//90 degree rotation will turn the object side ways
	//Apply the rotation
	rotation = newAxis * rotation;
	entity->setOrientation( rotation );
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::camRot( Vector3f pitchYawRoll )
{
	//ofwarn("Rotating Camera to :: %1%", %pitchYawRoll);
	getServer()->getDefaultCamera()->setYawPitchRoll(pitchYawRoll);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::camTrans( Vector3f pos)
{
	//ofwarn("Moving Camera to :: %1%", %pos);
	getServer()->getDefaultCamera()->setPosition(pos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::camDefault()
{
	isAnimating = true;
	isKeyFraming = false;

	//~~~~~~ Setup the camera stuff 
	isFreeFly = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::toggleCameraController( )
{
	//enable/disable the free fly
	getServer()->getDefaultCamera()->setControllerEnabled(isFreeFly);
}

Vector3f  OmegaViewer::Lerp(float t, Vector3f start,Vector3f end)
{
	return start*t+end*(1-t);
}





///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<OmegaViewer> app("railSimmodeshapes");
	oargs().newNamedString('s', "script", "script", "script to launch at startup", sDefaultScript);
	return omain(app, argc, argv);
}



