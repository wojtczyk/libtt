#ifndef TRAX_INPUT_OPENCVCAMERA_H
#define TRAX_INPUT_OPENCVCAMERA_H

#include "ImageDevice.h"

namespace tt
{

namespace input
{

/**
 * @class OpenCVCamera OpenCVCamera.h trax/input/OpenCVCameraCamera.h
 * @brief Adapter class to interface the cameras supported in OpenCV.
 * @author Martin Wojtczyk <wojtczyk@in.tum.de>
 * 
 * OpenCVCamera implementes the ImageDevice interface for the cameras which
 * are supported in OpenCV.
 */
class OpenCVCamera : public tt::input::ImageDevice
{
public:
	OpenCVCamera();
	virtual ~OpenCVCamera();

	virtual void open();
	virtual void close();
	virtual void init();
	virtual void captureStart();
	virtual void captureStop();
	virtual void captureNext();
	virtual tt::ds::Image* getImage();
	virtual const int getImageWidth() const;
	virtual const int getImageHeight() const;

};

}

}

#endif /*TRAX_INPUT_OPENCVCAMERA_H*/
