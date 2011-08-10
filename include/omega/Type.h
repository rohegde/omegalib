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
#ifndef __TYPE_H__
#define __TYPE_H__

#include "osystem.h"

#define OMEGA_DECLARE_TYPE(tname) \
	private: \
	static const omega::Type* _type; \
	public: \
	static const omega::Type* getType() { return _type; } 

#define OMEGA_DEFINE_TYPE(tname, tbase) const omega::Type* tname::_type = new omega::Type(#tname, tbase::getType());
#define OMEGA_DEFINE_TYPE_NOBASE(tname) const omega::Type* tname::_type = new omega::Type(#tname, NULL);

namespace omega
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//! The type object is used to implement a very basic run type type information system inside
	//! omegalib.
	class Type
	{
	public:
		Type(const String& name, const Type* base);

		String getName() const;
		const Type* getBaseType() const; 
		bool isDerivedFrom(const Type* base) const;

	private:
		String myName;
		const Type* myBaseType;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Type::Type(const String& name, const Type* base):
		myName(name),
		myBaseType(base)
	{}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline String Type::getName() const
	{ return myName; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Type* Type::getBaseType() const
	{ return myBaseType; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Type::isDerivedFrom(const Type* base) const
	{
		if(base == NULL) return false;
		if(base == this) return true;
		if(myBaseType == NULL) return false;
		return myBaseType->isDerivedFrom(base);
	}
}; // namespace omega

#endif
