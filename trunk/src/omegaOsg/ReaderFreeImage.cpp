#include <osg/Image>
#include <osg/Notify>

#include <omega.h>

//#include <osgDB/Registry>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
//#include <osgDB/fstream>

#include "omegaOsg/ReaderFreeImage.h"
#include "omegaOsg/OsgModule.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderFreeImage::ReaderFreeImage()
{
    supportsExtension("tga","Tga Image format");
    supportsExtension("png","Png Image format");
    supportsExtension("jpg","Jpg Image format");
    supportsExtension("JPG","Jpg Image format");
    supportsExtension("GIF","Jpg Image format");
    supportsExtension("PNG","Jpg Image format");
}
        
///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::ReadResult ReaderFreeImage::readObject(std::istream& fin,const osgDB::ReaderWriter::Options* options) const
{
	return ReadResult::FILE_NOT_HANDLED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::ReadResult ReaderFreeImage::readObject(const std::string& file, const osgDB::ReaderWriter::Options* options) const
{
    return readImage(file, options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::ReadResult ReaderFreeImage::readImage(std::istream& fin,const Options* ) const
{
	return ReadResult::FILE_NOT_HANDLED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::ReadResult ReaderFreeImage::readImage(const std::string& file, const osgDB::ReaderWriter::Options* options) const
{
	String filePath = file;

	std::string ext = osgDB::getLowerCaseFileExtension(filePath);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    //std::string fileName = osgDB::findDataFile( file, options );
    //if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

	bool leaveMemoryAlone = false;

	omega::Ref<omega::PixelData> img = omega::ImageUtils::loadImage(filePath, false);
	if(img == NULL) return ReadResult::FILE_NOT_FOUND;

	osg::Image* pOsgImage = omegaOsg::OsgModule::pixelDataToOsg(img);
	
    ReadResult rr(pOsgImage);
    if(rr.validImage()) rr.getImage()->setFileName(filePath);
    return rr;
}
       
///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::WriteResult ReaderFreeImage::writeImage(const osg::Image& image, std::ostream& fout, const Options*) const
{
	return WriteResult::NOT_IMPLEMENTED;
}
        
///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::WriteResult ReaderFreeImage::writeImage(const osg::Image& image, const std::string& fileName, const Options* options) const
{
	return WriteResult::NOT_IMPLEMENTED;
}
