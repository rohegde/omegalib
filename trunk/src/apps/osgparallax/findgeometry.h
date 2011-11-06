/*******************************************************************************/
/*
 * Author: Ing. Jose Larios Delgado.
 * Project:  glsl shaders implementation with OpenSceneGraph.
 * Institutions: DGSCA, UNAM, Virtual Reality Department.
 * Description: Find Node Visitor.   
 */
/*******************************************************************************/
#include <osg/NodeVisitor>
#include <iostream>

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;

//!Class that finds the geometry nodes.
/*!
   Traverses a scenegraph storing all the geometry nodes pointers.
*/
class findGeoVisitor : public osg::NodeVisitor
{
public:
	//!Constructor
	/*!
		Defines the traversal mode.
	*/
	findGeoVisitor()
	{
		setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
	}

	//!Overloaded apply method.
	/*!
		Gets the class type of the traversing node, and stores all the geometry nodes pointers.

		\param[in] searchNode      Traversing node.
		
		\return Nothing.

	*/
	virtual void apply(osg::Node &searchNode)
	{
		if(!strcmp(searchNode.className(), "Geode"))
		{
		
			osg::Geode *tmpGeode = dynamic_cast<osg::Geode*>(&searchNode);
			tmpGeode->setNodeMask(ReceivesShadowTraversalMask | CastsShadowTraversalMask);
			
			osg::Geometry *tmpGeo = dynamic_cast<osg::Geometry*>(tmpGeode->getDrawable(0));
			geolist.push_back(tmpGeo);
	
		}
		//std::cout<<searchNode.getName()<<std::endl;
		traverse(searchNode);
	}

	std::vector<osg::Geometry*> geolist;/*!< List of geometry pointers.*/
};