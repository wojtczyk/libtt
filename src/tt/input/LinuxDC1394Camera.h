#ifndef TRAX_INPUT_LINUXDC1394CAMERA_H
#define TRAX_INPUT_LINUXDC1394CAMERA_H

#ifdef LINUX // Build LinuxDC1394Camera only if compiled on a Linux platform

#include <libraw1394/raw1394.h>
#include <libdc1394/dc1394_control.h>
#include <tt/ds/Image.h>
#include "FirewireCamera.h"

namespace tt
{

namespace input
{

/**
 * @class LinuxDC1394Camera LinuxDC1394Camera.h trax/input/LinuxDC1394Camera.h
 * @brief Concrete class implementing the Camera interface for Firewire cameras.
 * @author Martin Wojtczyk <wojtczyk@in.tum.de>
 * 
 * LinuxDC1394Camera implements the Camera interface for Firewire cameras
 * on Linux platforms and is based on the two open source libraries
 * libraw1394 and libdc1394.
 */
class LinuxDC1394Camera : public tt::input::FirewireCamera
{
private:
	/** @brief Index of the camera */
	int cameraIndex;
	/** @brief Firewire adapter number. See documentation of libraw1394. */
	int card;
	/** @brief The libraw1394 handle. See documentation of libraw1394. */
	raw1394handle_t rawHandle;
	/** @brief The cameras node number on the Firewire bus. See documentation of libdc1394. */
	nodeid_t cameraNode;
	/** @brief The libdc1394 camera capture structure. See documentation of libdc1394. */
	dc1394_cameracapture camera;
	/** @brief Indicates, if we are in capture state or not. */
	bool capturing;
	/** @brief The libdc1394 channel. See documentation of libdc1394. */
	int channel;
	/** @brief The highest current ISO channel. See documentation of libdc1394. */
	static int currentChannel;
	/** @brief The libdc1394 speed. See documentation of libdc1394. */
	int speed;
	/** @brief The number of buffers libdc1394 should use for capturing images. See documentation of libdc1394. */
	int dmaBufNum;
	/** @brief Width of the captured image. */
	int imageWidth;
	/** @brief Height of the captured image. */
	int imageHeight;
	/** @brief Size of the buffer to store one frame. */
	int bufferSize;
	/** @brief The color setting for the current video mode. */
	Color colorMode;
	/** @brief The grabbed frame. */
	tt::ds::Image* currentFrame;
	/** @brief The grabbed frame as a RGB image */ 
	tt::ds::Image* currentRGBFrame;
	
public:
	LinuxDC1394Camera();
	virtual ~LinuxDC1394Camera();
	
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
	
	// LinuxDC1394Camera specific functions
	static int getNumberOfLinuxDC1394Cameras();
	
	/** @brief Select the camera with the specified index */
	void selectCamera(int index);
};

} // namespace input

} // namespace trax

#endif // LINUX

#endif /*TRAX_INPUT_LINUXDC1394CAMERA_H*/
