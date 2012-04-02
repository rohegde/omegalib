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
	OmegaViewer();

	//Pointer to the SceneManger
	SceneManager* sceneMngr;						

	//Pointers to the entities in the SceneManger
	Entity* wheelSetEntity;						
	Entity* frameEntity;						
	Entity* railFailEntity;						

	virtual void initialize();
	virtual void update(const UpdateContext& context);
	virtual void handleEvent(const Event& evt);

	//setup methods for camera behavior
	void setupCameraBehavior( );
	void camTrans( Vector3f pos );
	void camRot( Vector3f pitchYawRoll );

	//the data sets
	void initializeData();
	bool loadData( String configAttr , Vector<float> &dataVector , bool pos );

	//updating a specific entity
	void updateEntity( Entity* entity , vector<float> pos , vector<float> rot, int curTimeStep );
	void updateCamera( vector<float> pos , vector<float> rot, int curTimeStep );

	//animation info data structure
	Vector<float> wheel_PVec;
	Vector<float> frame_PVec;
	Vector<float> railFail_PVec;

	Vector<float> wheel_RVec;
	Vector<float> frame_RVec;
	Vector<float> railFail_RVec;

	//animation varaibles
	float animationTimer;
	float curTime;
	int numTimeSteps;

	//scene flags
	bool isAnimating;
	bool isFreeFly;
	bool isFollowing;

};

