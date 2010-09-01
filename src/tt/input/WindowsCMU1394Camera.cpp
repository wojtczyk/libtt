/*
 * WindowsCMU1394Camera
 * by Martin Wojtczyk <wojtczyk@in.tum.de>
 */

#ifdef WIN32 // Build WindowsCMU1394Camera only if compiled on a Win32 platform

#include <string>
#include <sstream>
#include <assert.h>
#include <tt/process/Bayer.h>
#include "WindowsCMU1394Camera.h"

#include <iostream>

using namespace std;
using namespace tt::process;
using namespace tt::ds;

namespace tt
{

namespace input
{

WindowsCMU1394Camera::WindowsCMU1394Camera()
{
	this->cameraIndex = 0;
	this->imageWidth = 0;
	this->imageHeight = 0;
	this->capturing = false;
	this->currentFrame = NULL;
	this->currentRGBFrame = NULL;
};

WindowsCMU1394Camera::~WindowsCMU1394Camera()
{
	/*
	if (currentFrame != NULL)
	{
		delete currentFrame;
	};
	if (currentRGBFrame != NULL)
	{
		delete currentRGBFrame;
	};
	*/
};

void WindowsCMU1394Camera::open()
{
	string functionSignature = "void WindowsCMU1394Camera::open()";
	
	this->captureStop();
	this->camera.RefreshCameraList();

	this->camera.SelectCamera(this->cameraIndex);
	int result = this->camera.InitCamera(true);
	if (result != CAM_SUCCESS)
	{
		throw std::runtime_error(functionSignature + " no camera found.");
	};
};

void WindowsCMU1394Camera::close()
{
	this->captureStop();
};

void WindowsCMU1394Camera::init()
{
	string functionSignature = "void WindowsCMU1394Camera::init()";
	
	this->captureStop();
	this->camera.SelectCamera(this->cameraIndex);
	int result = this->camera.InitCamera(true);
	if (result != CAM_SUCCESS)
	{
		throw std::runtime_error(functionSignature + " no camera found.");
	};
};

void WindowsCMU1394Camera::captureStart()
{
	string functionSignature = "void WindowsCMU1394Camera::captureStart()";
	
	int result;

	// Set the video format according to the class FirewireCamera
	if (this->videoFormatSet == true)
	{
		result = this->camera.SetVideoFormat(this->videoFormat);

		if (result != CAM_SUCCESS)
		{
			throw std::runtime_error(functionSignature + 
				" camera does not support Format " + 
				FirewireCamera::getVideoFormatString(this->videoFormat) + ".");
		};
	};
	
	// Set the video mode according to the class FirewireCamera
	if (this->videoModeSet == true)
	{
		switch (this->videoMode)
		{
			// Format 7 modes - not supported by CMU driver according to
			// documentation. So we just ignore it.
			case MODE0:
				// result = this->camera.SetVideoMode(0);
				break;

			case MODE1:
				// result = this->camera.SetVideoMode(1);
				break;

			// Format 0 modes 
			case MODE_160x120_YUV444:
				result = this->camera.SetVideoMode(0);
				break;

			case MODE_320x240_YUV422:
				result = this->camera.SetVideoMode(1);
				break;

			case MODE_640x480_YUV411:
				result = this->camera.SetVideoMode(2);
				break;

			case MODE_640x480_YUV422:
				result = this->camera.SetVideoMode(3);
				break;

			case MODE_640x480_RGB:
				result = this->camera.SetVideoMode(4);
				break;

			case MODE_640x480_MONO8:
				result = this->camera.SetVideoMode(5);
				break;

			case MODE_640x480_MONO16:
				result = this->camera.SetVideoMode(6);
				break;

			// Format 1 modes 
			case MODE_800x600_YUV422:
				result = this->camera.SetVideoMode(0);
				break;

			case MODE_800x600_RGB:
				result = this->camera.SetVideoMode(1);
				break;

			case MODE_800x600_MONO8:
				result = this->camera.SetVideoMode(2);
				break;

			case MODE_1024x768_YUV422:
				result = this->camera.SetVideoMode(3);
				break;

			case MODE_1024x768_RGB:
				result = this->camera.SetVideoMode(4);
				break;

			case MODE_1024x768_MONO8:
				result = this->camera.SetVideoMode(5);
				break;

			case MODE_800x600_MONO16:
				result = this->camera.SetVideoMode(6);
				break;

			case MODE_1024x768_MONO16:
				result = this->camera.SetVideoMode(7);
				break;

			// Format 2 modes 
			case MODE_1280x960_YUV422:
				result = this->camera.SetVideoMode(0);
				break;

			case MODE_1280x960_RGB:
				result = this->camera.SetVideoMode(1);
				break;

			case MODE_1280x960_MONO8:
				result = this->camera.SetVideoMode(2);
				break;

			case MODE_1600x1200_YUV422:
				result = this->camera.SetVideoMode(3);
				break;

			case MODE_1600x1200_RGB:
				result = this->camera.SetVideoMode(4);
				break;

			case MODE_1600x1200_MONO8:
				result = this->camera.SetVideoMode(5);
				break;

			case MODE_1280x960_MONO16:
				result = this->camera.SetVideoMode(6);
				break;

			case MODE_1600x1200_MONO16:
				result = this->camera.SetVideoMode(7);
				break;
		};

		if (result != CAM_SUCCESS)
		{
			throw std::runtime_error(functionSignature + 
				" camera does not support Mode " + 
				FirewireCamera::getVideoModeString(this->videoMode) + ".");
		};
	};

	// Set the video framerate according to the class FirewireCamera
	if (this->videoFramerateSet == true)
	{
		result = this->camera.SetVideoFrameRate(this->videoFramerate);

		if (result != CAM_SUCCESS)
		{
			throw std::runtime_error(functionSignature + 
				" camera does not support Framerate " + 
				FirewireCamera::getVideoFramerateString(this->videoFramerate) + 
				".");
		};
	};
	
	result = this->camera.StartImageCapture();
	if (result != CAM_SUCCESS)
	{
		throw std::runtime_error(functionSignature + " unable to start transmission from camera.");
	};
	this->capturing = true;
	
	unsigned long width;
	unsigned long height;
	camera.GetVideoFrameDimensions(&width, &height);
	this->imageWidth = width;
	this->imageHeight = height;
	
	// allocate Images to store frames
	if (currentFrame != NULL)
	{
		delete currentFrame;
	};
	currentFrame = new Image(this->imageWidth, this->imageHeight, Image::GREYSCALE);
	if (currentRGBFrame != NULL)
	{
		delete currentRGBFrame;
	};
	currentRGBFrame = new Image(this->imageWidth, this->imageHeight, Image::RGB);
};

void WindowsCMU1394Camera::captureStop()
{
	string functionSignature = "void WindowsCMU1394Camera::captureStop()";

	if (this->capturing == true)
	{
		int result = this->camera.StopImageCapture();
		this->capturing = false;
		if (result != CAM_SUCCESS)
		{
			throw std::runtime_error(functionSignature + " could not stop camera.");
		};
	};
};

void WindowsCMU1394Camera::captureNext()
{
	string functionSignature = "void WindowsCMU1394Camera::captureNext()";

	if (this->capturing == true)
	{	
		int result = this->camera.AcquireImage();
		if (result != CAM_SUCCESS)
		{
			throw std::runtime_error(functionSignature + " could not capture frame.");
		};
	};
};

tt::ds::Image* WindowsCMU1394Camera::getImage()
{
	if (this->capturing == true)
	{
		if (this->bayerFilter != tt::process::Bayer::NONE)
		{ // manual debayering 
			unsigned long cameraBufferLength;
			unsigned char* cameraBuffer = this->camera.GetRawData(&cameraBufferLength);
	
			// copy from camera buffer to grey image
			unsigned char* greyImage = this->currentFrame->getImageBuffer();
			unsigned long greyImageLength = this->currentFrame->getAllocatedBytes();
			// note: the cameraBuferLength can be greater than the greyImageLength
			// that was one day of debugging.
			memcpy(greyImage, cameraBuffer, greyImageLength); 
			
			Bayer::deBayer(this->currentFrame, this->currentRGBFrame, this->bayerFilter);
		}
		else
		{ // use RGB auto multiplexer from CMU driver
			this->camera.getRGB((unsigned char*) (this->currentRGBFrame->getImageBuffer()), this->currentRGBFrame->getAllocatedBytes());
		}
		return this->currentRGBFrame;
	};
	return NULL;
};

const int WindowsCMU1394Camera::getImageWidth() const
{
	return this->imageWidth;
};

const int WindowsCMU1394Camera::getImageHeight() const
{
	return this->imageHeight;	
};

void WindowsCMU1394Camera::getCaptureParameters(int& width, int& height, 
	ds::Image::Channels& channels, ds::Image::BitsPerChannel& bpc)
{
	width = this->imageWidth;
	height = this->imageHeight;
	channels = ds::Image::RGB;
	bpc = ds::Image::BPC8;
};

void WindowsCMU1394Camera::enableWhiteBalanceOnePush(bool enable)
{
	C1394CameraControl whiteBalance(&camera, FEATURE_WHITE_BALANCE);
	whiteBalance.SetOnePush(enable);
};

void WindowsCMU1394Camera::enableWhiteBalanceAuto(bool enable)
{
	C1394CameraControl whiteBalance(&camera, FEATURE_WHITE_BALANCE);
	whiteBalance.SetAutoMode(enable);
};

void WindowsCMU1394Camera::getWhiteBalance(unsigned int* ubValue, unsigned int* vrValue)
{
	unsigned short subValue;
	unsigned short svrValue;

	C1394CameraControl whiteBalance(&camera, FEATURE_WHITE_BALANCE);
	whiteBalance.Status(); // Read the register from the cam
	whiteBalance.GetValue(&subValue, &svrValue);
	
	*ubValue = (unsigned int) subValue;
	*vrValue = (unsigned int) svrValue;
};

void WindowsCMU1394Camera::setWhiteBalance(unsigned int ubValue, unsigned int vrValue)
{
	C1394CameraControl whiteBalance(&camera, FEATURE_WHITE_BALANCE);
	whiteBalance.SetValue(ubValue, vrValue);
};

void WindowsCMU1394Camera::enableShutterAuto(bool enable)
{
	C1394CameraControl shutter(&camera, FEATURE_SHUTTER);
	shutter.SetAutoMode(enable);
};

void WindowsCMU1394Camera::enableGainAuto(bool enable)
{
	C1394CameraControl gain(&camera, FEATURE_GAIN);
	gain.SetAutoMode(enable);
};

int WindowsCMU1394Camera::getNumberOfWindowsCMU1394Cameras()
{
	// We need a local instance of a C1394Camera for this static function. 
	C1394Camera camera;
	int cameras = camera.RefreshCameraList();
	return cameras;
};

void WindowsCMU1394Camera::selectCamera(int index)
{
	this->cameraIndex = index;
	this->camera.SelectCamera(this->cameraIndex);
};

} // namespace input

} // namespace trax

#endif // WIN32
