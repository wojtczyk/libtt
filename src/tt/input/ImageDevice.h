#ifndef TT_INPUT_IMAGEDEVICE_H
#define TT_INPUT_IMAGEDEVICE_H

#include <exception>
#include <stdexcept>
#include <tt/ds/Image.h>

namespace tt
{
	
namespace input
{

/**
 * @class ImageDevice ImageDevice.h tt/input/ImageDevice.h
 * @brief Abstract base class for image devices.
 * @author Martin Wojtczyk <wojtczyk@in.tum.de>
 * 
 * The ImageDevice class is an abstract class specifying the ImageDevice
 * interface.
 */
class ImageDevice
{
public:
	ImageDevice();
	virtual ~ImageDevice();

	virtual void open() = 0;
	virtual void close() = 0;
	virtual void init() = 0;
	virtual void captureStart() = 0;
	virtual void captureStop() = 0;
	virtual void captureNext() = 0;
	virtual tt::ds::Image* getImage() = 0;
	virtual const int getImageWidth() const = 0;
	virtual const int getImageHeight() const = 0;
};

} // namespace input

} // namespace tt

#endif /*TT_INPUT_IMAGEDEVICE_H*/
