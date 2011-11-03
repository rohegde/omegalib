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
#include "omega/DataManager.h"
#include "omega/FilesystemDataSource.h"

using namespace omega;

DataManager* DataManager::mysInstance = NULL;
	
///////////////////////////////////////////////////////////////////////////////////////////////////
bool DataManager::findFile(const String& name, String& outPath)
{
	DataInfo di = mysInstance->getInfo(name);
	if(!di.isNull())
	{
		outPath = di.path;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
DataManager::DataManager()
{
	if(mysInstance != NULL)
	{
		oerror("DataManager: creating multiple instances not allowed.");
	}
	mysInstance = this;
	myCurrentPath = new FilesystemDataSource("[cwd]");
	addSource(myCurrentPath);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DataManager::setCurrentPath(const String& path)
{
	myCurrentPath->setPath(path);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
String DataManager::getCurrentPath()
{
	return myCurrentPath->getPath();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DataManager::addSource(DataSource* source)
{
	mySources[source->getName()] = source;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DataManager::removeSource(DataSource* source)
{
	mySources.erase(source->getName());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
DataSource* DataManager::getSource(const String& name)
{
	return mySources[name];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
DataInfo DataManager::getInfo(const String& path)
{
	foreach(SourceDictionary::Item ds, mySources)
	{
		if(ds->exists(path))
		{
			return ds->getInfo(path);
		}
	}
	// Data stream not found, return a null data info.
	return DataInfo();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
DataStream* DataManager::createStream(const String& path)
{
	DataStream* stream = NULL;
	foreach(SourceDictionary::Item ds, mySources)
	{
		stream = ds->newStream(path);
		if(stream != NULL) break;
	}
	return stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
DataStream* DataManager::openStream(const String& path, DataStream::Mode mode)
{
	DataStream* stream = createStream(path);
	if(stream != NULL)
	{
		stream->open(mode);
	}
	return stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DataManager::deleteStream(DataStream* stream)
{
	if(stream->isOpen()) stream->close();
	delete stream;
}