///////////////////////////////////////////////////////////////////////////////////////////////////
OmegaViewer::OmegaViewer()
{
	//Instanciate an instance of ScenenManager
	sceneMngr = new SceneManager();

	//Registed the sceneMngr module
	ModuleServices::addModule(sceneMngr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void OmegaViewer::initializeData()
{
	// Load the data sets
	String data;
	bool gotAllData = true;

	numTimeSteps = INT_MAX;
	
	data = "config/wheelPos";
	gotAllData = loadData( data , wheel_PVec , true);
	if(!gotAllData ) ofwarn("!Some data failed to load %1%" , %data);

	data = "config/wheelRot";
	gotAllData = loadData( data , wheel_RVec , false);
	if(!gotAllData ) ofwarn("!Some data failed to load %1%" , %data);

	data = "config/framePos";
	gotAllData = loadData( data , frame_PVec, true);
	if(!gotAllData ) ofwarn("!Some data failed to load %1%" , %data);

	data = "config/frameRot";
	gotAllData = loadData( data , frame_RVec , false);
	if(!gotAllData ) ofwarn("!Some data failed to load %1%" , %data);

	data = "config/railFailPos";
	gotAllData = loadData( data , railFail_PVec, true);
	if(!gotAllData ) ofwarn("!Some data failed to load %1%" , %data);

	data = "config/railFailRot";
	gotAllData = loadData( data , railFail_RVec, false);
	if(!gotAllData ) ofwarn("!Some data failed to load %1%" , %data);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool OmegaViewer::loadData( String configAttr , Vector<float> &dataVector , bool pos )
{
	//flag to determine success
	bool successful = false;

	//get the config file
	Config* cfg = SystemManager::instance()->getAppConfig();

	//Load the wheel position
	if(cfg->exists(configAttr))
	{
		String mapName = String((const char*)cfg->lookup(configAttr));

		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo cfgInfo = dm->getInfo(mapName);
		if(!cfgInfo.isNull())
		{
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
		numTimeSteps = minimum( numTimeSteps , (int) ((dataVector.size() - 1) / 4.0) );
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
	//~~~~~~ Setup the camera stuff 
	isFreeFly = true;
	setupCameraBehavior( );

	//~~~~~~ Form context to scene nodes that represent the .objs
	railFailEntity = sceneMngr->findEntity( 0 );
	if(!railFailEntity) owarn("Rail not loaded");
	else omsg("Rail loaded");

	wheelSetEntity = sceneMngr->findEntity( 1 );
	if(!wheelSetEntity) owarn("Wheel not loaded");
	else omsg("Wheel loaded");

	frameEntity = sceneMngr->findEntity( 2 );
	if(!frameEntity) owarn("Frame not loaded");
	else omsg("Frame loaded");

	initializeData();

	curTime = 0;
	animationTimer = 10.0;
	isAnimating = true;
	isFollowing = false;
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
		isAnimating = !isAnimating;
		ofwarn("Animation set to :: %1%", %isAnimating);
	}

	else if(evt.isKeyDown('c'))
    {
		isFreeFly = !isFreeFly;
		setupCameraBehavior( );
	}
	
	//Set to the Hardcoded origin
	else if(evt.isKeyDown('h'))
    {
		camTrans( Vector3f( 15.0 , 8.0 , 27.0 ) );
		camRot( Vector3f( 25.0 , 0.0 , 0.0 ) );
		isFreeFly = true;
		isFollowing = false;
	}

	//Set to the Hardcoded origin
	else if(evt.isKeyDown('t'))
    {
		//isFreeFly = false;
		isFollowing = true;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::update(const UpdateContext& context) 
{
	if( (curTime < animationTimer && isAnimating) )
	{
		vector<float> pos;
		vector<float> rot;
		float ratio = curTime / (animationTimer );
		int curTimeStep = (int)(numTimeSteps * ratio);		
		
		//printf("\n");
		//printf("\n Time        :: %f " , curTime );
		//printf("\n dt          :: %f " , context.dt );
		//printf("\n curTimeStep :: %d " , curTimeStep );
		//if( isEnd ) curTimeStep = numTimeSteps;

		updateEntity ( railFailEntity , railFail_PVec , railFail_RVec, curTimeStep );
		updateEntity ( wheelSetEntity , wheel_PVec , wheel_RVec, curTimeStep ); 
		updateEntity ( frameEntity , frame_PVec , frame_RVec, curTimeStep ); 
		curTime += context.dt;

		if( isFollowing )
		{
			setupCameraBehavior( );	//will turn off freeFly assuming flag set
			updateCamera( wheel_PVec , wheel_RVec, curTimeStep );
		}
	}
	else if( curTime >= animationTimer ) curTime = 0;	//reset the timer
	else if( !isAnimating ) return;						//do nothing cause animation has paused
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::updateEntity( Entity* entity , vector<float> pos , vector<float> rot, int curTimeStep ) 
{
	int actualVecIndex = curTimeStep * 4;
	Vector<float> position;
	position.push_back( pos[actualVecIndex+0]);
	position.push_back( pos[actualVecIndex+1]);
	position.push_back( pos[actualVecIndex+2]);
	position.push_back( pos[actualVecIndex+3]);
	entity->getSceneNode()->setPosition( position[0] , position[1] , position[2] );

	//!!!!! HACK !!!!!//	Data set has a weird rotation
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
	entity->getSceneNode()->setOrientation( rotation );

}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::updateCamera( vector<float> pos , vector<float> rot, int curTimeStep )
{
	//Hardcoded :: the following location
	int actualVecIndex = curTimeStep * 4;
	Vector<float> position;
	position.push_back( pos[actualVecIndex+0] - 5);
	position.push_back( -0.5 );
	position.push_back( -0.22 );
	camTrans( Vector3f( position[0] , position[1] , position[2] ) );

	//Hardcoded :: the following orientation
	camRot( Vector3f( 0.0, 90.0, 0.0 ) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::camRot( Vector3f pitchYawRoll )
{
	ofwarn("Rotating Camera to :: %1%", %pitchYawRoll);
	getServer()->getDefaultCamera()->setOrientation(pitchYawRoll);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::camTrans( Vector3f pos)
{
	ofwarn("Moving Camera to :: %1%", %pos);
	getServer()->getDefaultCamera()->setPosition(pos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
void OmegaViewer::setupCameraBehavior( )
{
	//enable/disable the free fly
	getServer()->getDefaultCamera()->setControllerEnabled(isFreeFly);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	Application<OmegaViewer> app("orun");
	oargs().newNamedString('s', "script", "script", "script to launch at startup", sDefaultScript);
	return omain(app, argc, argv);
}
