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
#ifndef __POINTER_H__
#define __POINTER_H__

#include "Renderable.h"

namespace omega {
	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API Pointer: public RenderableFactory
	{
	friend class PointerRenderable;
	public:
		enum PointerMode { ModeMouse, ModeWand };

	public:
		Pointer(): 
 		  myVisible(true),
		  myColor(0.1f, 0.1f, 0.2f, 0.95f),
		  myMode(ModeMouse),
		  myText("Pointer"),
		  myPointerSize(0.01f),
		  myPointerLength(4.0f) {}

		virtual Renderable* createRenderable();

		void setVisible(bool value);
		bool getVisible();

		void setRay(const Ray& value);
		const Ray& getRay();

		PointerMode getPointerMode();
		void setPointerMode(PointerMode value);

		void setPosition(const Vector2i& position);
		void setPosition(int x, int y);
		Vector2i getPosition();

		Color getColor();
		void setColor(const Color& value);

		String getText();
		void setText(const String& value);
		
		void setLength(float value) { myPointerLength = value; }
		void setSize(float value) { myPointerSize = value; }

	private:
		bool myVisible;
		Color myColor;
		Vector2i myPosition;
		Ray myRay;
		String myText;
		PointerMode myMode;
		
		float myPointerLength;
		float myPointerSize;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	class OMEGA_API PointerRenderable: public Renderable
	{
	public:
		PointerRenderable(Pointer* pointer): 
		  myPointer(pointer)
		{}
		void draw(const DrawContext& context);

	private:
		Pointer* myPointer;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Pointer::setVisible(bool value)
	{ myVisible = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Pointer::getVisible()
	{ return myVisible; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Color Pointer::getColor()
	{ return myColor; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Pointer::setColor(const Color& value)
	{ myColor = value;}

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Pointer::setPosition(const Vector2i& position)
	{ myPosition = position; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Pointer::setPosition(int x, int y)
	{ myPosition = Vector2i(x, y); }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Vector2i Pointer::getPosition()
	{ return myPosition; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline String Pointer::getText()
	{ return myText; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Pointer::setText(const String& value)
	{ myText = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Pointer::setRay(const Ray& value)
	{ myRay = value; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline const Ray& Pointer::getRay()
	{ return myRay; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline Pointer::PointerMode Pointer::getPointerMode()
	{ return myMode; }

	///////////////////////////////////////////////////////////////////////////////////////////////
	inline void Pointer::setPointerMode(Pointer::PointerMode value)
	{ myMode = value; }
}; // namespace omega

#endif