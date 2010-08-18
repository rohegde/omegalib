/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "omega.h"
#include "outk/ui/AbstractButton.h"
#include "outk/ui/Label.h"

namespace outk
{
namespace ui
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Slider: public Widget
	{
	public:
		OUTK_API Slider(omega::String name);
		OUTK_API virtual ~Slider();

		omega::String getText() { return myLabel.getText(); }
		void setText(omega::String value) { myLabel.setText(value); }

		float getValue() { return myValue; }
		void setValue(float value) { myValue = value; }

		float getMaxValue() { return myMaxValue; }
		void setMaxValue(float value) { myMaxValue = value; }

		float getMinValue() { return myMinValue; }
		void setMinValue(float value) { myMinValue = value; }

		void setProgressBarMode(bool value) { myProgressBarMode = value; }
		bool getProgressBarMode() { return myProgressBarMode; }

		//! Gets the label subobject used by the slider.
		Label* getLabel() { return &myLabel; }

	protected:
			virtual void update(const omega::UpdateContext& context);
			virtual bool processInputEvent(const omega::InputEvent& evt);
			virtual void draw();
			virtual void getSliderCoords(omega::Vector2f* pos, omega::Vector2f* size);

	protected:
		Label myLabel;

		bool myProgressBarMode;
		
		bool myPressed;
		int myPressPos;

		float mySmoothValue;
		int myValue;
		int myMaxValue;
		int myMinValue;
		int mySliderSize;
	};
}; // namespace gfx
}; // namespace outk

#endif