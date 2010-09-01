/*
 * FirewireCamera
 * by Martin Wojtczyk <wojtczyk@in.tum.de>
 */

#include "FirewireCamera.h"
#include <string>

// include linux camera interface
#ifdef LINUX
	#include "LinuxDC1394Camera.h"
#endif
// include windows camera interface
#ifdef WIN32
	#include "WindowsCMU1394Camera.h"
#endif

namespace tt
{
	
namespace input
{

FirewireCamera::FirewireCamera() :
	videoFormat(FirewireCamera::FORMAT7),
	videoFormatSet(false),
	videoMode(FirewireCamera::MODE0),
	videoModeSet(false),
	videoFramerate(FirewireCamera::FRAMERATE_15),
	videoFramerateSet(false),
	bayerFilter(tt::process::Bayer::BayerRG2BGR)
{
}

FirewireCamera::~FirewireCamera()
{
}

int FirewireCamera::getNumberOfFirewireCameras()
{
	#if LINUX
		return LinuxDC1394Camera::getNumberOfLinuxDC1394Cameras();
	#elif WIN32
		return WindowsCMU1394Camera::getNumberOfWindowsCMU1394Cameras();
	#else
		return 0;
	#endif
}

FirewireCamera* FirewireCamera::createFirewireCamera(int cameraNumber)
{
	#if LINUX
		LinuxDC1394Camera* camera = new LinuxDC1394Camera();
		camera->selectCamera(cameraNumber);
		return camera;
	#elif WIN32
		WindowsCMU1394Camera* camera = new WindowsCMU1394Camera();
		camera->selectCamera(cameraNumber);
		return camera;
	#else
		return NULL;
	#endif
}

void FirewireCamera::deleteFirewireCamera(FirewireCamera* camera)
{
	// call virtual destructor of camera
	delete camera;
}

void FirewireCamera::setVideoFormat(input::FirewireCamera::Format format)
{
	this->videoFormat = format;
	this->videoFormatSet = true;
}

FirewireCamera::Format FirewireCamera::getVideoFormat() const
{	
	std::string functionSignature = "Format FirewireCamera::getVideoFormat() const";

	if (!videoFormatSet)
	{
		throw std::runtime_error(functionSignature + " video format was not set.");
	};

	return this->videoFormat;
}

const std::string& FirewireCamera::getVideoFormatString(Format format)
{
	static const std::string formatTable[8] = 
		{
			"0",
			"1",
			"2", 
			"", "",	"", "", /* 4 empty etries */
			"7"
		};

	static const std::string formatUnknown = "unknown format";
	
	switch (format)
	{
		case FirewireCamera::FORMAT0:
		case FirewireCamera::FORMAT1:
		case FirewireCamera::FORMAT2:
		case FirewireCamera::FORMAT7:
			return formatTable[format];
			break;
			
		default:
			return formatUnknown;
			break;
	};
	
	return formatUnknown;
}

void FirewireCamera::setVideoMode(Mode mode)
{
	std::string functionSignature = "void FirewireCamera::setVideoMode(Mode mode)";

	this->videoMode = mode;
	this->videoModeSet = true;
	
	switch (mode)
	{
		case MODE0:
		case MODE_640x480_MONO8:
		case MODE_800x600_MONO8:
		case MODE_1024x768_MONO8:
		case MODE_1280x960_MONO8:
		case MODE_1600x1200_MONO8:
			this->bayerFilter = tt::process::Bayer::BayerRG2BGR;
			break;
			
		default:
			this->bayerFilter = tt::process::Bayer::NONE;
			break;
	};
}

FirewireCamera::Mode FirewireCamera::getVideoMode() const
{
	std::string functionSignature = "Format FirewireCamera::getVideoMode() const";

	if (!videoModeSet)
	{
		throw std::runtime_error(functionSignature + " video mode was not set.");
	};

	return this->videoMode;
}

const std::string& FirewireCamera::getVideoModeString(Mode mode)
{
	static const std::string modeTable[136] = 
		{
			"0",
			"1",
			"", "", "", "", "", "", "", "", "", "", /* 62 empty entries */
			"", "", "", "", "", "", "", "", "", "",
			"", "", "", "", "", "", "", "", "", "",
			"", "", "", "", "", "", "", "", "", "",
			"", "", "", "", "", "", "", "", "", "",
			"", "", "", "", "", "", "", "", "", "",
			"", "",
			"160x120 YUV 4:4:4",
			"320x240 YUV 4:2:2",
			"640x480 YUV 4:1:1",
			"640x480 YUV 4:2:2",
			"640x480 RGB",
			"640x480 Mono 8bpp",
			"640x480 Mono 16bpp",
			"", "", "", "", "", "", "", "", "", "", /* 25 empty entries */
			"", "", "", "", "", "", "", "", "", "",
			"", "", "", "", "",
			"800x600 YUV 4:2:2",
			"800x600 RGB",
			"800x600 Mono 8bpp",
			"1024x768 YUV 4:2:2",
			"1024x768 RGB",
			"1024x768 Mono 8bpp",
			"800x600 Mono 16bpp",
			"1024x768 Mono 16bpp",
			"", "", "", "", "", "", "", "", "", "", /* 24 empty entries */
			"", "", "", "", "", "", "", "", "", "",
			"", "", "", "",
			"1280x960 YUV 4:2:2",
			"1280x960 RGB",
			"1280x960 Mono 8bpp",
			"1600x1200 YUV 4:2:2",
			"1600x1200 RGB",
			"1600x1200 Mono 8bpp",
			"1280x960 Mono 16bpp",
			"1600x1200 Mono 16bpp"
		};

	static const std::string modeUnknown = "unknown mode";

	switch (mode)
	{
		case FirewireCamera::MODE0:
		case FirewireCamera::MODE1:
		case FirewireCamera::MODE_160x120_YUV444:
		case FirewireCamera::MODE_320x240_YUV422:
		case FirewireCamera::MODE_640x480_YUV411:
		case FirewireCamera::MODE_640x480_YUV422:
		case FirewireCamera::MODE_640x480_RGB:
		case FirewireCamera::MODE_640x480_MONO8:
		case FirewireCamera::MODE_640x480_MONO16:
		case FirewireCamera::MODE_800x600_YUV422:
		case FirewireCamera::MODE_800x600_RGB:
		case FirewireCamera::MODE_800x600_MONO8:
		case FirewireCamera::MODE_1024x768_YUV422:
		case FirewireCamera::MODE_1024x768_RGB:
		case FirewireCamera::MODE_1024x768_MONO8:
		case FirewireCamera::MODE_800x600_MONO16:
		case FirewireCamera::MODE_1024x768_MONO16:
		case FirewireCamera::MODE_1280x960_YUV422:
		case FirewireCamera::MODE_1280x960_RGB:
		case FirewireCamera::MODE_1280x960_MONO8:
		case FirewireCamera::MODE_1600x1200_YUV422:
		case FirewireCamera::MODE_1600x1200_RGB:
		case FirewireCamera::MODE_1600x1200_MONO8:
		case FirewireCamera::MODE_1280x960_MONO16:
		case FirewireCamera::MODE_1600x1200_MONO16:
			return modeTable[mode];
			break;
			
		default:
			return modeUnknown;
			break;
	};

	return modeUnknown;
}

void FirewireCamera::setVideoFramerate(Framerate framerate)
{
	std::string functionSignature = "void FirewireCamera::setVideoFramerate(Framerate framerate)";

	this->videoFramerate = framerate;
	this->videoFramerateSet = true;
}

FirewireCamera::Framerate FirewireCamera::getVideoFramerate() const
{
	std::string functionSignature = "Framerate FirewireCamera::getVideoFramerate() const";

	if (!videoFramerateSet)
	{
		throw std::runtime_error(functionSignature + " video framerate was not set.");
	};

	return this->videoFramerate;
}

const std::string& FirewireCamera::getVideoFramerateString(Framerate framerate)
{
	static const std::string framerateTable[8] = 
		{
			"1.875",
			"3.75",
			"7.5",
			"15",
			"30",
			"60",
			"120",
			"240"
		};
	
	static const std::string framerateUnknown = "unknown framerate";

	switch (framerate)
	{
		case FirewireCamera::FRAMERATE_1_875:
		case FirewireCamera::FRAMERATE_3_75:
		case FirewireCamera::FRAMERATE_7_5:
		case FirewireCamera::FRAMERATE_15:
		case FirewireCamera::FRAMERATE_30:
		case FirewireCamera::FRAMERATE_60:
		case FirewireCamera::FRAMERATE_120:
		case FirewireCamera::FRAMERATE_240:
			return framerateTable[framerate];
			break;
			
		default:
			return framerateUnknown;
			break;
	};
	
	return framerateUnknown;
}

void FirewireCamera::setBayerFilter(process::Bayer::Filter filter)
{
	this->bayerFilter = filter;
}

process::Bayer::Filter FirewireCamera::getBayerFilter() const
{
	return this->bayerFilter;
}

} // namespace input

} // namespace trax
