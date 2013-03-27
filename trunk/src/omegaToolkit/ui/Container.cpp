/**************************************************************************************************
 * THE OMEGA LIB PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2013		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2013, Electronic Visualization Laboratory, University of Illinois at Chicago
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
#include "omegaToolkit/ui/Container.h"
#include "omegaToolkit/UiModule.h"
#include "omega/DisplaySystem.h"

#include "omega/DisplaySystem.h"

#include "omegaGl.h"

using namespace omega;
using namespace omegaToolkit;
using namespace omegaToolkit::ui;

///////////////////////////////////////////////////////////////////////////////////////////////////
Container::Container(Engine* server):
		Widget(server),
		myPadding(5),
		myMargin(5),
		myHorizontalAlign(AlignCenter),
		myVerticalAlign(AlignMiddle),
		myGridRows(1),
		myGridColumns(1),
		myPixelOutputEnabled(false)
{
	// Containers have autosize enabled by default.
	setAutosize(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Container::~Container()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::load(Setting& setting)
{
	if(setting.exists("layout"))
	{
		String layout = setting["layout"];
		if(layout == "LayoutFree") myLayout = LayoutFree;
		if(layout == "LayoutVertical") myLayout = LayoutVertical;
		if(layout == "LayoutHorizontal") myLayout = LayoutHorizontal;
		if(layout == "LayoutGridHorizontal") myLayout = LayoutGridHorizontal;
		if(layout == "LayoutGridVertical") myLayout = LayoutGridVertical;
	}
	if(setting.exists("position"))
	{
		String position = setting["position"];
		setPosition(Vector2f(position[0], position[1]));
	}
	if(setting.exists("horizontalAlign"))
	{
		String align = setting["horizontalAlign"];
		if(align == "AlignLeft") myHorizontalAlign = AlignLeft;
		if(align == "AlignCenter") myHorizontalAlign = AlignCenter;
		if(align == "AlignRight") myHorizontalAlign = AlignRight;
	}
	if(setting.exists("verticalAlign"))
	{
		String align = setting["verticalAlign"];
		if(align == "AlignTop") myVerticalAlign = AlignTop;
		if(align == "AlignBottom") myVerticalAlign = AlignBottom;
		if(align == "AlignMiddle") myVerticalAlign = AlignMiddle;
	}
	if(setting.exists("size"))
	{
		Setting& size = setting["size"];
		setSize(Vector2f(size[0], size[1]));
	}
	if(setting.exists("width"))
	{
		setWidth((int)setting["width"]);
	}
	if(setting.exists("height"))
	{
		setHeight((int)setting["height"]);
	}
	if(setting.exists("padding"))
	{
		myPadding = setting["padding"];
	}
	if(setting.exists("margin"))
	{
		myMargin = setting["margin"];
	}
	if(setting.exists("gridRows"))
	{
		myGridRows = setting["gridRows"];
	}
	if(setting.exists("gridColumns"))
	{
		myGridRows = setting["gridColumns"];
	}
	if(setting.exists("debugMode"))
	{
		myDebugModeEnabled = setting["debugMode"];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::addChild(Widget* child)
{
	requestLayoutRefresh();
	myChildren.push_back(child);
	child->setContainer(this);
	if(child->isNavigationEnabled()) updateChildrenNavigation();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::removeChild(Widget* child)
{
	requestLayoutRefresh();
	myChildren.remove(child);
	child->setContainer(NULL);
	if(child->isNavigationEnabled())  updateChildrenNavigation();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
Widget* Container::getChildByName(const String& name)
{
	foreach(Widget* w, myChildren)
	{
		if(w->getName() == name) return w;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Widget* Container::getChildBefore(const Widget* child)
{
	Widget* prev = NULL;
	foreach(Widget* w, myChildren)
	{
		if(w->isNavigationEnabled())
		{
			if(child == w) return prev;
			prev = w;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Widget* Container::getChildAfter(const Widget* child)
{
	bool found = false;
	foreach(Widget* w, myChildren)
	{
		if(w->isNavigationEnabled())
		{
			if(found) return w;
			if(child == w) found = true;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Widget* Container::getChildByIndex(int index)
{
	if(getNumChildren() > index)
	{
		int i = 0;
		foreach(Widget* w, myChildren)
		{
			if(i == index) return w;
			i++;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::updateSize(Renderer* r)
{
	if(needLayoutRefresh())
	{
		foreach(Widget* w, myChildren)
		{
			w->updateSize(r);
		}
		Widget::updateSize(r);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::autosize(Renderer* r)
{
	int width = 0;
	int height = 0;
	int maxwidth = 0;
	int maxheight = 0;
	foreach(Widget* w, myChildren)
	{
		if(w->getWidth() > maxwidth) maxwidth = w->getWidth();
		if(w->getHeight() > maxheight) maxheight = w->getHeight();
		width += w->getWidth();
		height += w->getHeight();
	}
	if(myLayout == LayoutHorizontal)
	{
		width += myPadding * getNumChildren();
		height = maxheight;
		foreach(Widget* w, myChildren)
		{
			//w->setActualSize(maxheight, Vertical);
		}
	}
	else if(myLayout == LayoutVertical)
	{
		height += myPadding * getNumChildren();
		width = maxwidth;
		foreach(Widget* w, myChildren)
		{
			w->setActualSize(maxwidth, Horizontal);
		}
	}

	width += myMargin * 2;
	height += myMargin * 2;

	mySize = Vector2f(width, height);
	myMinimumSize = mySize;
	myMaximumSize = mySize;
	//setSize(Vector2f(width, height));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::updateChildrenNavigation()
{
	foreach(Widget* w, myChildren)
	{
		if(myLayout == LayoutHorizontal)
		{
			w->setHorizontalNextWidget(getChildAfter(w));
			w->setHorizontalPrevWidget(getChildBefore(w));
			//Container* parent = getContainer();
			//if(parent != NULL)
			//{
			//	w->setVerticalNextWidget(parent->getChildAfter(this));
			//	w->setVerticalPrevWidget(parent->getChildBefore(this));
			//}
		}
		else
		{
			w->setVerticalNextWidget(getChildAfter(w));
			w->setVerticalPrevWidget(getChildBefore(w));
			Container* parent = getContainer();
			//if(parent != NULL)
			//{
			//	w->setHorizontalNextWidget(parent->getChildAfter(this));
			//	w->setHorizontalPrevWidget(parent->getChildBefore(this));
			//}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int Container::expandStep(int availableSpace, Orientation orientation)
{
	// Check space constraints for each child
	int childSpace = availableSpace / getNumChildren();
	int spaceLeft = availableSpace;

	foreach(Widget* w, myChildren)
	{
		int size = w->getSize()[orientation] + childSpace;
		w->setActualSize(size, orientation);
		spaceLeft -= (orientation == Horizontal ? w->getWidth(): w->getHeight());
	}
	return spaceLeft;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::updateChildrenLayoutPosition(Orientation orientation)
{
	int p = 0;

	if((orientation == Horizontal && myHorizontalAlign == AlignLeft) ||
		(orientation == Vertical && myVerticalAlign == AlignTop))
	{
		p = myMargin;
	}
	else if((orientation == Horizontal && myHorizontalAlign == AlignRight) ||
		(orientation == Vertical && myVerticalAlign == AlignBottom))
	{
		float size = (orientation == Horizontal ? getWidth(): getHeight());
		p = size - myMargin;
		foreach(Widget* w, myChildren)
		{
			float csize = (orientation == Horizontal ? w->getWidth(): w->getHeight());
			p -= (csize + myMargin);
		}
	}
	else
	{
		// TODO: Compute center align start position
		p = myMargin;
	}
	foreach(Widget* w, myChildren)
	{
		w->setPosition(p, orientation);
		p += w->getSize()[orientation] + myPadding;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::updateChildrenFreeBounds(Orientation orientation)
{
	// Compute the maximum available size
	//int available = getSize()[orientation] - myPadding * 2;
	int available = getSize()[orientation]  - myMargin * 2;

	foreach(Widget* w, myChildren)
	{
		// Set child size.
		w->setActualSize(available, orientation);

		// Set child position
		int pos = 0;
		if((orientation == Horizontal && myHorizontalAlign == AlignLeft) ||
			(orientation == Vertical && myVerticalAlign == AlignTop))
		{
			pos = myMargin;
		}
		else if((orientation == Horizontal && myHorizontalAlign == AlignRight) ||
			(orientation == Vertical && myVerticalAlign == AlignBottom))
		{
			float size = (orientation == Horizontal ? getWidth(): getHeight());
			float csize = (orientation == Horizontal ? w->getWidth(): w->getHeight());

			pos = size - csize - myMargin;
		}
		else
		{
			float csize = (orientation == Horizontal ? w->getWidth(): w->getHeight());
			pos = (available - csize) / 2 + myMargin;
		}

		w->setPosition(pos, orientation);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::resetChildrenSize(Orientation orientation)
{
	// Initialize widget width to 0
	foreach(Widget* w, myChildren)
	{ 
		w->mySize[orientation] = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::computeLinearLayout(Orientation orientation)
{
	int nc = getNumChildren();
	Orientation oppositeOrientation = (orientation == Vertical) ? Horizontal : Vertical;
	// TODO: Check satisfiability of conditions

	// Compute how much total space is available.
	int availableSpace = getSize()[orientation] - myPadding * 2 - (nc - 1) * myMargin;

	resetChildrenSize(orientation);
	while(availableSpace > 0)
	{
		availableSpace = expandStep(availableSpace, orientation) - 1;
	}
	updateChildrenLayoutPosition(orientation);
	updateChildrenFreeBounds(oppositeOrientation);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::computeGridLayout(Orientation orientation)
{
	int nc = getNumChildren();
	Orientation oppositeOrientation = (orientation == Vertical) ? Horizontal : Vertical;
	// TODO: Check satisfiability of conditions

	// Compute how much total space is available.
	int availableWidth = getSize()[Horizontal] - myPadding * 2 - (myGridColumns - 1) * myMargin;
	int availableHeight = getSize()[Vertical] - myPadding * 2 - (myGridRows - 1) * myMargin;

	int cellWidth = availableWidth / myGridColumns;
	int cellHeight = availableHeight / myGridRows;

	int cellX = myPadding;
	int cellY = myPadding;

	foreach(Widget* w, myChildren)
	{
		w->setActualSize(cellWidth, Horizontal);
		w->setActualSize(cellHeight, Vertical);

		w->setPosition(Vector2f(cellX, cellY));

		// Compute next cell position.
		if(orientation == Horizontal)
		{
			cellX += cellWidth + myMargin;
			if(cellX > availableWidth)
			{
				cellX = myPadding;
				cellY += cellHeight + myMargin;
			}
		}
		else
		{
			cellY += cellHeight + myMargin;
			if(cellY > availableHeight)
			{
				cellY = myPadding;
				cellX += cellWidth + myMargin;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::layout()
{
	if(getNumChildren() != 0)
	{
		if(needLayoutRefresh())
		{
			if(myLayout == LayoutHorizontal)
			{
				computeLinearLayout(Horizontal);
			}
			else if(myLayout == LayoutVertical)
			{
				computeLinearLayout(Vertical);
			}
			else if(myLayout == LayoutGridHorizontal)
			{
				computeGridLayout(Horizontal);
			}
			if(myLayout == LayoutGridVertical)
			{
				computeGridLayout(Vertical);
			}

			// Layout children.
			foreach(Widget* w, myChildren)
			{
				w->layout();
			}

			Widget::layout();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::update(const omega::UpdateContext& context)
{
	Widget::update(context);
	foreach(Widget* w, myChildren)
	{
		w->update(context);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::rayToPointerEvent(const Event& inEvt, Event& outEvt)
{
	// Shortcut: if this container is not in 3D mode, just return the original event.
	if(!my3dSettings.enable3d)
	{
		if(isEventInside(inEvt))
		{
			outEvt.copyFrom(inEvt);
			return true;
		}
		return false;
	}

	Ray r;
	if(!SystemManager::instance()->getDisplaySystem()->getViewRayFromEvent(inEvt, r))
	{
		// we could not generate a ray from the input event, return.
		return false;
	}

	// Intersect the pointer ray with the container 3d plane.
	Vector3f pos = my3dSettings.position;
	Vector3f normal = my3dSettings.normal;

	// If the container is attached to a node, convert the container position and normal to world
	// space before computing the plane intersection.
	if(my3dSettings.node != NULL)
	{
		const AffineTransform3& xform = my3dSettings.node->getFullTransform();
		pos = my3dSettings.node->convertLocalToWorldPosition(pos);
		normal = my3dSettings.node->getDerivedOrientation() * normal;
	}

	if(my3dSettings.center)
	{
		float width = getWidth() * my3dSettings.scale;
		float height = getHeight() * my3dSettings.scale;
		pos -= Vector3f(width / 2, height / 2, 0);
	}

	Plane plane(normal, pos);
	std::pair<bool, float> result = Math::intersects(r, plane);
	if(result.first)
	{
		// An intersection exists: find the point.
		Vector3f intersection = r.getPoint(result.second);

		// Turn the intersection point from world coordinates to pixel, ui coordinates.
		intersection = pos - intersection;
		
		Vector3f widthVector = -my3dSettings.up.cross(my3dSettings.normal);
		Vector3f heightVector = -my3dSettings.up;

		widthVector.normalize();
		heightVector.normalize();

		float x = intersection.dot(widthVector);
		float y = intersection.dot(heightVector);

		Vector3f pointerPosition(x / my3dSettings.scale + myPosition[0], getHeight() - (y / my3dSettings.scale) + myPosition[1], 0);
		outEvt.reset(inEvt.getType(), Service::Pointer);
		outEvt.setPosition(pointerPosition);
		outEvt.setFlags(inEvt.getFlags());

		if(isDebugModeEnabled())
		{
			ofmsg("intersection: %1%    ui pos: %2%", %intersection %pointerPosition);
		}

		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::isEventInside(const Event& evt)
{
	// Intersection with 3D containers
	if(my3dSettings.enable3d)
	{
		// We will just discard the converted event.
		Event newEvt;
		return rayToPointerEvent(evt, newEvt);
	}
	
	// Intersection with 2D containers
	if(evt.getServiceType() == Event::ServiceTypePointer)
	{
		Vector2f pos2d = Vector2f(evt.getPosition().x(), evt.getPosition().y());
		return hitTest(pos2d);
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::handleEvent(const Event& evt)
{
	// Only handle events if the container is visible.
	if(isVisible() && isEnabled())
	{
		// Container is displayed in 3d mode. Convert pointer rays and wand rays into 
		// standard pointer events.
		if(my3dSettings.enable3d && isPointerInteractionEnabled())
		{
			Event newEvt;
			if(rayToPointerEvent(evt, newEvt))
			{
				foreach(Widget* w, myChildren)
				{
					w->handleEvent(newEvt);
				}
			}
		}
		else
		{
			if(isPointerInteractionEnabled())
			{
				// For pointer interaction, just dispatch the event to all children
				foreach(Widget* w, myChildren)
				{
					w->handleEvent(evt);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::activate()
{
	if(isEnabled())
	{
		// Activate is rerouted by default to first child. If this container has no children,
		// mark no widget as active.
		Widget* child = getChildByIndex(0);
		if(child != NULL) UiModule::instance()->activateWidget(child);
		else UiModule::instance()->activateWidget(NULL);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Container::isPixelOutputEnabled() 
{ 
	return myPixelOutputEnabled; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Container::setPixelOutputEnabled(bool value)
{
	myPixelOutputEnabled = value;
	if(myPixelOutputEnabled && myPixels == NULL)
	{
		myPixels = new PixelData(PixelData::FormatRgba, 100, 100);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
PixelData* Container::getPixels()
{
	return myPixels;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Renderable* Container::createRenderable()
{
	return new ContainerRenderable(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ContainerRenderable::drawChildren(const DrawContext& context, bool containerOnly)
{
	// draw children.
	for(int layer = Widget::Back; layer < Widget::NumLayers; layer++)
	{
		foreach(Widget* w, myOwner->myChildren)
		{
			if(w->getLayer() == layer)
			{
				Renderable* childRenderable;
				if(containerOnly) childRenderable = dynamic_cast<ContainerRenderable*>(w->getRenderable(getClient())); 
				else childRenderable = w->getRenderable(getClient()); 
				if(childRenderable != NULL) 
				{
					childRenderable->draw(context);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ContainerRenderable::draw3d(const DrawContext& context)
{
	if(myTexture != NULL)
	{
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		myTexture->bind(GpuManager::TextureUnit0);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Container3dSettings& c3ds = myOwner->get3dSettings();
		float width = myOwner->getWidth() * c3ds.scale;
		float height = myOwner->getHeight() * c3ds.scale;

		glColor4ub(255, 255, 255, (GLubyte)(c3ds.alpha * 255));

		glPushMatrix();

		if(c3ds.node != NULL)
		{
			getRenderer()->pushTransform(c3ds.node->getFullTransform());
		}

		if(!c3ds.center)
		{
			glTranslatef(c3ds.position[0], c3ds.position[1], c3ds.position[2]);
		}
		else
		{
			glTranslatef(c3ds.position[0] - width / 2, c3ds.position[1] - height / 2, c3ds.position[2]);
		}

		// TODO: redo this using DrawInterface.
		Vector3f downLeft = c3ds.up;
		Vector3f topRight = downLeft.cross(c3ds.normal);
		downLeft *= height;
		topRight *= width;

		Vector3f downRight = topRight + downLeft;

		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 1);
		glVertex3f(0, 0, 0);

		glTexCoord2f(1, 1);
		glVertex3f(topRight.x(), topRight.y(), topRight.z());

		glTexCoord2f(0, 0);
		glVertex3f(downLeft.x(), downLeft.y(), downLeft.z());

		glTexCoord2f(1, 0);
		glVertex3f(downRight.x(), downRight.y(), downRight.z());

		glEnd();

		if(c3ds.node != NULL)
		{
			getRenderer()->popTransform();
		}
		glPopMatrix();

		myTexture->unbind();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ContainerRenderable::beginDraw(const DrawContext& context)
{
	if(myOwner->get3dSettings().enable3d || myOwner->isPixelOutputEnabled())
	{
		if(myOwner->get3dSettings().enable3d)
		{
			if(myRenderTarget == NULL || 
				myTexture->getWidth() != myOwner->getWidth() ||
				myTexture->getHeight() != myOwner->getHeight())
			{
 				myTexture = new Texture(context.gpuContext);
				myTexture->initialize(myOwner->getWidth(), myOwner->getHeight());
				myRenderTarget = new RenderTarget(context.gpuContext, RenderTarget::RenderToTexture);
				myRenderTarget->setTextureTarget(myTexture);
			}
		}
		else if(myOwner->isPixelOutputEnabled())
		{
			PixelData* pixels = myOwner->getPixels();
			if(myRenderTarget == NULL || 
				pixels->getWidth() != myOwner->getWidth() ||
				pixels->getHeight() != myOwner->getHeight())
			{
				// Resize the pixel buffer.
				pixels->resize(myOwner->getWidth(), myOwner->getHeight());
				myRenderTarget = new RenderTarget(context.gpuContext, RenderTarget::RenderOffscreen);
				myRenderTarget->setReadbackTarget(pixels);
			}
			pixels->setDirty(true);
		}

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, myOwner->getWidth(), myOwner->getHeight());
				
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		// Kindof hackish... when saving data to a pixel buffer we have to render inverted y (glReadPixels reands from the bottom line up)
		if(myOwner->isPixelOutputEnabled()) glOrtho(0, myOwner->getWidth(), myOwner->getHeight(), 0, 0, 1);
		else glOrtho(0, myOwner->getWidth(), 0, myOwner->getHeight(), 0, 1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		//glScalef(0.05f, 0.05f, 1);
		//glTranslatef(0, -SystemManager::instance()->getDisplaySystem()->getCanvasSize().y(), 0);

		myRenderTarget->bind();

		pushDrawAttributes();

		if(myOwner->isPixelOutputEnabled()) 
		{
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
	else
	{
		preDraw();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ContainerRenderable::endDraw(const DrawContext& context)
{
	if(myOwner->get3dSettings().enable3d || myOwner->isPixelOutputEnabled())
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glPopAttrib();

		if(myOwner->isPixelOutputEnabled()) myRenderTarget->readback();

		myRenderTarget->unbind();
		popDrawAttributes();
	}
	else
	{
		postDraw();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void ContainerRenderable::draw(const DrawContext& context)
{
	if(myOwner->isVisible())
	{
		if(context.task == DrawContext::SceneDrawTask)
		{
			if(myOwner->get3dSettings().enable3d)
			{
				draw3d(context);
			}
			else
			{
				drawChildren(context, true);
			}
		}
		else
		{
			beginDraw(context);

			// draw myself.
			if(myOwner->isStereo())
			{
				if(context.eye != DrawContext::EyeCyclop) drawContent(context);
			}
			else
			{
				if(context.eye == DrawContext::EyeCyclop) drawContent(context);
			}

			drawChildren(context, false);

			endDraw(context);
		}
	}
}


