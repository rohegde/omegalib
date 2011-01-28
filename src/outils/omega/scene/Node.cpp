/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti							febret@gmail.com
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright (c) 2010, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions 
 * and the following disclaimer in the documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------------------------------------------------
 * Original code taken from OGRE
 * Copyright (c) 2000-2009 Torus Knot Software Ltd
 *  For the latest info, see http://www.ogre3d.org/
 *********************************************************************************************************************/
#include "omega/scene/Node.h"

using namespace omega;
using namespace omega::scene;

NameGenerator Node::msNameGenerator("Unnamed_");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node::Node()
	:mParent(0),
	mNeedParentUpdate(false),
	mNeedChildUpdate(false),
	mParentNotified(false),
    mQueuedForUpdate(false),
	mOrientation(Quaternion::IDENTITY),
	mPosition(Vector3f::ZERO),
	mScale(Vector3f::ONE),
    mInheritOrientation(true),
	mInheritScale(true),
	mDerivedOrientation(Quaternion::IDENTITY),
	mDerivedPosition(Vector3f::ZERO),
	mDerivedScale(Vector3f::ONE),
	mCachedTransformOutOfDate(true)
{
    // Generate a name
    mName = msNameGenerator.generate();

    needUpdate();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node::Node(const String& name)
	:
	mParent(0),
	mNeedParentUpdate(false),
	mNeedChildUpdate(false),
	mParentNotified(false),
    mQueuedForUpdate(false),
	mName(name),
	mOrientation(Quaternion::IDENTITY),
	mPosition(Vector3f::ZERO),
	mScale(Vector3f::ONE),
    mInheritOrientation(true),
	mInheritScale(true),
	mDerivedOrientation(Quaternion::IDENTITY),
	mDerivedPosition(Vector3f::ZERO),
	mDerivedScale(Vector3f::ONE),
	mCachedTransformOutOfDate(true)

{

    needUpdate();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node::~Node()
{
	removeAllChildren();
	if(mParent)
		mParent->removeChild(this);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node* Node::getParent(void) const
{
    return mParent;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setParent(Node* parent)
{
	bool different = (parent != mParent);

    mParent = parent;
    // Request update from parent
	mParentNotified = false ;
    needUpdate();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Matrix4f& Node::getFullTransform(void) const
{
    if (mCachedTransformOutOfDate)
    {
        // Use derived values
        mCachedTransform.makeTransform(
            getDerivedPosition(),
            getDerivedScale(),
            getDerivedOrientation());
        mCachedTransformOutOfDate = false;
    }
    return mCachedTransform;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::_update(bool updateChildren, bool parentHasChanged)
{
	// always clear information about parent notification
	mParentNotified = false ;

    // Short circuit the off case
    if (!updateChildren && !mNeedParentUpdate && !mNeedChildUpdate && !parentHasChanged )
    {
        return;
    }


    // See if we should process everyone
    if (mNeedParentUpdate || parentHasChanged)
    {
        // Update transforms from parent
        updateFromParent();
	}

	if (mNeedChildUpdate || parentHasChanged)
	{

        ChildNodeMap::iterator it, itend;
		itend = mChildren.end();
        for (it = mChildren.begin(); it != itend; ++it)
        {
            Node* child = it->second;
            child->_update(true, true);
        }
        mChildrenToUpdate.clear();
    }
    else
    {
        // Just update selected children

        ChildUpdateSet::iterator it, itend;
		itend = mChildrenToUpdate.end();
        for(it = mChildrenToUpdate.begin(); it != itend; ++it)
        {
            Node* child = *it;
            child->_update(true, false);
        }

        mChildrenToUpdate.clear();
    }

    mNeedChildUpdate = false;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::updateFromParent(void) const
{
    if (mParent)
    {
        // Update orientation
        const Quaternion& parentOrientation = mParent->getDerivedOrientation();
        if (mInheritOrientation)
        {
            // Combine orientation with that of parent
            mDerivedOrientation = parentOrientation * mOrientation;
        }
		else
        {
            // No inheritence
            mDerivedOrientation = mOrientation;
        }

        // Update scale
        const Vector3f& parentScale = mParent->getDerivedScale();
        if (mInheritScale)
        {
            // Scale own position by parent scale, NB just combine
            // as equivalent axes, no shearing
            mDerivedScale = parentScale * mScale;
        }
        else
        {
            // No inheritence
            mDerivedScale = mScale;
        }

        // Change position vector based on parent's orientation & scale
        mDerivedPosition = parentOrientation * (parentScale * mPosition);

        // Add altered position vector to parents
        mDerivedPosition += mParent->getDerivedPosition();
    }
    else
    {
        // Root node, no parent
        mDerivedOrientation = mOrientation;
        mDerivedPosition = mPosition;
        mDerivedScale = mScale;
    }

	mCachedTransformOutOfDate = true;
	mNeedParentUpdate = false;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::addChild(Node* child)
{
    if (child->mParent)
    {
		owarn(String(child->getName() + "' already was a child of '" + child->mParent->getName() + "'.").c_str());
    }

    mChildren.insert(ChildNodeMap::value_type(child->getName(), child));
    child->setParent(this);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short Node::numChildren(void) const
{
    return static_cast< unsigned short >( mChildren.size() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node* Node::getChild(unsigned short index) const
{
    if( index < mChildren.size() )
    {
        ChildNodeMap::const_iterator i = mChildren.begin();
        while (index--) ++i;
        return i->second;
    }
    else
        return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node* Node::removeChild(unsigned short index)
{
    Node* ret;
    if (index < mChildren.size())
    {
        ChildNodeMap::iterator i = mChildren.begin();
        while (index--) ++i;
        ret = i->second;
        // cancel any pending update
        cancelUpdate(ret);

        mChildren.erase(i);
        ret->setParent(NULL);
        return ret;
    }
    else
    {
        owarn("Child index out of bounds.");
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node* Node::removeChild(Node* child)
{
    if (child)
    {
        ChildNodeMap::iterator i = mChildren.find(child->getName());
        // ensure it's our child
        if (i != mChildren.end() && i->second == child)
        {
            // cancel any pending update
            cancelUpdate(child);

            mChildren.erase(i);
            child->setParent(NULL);
        }
    }
    return child;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Quaternion& Node::getOrientation() const
{
    return mOrientation;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setOrientation( const Quaternion & q )
{
	//oassert(!q.isNaN() && "Invalid orientation supplied as parameter");
	mOrientation = q;
	mOrientation.normalize();
    needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setOrientation( float w, float x, float y, float z)
{
	setOrientation(Quaternion(w, x, y, z));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::resetOrientation(void)
{
    mOrientation = Quaternion::IDENTITY;
    needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setPosition(const Vector3f& pos)
{
	//oassert(!pos.isNaN() && "Invalid vector supplied as parameter");
    mPosition = pos;
    needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setPosition(float x, float y, float z)
{
    Vector3f v(x,y,z);
    setPosition(v);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Vector3f & Node::getPosition(void) const
{
    return mPosition;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Matrix3f Node::getLocalAxes(void) const
{
    Vector3f axisX = Vector3f::UNIT_X;
    Vector3f axisY = Vector3f::UNIT_Y;
    Vector3f axisZ = Vector3f::UNIT_Z;

    axisX = mOrientation * axisX;
    axisY = mOrientation * axisY;
    axisZ = mOrientation * axisZ;

	Matrix3f mt;
	mt.set_row(0, axisX);
	mt.set_row(1, axisY);
	mt.set_row(2, axisZ);
	return mt;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::translate(const Vector3f& d, TransformSpace relativeTo)
{
    switch(relativeTo)
    {
    case TS_LOCAL:
        // position is relative to parent so transform downwards
        mPosition += mOrientation * d;
        break;
    case TS_WORLD:
        // position is relative to parent so transform upwards
        if (mParent)
        {
			mPosition += (mParent->getDerivedOrientation().inverse() * d) / mParent->getDerivedScale();
        }
        else
        {
            mPosition += d;
        }
        break;
    case TS_PARENT:
        mPosition += d;
        break;
    }
    needUpdate();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::translate(float x, float y, float z, TransformSpace relativeTo)
{
    Vector3f v(x,y,z);
    translate(v, relativeTo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::translate(const Matrix3f& axes, const Vector3f& move, TransformSpace relativeTo)
{
    Vector3f derived = axes * move;
    translate(derived, relativeTo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::translate(const Matrix3f& axes, float x, float y, float z, TransformSpace relativeTo)
{
    Vector3f d(x,y,z);
    translate(axes,d,relativeTo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::roll(const float& angle, TransformSpace relativeTo)
{
    rotate(Vector3f::UNIT_Z, angle, relativeTo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::pitch(const float& angle, TransformSpace relativeTo)
{
    rotate(Vector3f::UNIT_X, angle, relativeTo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::yaw(const float& angle, TransformSpace relativeTo)
{
    rotate(Vector3f::UNIT_Y, angle, relativeTo);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::rotate(const Vector3f& axis, const float& angle, TransformSpace relativeTo)
{
    Quaternion q;
    q.fromAngleAxis(angle,axis);
    rotate(q, relativeTo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::rotate(const Quaternion& q, TransformSpace relativeTo)
{
	// Normalise quaternion to avoid drift
	Quaternion qnorm = q;
	qnorm.normalize();

    switch(relativeTo)
    {
    case TS_PARENT:
        // Rotations are normally relative to local axes, transform up
        mOrientation = qnorm * mOrientation;
        break;
    case TS_WORLD:
        // Rotations are normally relative to local axes, transform up
        mOrientation = mOrientation * getDerivedOrientation().inverse()
            * qnorm * getDerivedOrientation();
        break;
    case TS_LOCAL:
        // Note the order of the mult, i.e. q comes after
        mOrientation = mOrientation * qnorm;
        break;
    }
    needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::_setDerivedPosition( const Vector3f& pos )
{
	//find where the node would end up in parent's local space
	setPosition( mParent->convertWorldToLocalPosition( pos ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::_setDerivedOrientation( const Quaternion& q )
{
	//find where the node would end up in parent's local space
	setOrientation( mParent->convertWorldToLocalOrientation( q ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Quaternion & Node::getDerivedOrientation(void) const
{
	if (mNeedParentUpdate)
	{
        updateFromParent();
	}
    return mDerivedOrientation;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Vector3f & Node::getDerivedPosition(void) const
{
	if (mNeedParentUpdate)
	{
        updateFromParent();
	}
    return mDerivedPosition;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Vector3f & Node::getDerivedScale(void) const
{
    if (mNeedParentUpdate)
    {
        updateFromParent();
    }
    return mDerivedScale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vector3f Node::convertWorldToLocalPosition( const Vector3f &worldPos )
{
	if (mNeedParentUpdate)
    {
        updateFromParent();
    }
	return mDerivedOrientation.inverse() * (worldPos - mDerivedPosition) / mDerivedScale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vector3f Node::convertLocalToWorldPosition( const Vector3f &localPos )
{
	if (mNeedParentUpdate)
    {
        updateFromParent();
    }
	return (mDerivedOrientation * localPos * mDerivedScale) + mDerivedPosition;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Quaternion Node::convertWorldToLocalOrientation( const Quaternion &worldOrientation )
{
	if (mNeedParentUpdate)
	{
		updateFromParent();
	}
	return mDerivedOrientation.inverse() * worldOrientation;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Quaternion Node::convertLocalToWorldOrientation( const Quaternion &localOrientation )
{
	if (mNeedParentUpdate)
	{
		updateFromParent();
	}
	return mDerivedOrientation * localOrientation;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::removeAllChildren(void)
{
	ChildNodeMap::iterator i, iend;
	iend = mChildren.end();
	for (i = mChildren.begin(); i != iend; ++i)
	{
		i->second->setParent(0);
	}
    mChildren.clear();
	mChildrenToUpdate.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setScale(const Vector3f& inScale)
{
	//assert(!inScale.isNaN() && "Invalid vector supplied as parameter");
    mScale = inScale;
    needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setScale(float x, float y, float z)
{
	setScale(Vector3f(x, y, z));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Vector3f & Node::getScale(void) const
{
    return mScale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setInheritOrientation(bool inherit)
{
    mInheritOrientation = inherit;
    needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Node::getInheritOrientation(void) const
{
    return mInheritOrientation;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::setInheritScale(bool inherit)
{
    mInheritScale = inherit;
    needUpdate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Node::getInheritScale(void) const
{
    return mInheritScale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::scale(const Vector3f& inScale)
{
    mScale = mScale * inScale;
    needUpdate();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::scale(float x, float y, float z)
{
    mScale.x() *= x;
    mScale.y() *= y;
    mScale.z() *= z;
    needUpdate();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const String& Node::getName(void) const
{
    return mName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node* Node::getChild(const String& name) const
{
    ChildNodeMap::const_iterator i = mChildren.find(name);

    if (i == mChildren.end())
    {
        owarn(String("Child node named " + name + " does not exist.").c_str());
    }
    return i->second;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node* Node::removeChild(const String& name)
{
    ChildNodeMap::iterator i = mChildren.find(name);

    if (i == mChildren.end())
    {
		owarn(String("Child node named " + name + " does not exist.").c_str());
    }

    Node* ret = i->second;
    // Cancel any pending update
    cancelUpdate(ret);

    mChildren.erase(i);
    ret->setParent(NULL);

    return ret;


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node::ChildNodeIterator Node::getChildIterator(void)
{
    return ChildNodeIterator(mChildren.begin(), mChildren.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Node::ConstChildNodeIterator Node::getChildIterator(void) const
{
	return ConstChildNodeIterator(mChildren.begin(), mChildren.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::needUpdate(bool forceParentUpdate)
{

    mNeedParentUpdate = true;
	mNeedChildUpdate = true;
    mCachedTransformOutOfDate = true;

    // Make sure we're not root and parent hasn't been notified before
    if (mParent && (!mParentNotified || forceParentUpdate))
    {
        mParent->requestUpdate(this, forceParentUpdate);
		mParentNotified = true ;
    }

    // all children will be updated
    mChildrenToUpdate.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::requestUpdate(Node* child, bool forceParentUpdate)
{
    // If we're already going to update everything this doesn't matter
    if (mNeedChildUpdate)
    {
        return;
    }

    mChildrenToUpdate.insert(child);
    // Request selective update of me, if we didn't do it before
    if (mParent && (!mParentNotified || forceParentUpdate))
	{
        mParent->requestUpdate(this, forceParentUpdate);
		mParentNotified = true ;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Node::cancelUpdate(Node* child)
{
    mChildrenToUpdate.erase(child);

    // Propogate this up if we're done
    if (mChildrenToUpdate.empty() && mParent && !mNeedChildUpdate)
    {
        mParent->cancelUpdate(this);
		mParentNotified = false ;
    }
}
