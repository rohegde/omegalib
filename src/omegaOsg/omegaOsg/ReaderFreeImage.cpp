#include <osg/Image>
#include <osg/Notify>

#include <omega.h>

//#include <osgDB/Registry>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
//#include <osgDB/fstream>

#include "omegaOsg/ReaderFreeImage.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderFreeImage::ReaderFreeImage()
{
    supportsExtension("tga","Tga Image format");
    supportsExtension("png","Png Image format");
    supportsExtension("jpg","Jpg Image format");
}
        
///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::ReadResult ReaderFreeImage::readObject(std::istream& fin,const osgDB::ReaderWriter::Options* options) const
{
	return ReadResult::NOT_IMPLEMENTED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::ReadResult ReaderFreeImage::readObject(const std::string& file, const osgDB::ReaderWriter::Options* options) const
{
    return readImage(file, options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::ReadResult ReaderFreeImage::readImage(std::istream& fin,const Options* ) const
{
	return ReadResult::NOT_IMPLEMENTED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
ReaderWriter::ReadResult ReaderFreeImage::readImage(const std::string& file, const osgDB::ReaderWriter::Options* options) const
{
    std::string ext = osgDB::getLowerCaseFileExtension(file);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    std::string fileName = osgDB::findDataFile( file, options );
    if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

	omega::ImageData* img = omega::ImageUtils::loadImage(file, true);
	if(img == NULL) return ReadResult::FILE_NOT_FOUND;

    //unsigned char *imageData = NULL;
    //int width_ret;
    //int height_ret;
    //int numComponents_ret;

    //imageData = simage_tga_load(fin,&width_ret,&height_ret,&numComponents_ret);

    //if (imageData==NULL) return ReadResult::FILE_NOT_HANDLED;

	int s = img->getWidth();
    int t = img->getHeight();
    int r = 1;

    int internalFormat = img->getPixels()->getBpp() / 8;

    unsigned int pixelFormat =
        internalFormat == 1 ? GL_LUMINANCE :
    internalFormat == 2 ? GL_LUMINANCE_ALPHA :
    internalFormat == 3 ? GL_RGB :
    internalFormat == 4 ? GL_RGBA : (GLenum)-1;

    unsigned int dataType = GL_UNSIGNED_BYTE;

    osg::Image* pOsgImage = new osg::Image;
    pOsgImage->setImage(s,t,r,
        internalFormat,
        pixelFormat,
        dataType,
		img->getPixels()->lockData(),
        osg::Image::NO_DELETE);
	img->getPixels()->unlockData();

    ReadResult rr(pOsgImage);
    if(rr.validImage()) rr.getImage()->setFileName(file);
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
