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
#ifndef __MODEL_H__
#define __MODEL_H__

#include "omega/osystem.h"

namespace omega { namespace mvc {
	class Model;

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API IModelListener
	{
	public:
		//! This method is used to notify changes in the model state.
		//! @see Model::getState
		virtual void onModelStateChanged(Model* model) {}

		//! This method is used to notify progress in an operation performed on the model.
		//! The operation can be loading or updating of the model and is usually identified
		//! by the model state.
		virtual void onModelOperationProgress(Model* model, int progress) {}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OUTILS_API Model
	{
	OMEGA_DECLARE_TYPE(Model);
	public:
		enum State { StateUnloaded, StateLoading, StateLoaded, StateUpdating, StateUpdated, StateOutOfDate };
	public:
		Model();

		const String& getName() const;
		void setName(const String& value);

		const String& getSource() const;
		void setSource(const String& value);

		const String& getLabel() const;
		void setLabel(const String& value);

		State getState();

		void addListener(IModelListener* listener);
		void removeListener(IModelListener* listener);

	protected:
		void setState(State newState, bool signal = true);
		void signalStateChange();
		void signalOperationProgress(int progress);
		//virtual void load();
		//virtual void unload();

	private:
		String myName;
		String mySource;
		String myLabel;
		State myState;

		List<IModelListener*> myListeners;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Model::State Model::getState()
	{ return myState; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const String& Model::getName() const
	{ return myName; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Model::setName(const String& value)
	{ myName = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const String& Model::getSource() const
	{ return mySource; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Model::setSource(const String& value)
	{ mySource = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const String& Model::getLabel() const
	{ return myLabel; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Model::setLabel(const String& value)
	{ myLabel = value; }
}; }; // namespace omega

#endif
