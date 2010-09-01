#ifndef TRAX_INPUT_WINDOWSCMU1394CAMERA_H
#define TRAX_INPUT_WINDOWSCMU1394CAMERA_H

#ifdef WIN32 // Build WindowsCMU1394Camera only if compiled on a Win32 platform

#include <windows.h>
#include <1394Camera.h>
#include <tt/ds/Image.h>
#include "FirewireCamera.h"

namespace tt
{

namespace input
{

/**
 * @class WindowsCMU1394Camera WindowsCMU1394Camera.h trax/input/WindowsCMU1394Camera.h
 * @brief Concrete class implementing the Camera interface for Firewire cameras.
 * @author Martin Wojtczyk <wojtczyk@in.tum.de>
 * 
 * WindowsCMU1394Camera implements the Camera interface for Firewire cameras
 * on Windows platforms and is based on the open source CMU1394 driver.
 */
class WindowsCMU1394Camera : public tt::input::FirewireCamera
{
private:
	/** @brief Indicates, if we are in capture state or not. */
	bool capturing;
	/** @brief Index of the camera */
	int cameraIndex;
	/** @brief Instance of the CMU Camera. See documentation of CMU1394 driver. */
	C1394Camera camera;
	/** @brief Width of the captured image. */
	int imageWidth;
	/** @brief Height of the captured image. */
	int imageHeight;
	/** @brief Size of the buffer to store one frame. */
	int bufferSize;
	/** @brief The grabbed frame. */
	tt::ds::Image* currentFrame;
	/** @brief The grabbed frame as a RGB image */ 
	tt::ds::Image* currentRGBFrame;
	
public:
	/** @brief Initializes the members of the WindowsCMU1394Camera object. */
	WindowsCMU1394Camera();
	/** @brief Delete the allocated Image Frame Objects. */
	virtual ~WindowsCMU1394Camera();

	virtual void open();
	virtual void close();
	virtual void init();
	virtual void captureStart();
	virtual void captureStop();
	virtual void captureNext();
	virtual tt::ds::Image* getImage();
	virtual const int getImageWidth() const;
	virtual const int getImageHeight() const;
	virtual void getCaptureParameters(int& width, int& height, 
		ds::Image::Channels& channels, ds::Image::BitsPerChannel& bpc);
	virtual void enableWhiteBalanceOnePush(bool enable);
	virtual void enableWhiteBalanceAuto(bool enable);
	virtual void getWhiteBalance(unsigned int* ubValue, unsigned int* vrValue);
	virtual void setWhiteBalance(unsigned int ubValue, unsigned int vrValue);
	virtual void enableShutterAuto(bool enable);
	virtual void enableGainAuto(bool enable);

	// WindowsCMU1394Camera specific functions
	
	/** @brief Return the number of connected Firewire cameras, accessible by this driver. */
	static int getNumberOfWindowsCMU1394Cameras();
	
	/** @brief Select the camera with the specified index */
	void selectCamera(int index);
};

} // namespace input

} // namespace trax

#endif // WIN32

#endif /*TRAX_INPUT_WINDOWSCMU1394CAMERA_H*/
