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
#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <oengine.h>

#include <iostream>
#include <fstream>

#include "SceneManager.h"
#include "SceneLoader.h"

using namespace omega;
using namespace oengine;
using namespace oosg;
using namespace osg;
using namespace cyclops;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Railsim: public EngineServer
{
public:
	Railsim(Application* app): EngineServer(app) {}
	virtual void initialize();
	virtual void update(const UpdateContext& context);
	virtual void handleEvent(const Event& evt);
	void updateEntity( int entity , vector<float> pos , vector<float> rot, int curTimeStep );
	bool loadDataSets( String configAttr , vector<float> &dataVector , bool pos );
private:
	SceneManager* mySceneManager;

	Vector3f myCenter;

	float animationTimer;
	float curTime;

	bool rotation;
	bool animation;
	bool ending;

	//general animation info
	int numTimeSteps;

	// animation info
	vector<float> wheel_PVec;
	vector<float> frame_PVec;
	vector<float> railFail_PVec;

	vector<float> wheel_RVec;
	vector<float> frame_RVec;
	vector<float> railFail_RVec;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
void Railsim::initialize()
{
	EngineServer::initialize();
	Config* cfg = getSystemManager()->getAppConfig();

	removeAllRenderPasses();

	//Create a sceneManager for the scene
	mySceneManager = new SceneManager();
	mySceneManager->initialize(this);

	myCenter = Vector3f::Zero();

	// Add a new data path.
	if(cfg->exists("config/dataPath"))
	{
		String dataPath = String((const char*)cfg->lookup("config/dataPath"));
		DataManager* dm = SystemManager::instance()->getDataManager();
		dm->addSource(new FilesystemDataSource(dataPath));
	}

	// Load osg object
	if(cfg->exists("config/scene"))
	{
		String mapName = String((const char*)cfg->lookup("config/scene"));

		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo cfgInfo = dm->getInfo(mapName);
		if(!cfgInfo.isNull())
		{
			TiXmlDocument doc(cfgInfo.path.c_str());
			if(doc.LoadFile())
			{
				ofmsg("Loading map: %1%...", %mapName);

				SceneLoader* sl = new SceneLoader(doc);
				mySceneManager->load(sl);
			}
		}
		else
		{
			ofwarn("!File not found: %1%", %mapName);
		}
	}

	// Load the data sets
	bool gotAllData = true;
	ofmsg("Loading data sets: ");

	gotAllData = loadDataSets( "config/wheelPos" , wheel_PVec , true);
	numTimeSteps = (int) ((wheel_PVec.size() - 1) / 4.0);

	gotAllData = loadDataSets( "config/wheelRot" , wheel_RVec , false);
	numTimeSteps = min( numTimeSteps , (int) ((wheel_RVec.size() - 1) / 4.0) );

	gotAllData = loadDataSets( "config/framePos" , frame_PVec, true);
	numTimeSteps = min( numTimeSteps , (int) ((frame_PVec.size() - 1) / 4.0) );

	gotAllData = loadDataSets( "config/frameRot" , frame_RVec , false);
	numTimeSteps = min( numTimeSteps , (int) ((frame_RVec.size() - 1) / 4.0) );

	gotAllData = loadDataSets( "config/railFailPos" , railFail_PVec, true);
	numTimeSteps = min( numTimeSteps , (int) ((railFail_PVec.size() - 1) / 4.0) );

	gotAllData = loadDataSets( "config/railFailRot" , railFail_RVec, false);
	numTimeSteps = min( numTimeSteps , (int) ((railFail_RVec.size() - 1) / 4.0) );

	if(!gotAllData ) ofwarn("!Some data failed to load");
	else ofmsg("Data loaded fine.");

	curTime = 0;
	animationTimer = 5.0;
	rotation = true;
	animation = true;
	ending = false;

	//printf( "numTimeSteps: %d\n" , numTimeSteps );
	//for( int x = 0 ; x < numTimeSteps ; x ++ )
	//{
	//	printf( "%d: %f , %f , %f , %f\n" , x , railFail_RVec[x*4 + 0] , railFail_RVec[x*4 + 1], railFail_RVec[x*4 + 2], railFail_RVec[x*4 + 3] );
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Railsim::loadDataSets( String configAttr , vector<float> &dataVector , bool pos )
{
	//get the config file
	Config* cfg = getSystemManager()->getAppConfig();

	//Load the wheel position
	if(cfg->exists(configAttr))
	{
		String mapName = String((const char*)cfg->lookup(configAttr));

		DataManager* dm = SystemManager::instance()->getDataManager();
		DataInfo cfgInfo = dm->getInfo(mapName);
		if(!cfgInfo.isNull())
		{
			ofmsg("\tLoading map: %1%...", %configAttr);
 
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
				ofwarn("!Unable to open file : %1%", %mapName);	//if the file is not open output
				return false;
			}
		}
		else
		{
			ofwarn("!File not found: %1%", %mapName);
			return false;
		}
		ofmsg("\t Loaded.");
		return true;
	}
	else
	{
		ofwarn("\t!Data failed to load %1% ... " , %configAttr);
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Railsim::update(const UpdateContext& context) 
{
	EngineServer::update(context);
	mySceneManager->update(context);

	if( (curTime < animationTimer && animation) || ending )
	{
		vector<float> pos;
		vector<float> rot;
		float ratio = curTime / (animationTimer );
		int curTimeStep = (int)(numTimeSteps * ratio);		
		
		//printf("\n\n %d : " , curTimeStep );

		if( ending ) curTimeStep = numTimeSteps;

		updateEntity ( 0 , railFail_PVec , railFail_RVec, curTimeStep );
		updateEntity ( 1 , wheel_PVec , wheel_RVec, curTimeStep ); 
		updateEntity ( 2 , frame_PVec , frame_RVec, curTimeStep ); 
		curTime += context.dt;
	}
	else curTime = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Railsim::updateEntity( int entity , vector<float> pos , vector<float> rot, int curTimeStep ) 
{
	int actualVecIndex = curTimeStep * 4;
	
	vector<float> position;
	position.push_back( pos[actualVecIndex+0]);
	position.push_back( pos[actualVecIndex+1]);
	position.push_back( pos[actualVecIndex+2]);
	position.push_back( pos[actualVecIndex+3]);
	mySceneManager->updateEntityPos( entity , position );

	if( rotation)
	{
		vector<float> rotation;
		rotation.push_back( rot[actualVecIndex+0]);
		rotation.push_back( rot[actualVecIndex+1]);
		rotation.push_back( rot[actualVecIndex+2]);
		rotation.push_back( rot[actualVecIndex+3]);
		mySceneManager->updateEntityRot( entity , rotation );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Railsim::handleEvent(const Event& evt) 
{
	if(evt.isKeyDown('R'))
    {
		rotation = !rotation;
	}

	else if ( evt.isKeyDown('A'))
	{
		animation = !animation;
	}
	else if ( evt.isKeyDown('E'))
	{
		ending = !ending;
	}
	EngineServer::handleEvent(evt);
	mySceneManager->handleEvent(evt);

	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Application entry point
int main(int argc, char** argv)
{
	EngineApplication<Railsim> app;

	// Read config file name from command line or use default one.
	const char* cfgName = "railsim.cfg";
	if(argc == 2) cfgName = argv[1];

	omain(app, cfgName, "railsim.log", new FilesystemDataSource(OMEGA_DATA_PATH));

	return 0;
}
