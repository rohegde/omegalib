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
#include "omega/Color.h"

using namespace omega;

const Color Color::White = Color(1.0f, 1.0f, 1.0f);
const Color Color::Black = Color(0.0f, 0.0f, 0.0f);
const Color Color::Orange = Color(1.0f, 0.65f, 0.0f);
const Color Color::Silver = Color(0.75f, 0.75f, 0.75f);
const Color Color::Gray = Color(0.5f, 0.5f, 0.5f);
const Color Color::Red = Color(1.0f, 0.0f, 0.0f);
const Color Color::Maroon = Color(0.5f, 0.0f, 0.0f);
const Color Color::Yellow = Color(1.0f, 1.0f, 0.0f);
const Color Color::Olive = Color(0.5f, 0.5f, 0.0f);
const Color Color::Lime = Color(0.0f, 1.0f, 0.0f);
const Color Color::Green = Color(0.0f, 0.5f, 0.0f);
const Color Color::Aqua = Color(0.0f, 1.0f, 1.0f);
const Color Color::Teal = Color(0.0f, 0.5f, 0.5f);
const Color Color::Blue = Color(0.0f, 0.0f, 1.0f);
const Color Color::Navy = Color(0.0f, 0.0f, 0.5f);
const Color Color::Fuchsia = Color(1.0f, 0.0f, 1.0f);
const Color Color::Purple = Color(0.5f, 0.0f, 0.5f);