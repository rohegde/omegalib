#ifndef _POINTS_LOADER_H_
#define _POINTS_LOADER_H_

// CVR
#include <omega.h>
#include <cyclops.h>

// OSG
#include <osg/Group>
#include <osg/Vec3>
#include <osg/Uniform>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

class PointsLoader : public cyclops::ModelLoader
{
public:
	virtual bool load(cyclops::ModelAsset* model);
	virtual bool supportsExtension(const String& ext); 

    PointsLoader();
    virtual ~PointsLoader();
    void initialize();

private:
    bool loadFile(std::string file, osg::Group * grp);
    void readXYZ(const String& filename, osg::Vec3Array* points, osg::Vec4Array* colors);
};
#endif
