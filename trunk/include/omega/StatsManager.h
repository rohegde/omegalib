/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2012, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#ifndef __STATS_MANAGER__
#define __STATS_MANAGER__

#include "omega/osystem.h"

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API Stat
	{
	friend class StatsManager;
	public:
		const String& getName();
		bool isValid();
		void addSample(float sample);
		int getNumSamples();
		float getCur();
		float getMin();
		float getMax();
		float getAvg();
		float getTotal();

	private:
		Stat(const String& name): myName(name), myValid(false), myNumSamples(0) {}

	private:
		bool myValid;
		String myName;
		float myCur;
		float myMin;
		float myMax;
		float myAvg;
		int myNumSamples;
		float myAccumulator;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API StatsManager
	{
	public:
		StatsManager();

		Stat* createStat(const String& name);
		Stat* findStat(const String& name);
		List<Stat*>::Range getStats();

	private:
		Dictionary<String, Stat*> myStatDictionary;
		List<Stat*> myStatList;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Stat::addSample(float sample)
	{
		if(!myValid)
		{
			// First sample. Initialize the statistics
			myAccumulator = sample;
			myCur = sample;
			myMin = sample;
			myMax = sample;
			myAvg = sample;
			myNumSamples = 1;
			myValid = true;
		}
		else
		{
			// Update the statistics.
			myAccumulator += sample;
			myCur = sample;
			myNumSamples++;
			if(sample < myMin) myMin = sample;
			if(sample > myMax) myMax = sample;
			myAvg = myAccumulator / myNumSamples;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const String& Stat::getName()
	{ return myName; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Stat::isValid()
	{ return myValid; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline int Stat::getNumSamples()
	{ return myNumSamples; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Stat::getCur()
	{ oassert(myValid); return myCur; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Stat::getMin()
	{ oassert(myValid); return myMin; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Stat::getMax()
	{ oassert(myValid); return myMax; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Stat::getAvg()
	{ oassert(myValid); return myAvg; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline float Stat::getTotal()
	{ oassert(myValid); return myAccumulator;	}
}; // namespace omega

#endif