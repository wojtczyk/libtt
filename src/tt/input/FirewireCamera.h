#ifndef TT_INPUT_FIREWIRECAMERA_H
#define TT_INPUT_FIREWIRECAMERA_H

#include "ImageDevice.h"
#include <tt/process/Bayer.h>

namespace tt
{
	
namespace input
{

/**
 * @class FirewireCamera FirewireCamera.h tt/input/FirewireCamera.h
 * @brief Interface class with factory functions for Firewire cameras.
 * @author Martin Wojtczyk <wojtczyk@in.tum.de>
 * 
 * FirewireCamera is the base class for operating system specific
 * implementations of Firewire Cameras. Use this class to instantiate 
 * FirewireCamera Objects.
 */
class FirewireCamera : public tt::input::ImageDevice
{
public:	
	enum Format
	{
		FORMAT0 = 0,
		FORMAT1 = 1,
		FORMAT2 = 2,
		FORMAT7 = 7
	};
	
	enum Mode
	{
		/* Format 7 modes */
		MODE0 = 0,
		MODE1 = 1,
		/* Format 0 modes */
		MODE_160x120_YUV444 = 64,
		MODE_320x240_YUV422 = 65,
		MODE_640x480_YUV411 = 66,
		MODE_640x480_YUV422 = 67,
		MODE_640x480_RGB = 68,
		MODE_640x480_MONO8 = 69,
		MODE_640x480_MONO16 = 70,
		/* Format 1 modes */
		MODE_800x600_YUV422 = 96,
		MODE_800x600_RGB = 97,
		MODE_800x600_MONO8 = 98,
		MODE_1024x768_YUV422 = 99,
		MODE_1024x768_RGB = 100,
		MODE_1024x768_MONO8 = 101,
		MODE_800x600_MONO16 = 102,
		MODE_1024x768_MONO16 = 103,
		/* Format 2 modes */
		MODE_1280x960_YUV422 = 128,
		MODE_1280x960_RGB = 129,
		MODE_1280x960_MONO8 = 130,
		MODE_1600x1200_YUV422 = 131,
		MODE_1600x1200_RGB = 132,
		MODE_1600x1200_MONO8 = 133,
		MODE_1280x960_MONO16 = 134,
		MODE_1600x1200_MONO16 = 135
	};
	
	/**
	 * @brief Constants 
	 */
	enum Framerate
	{
		FRAMERATE_1_875 = 0,
		FRAMERATE_3_75 = 1,
		FRAMERATE_7_5 = 2,
		FRAMERATE_15 = 3,
		FRAMERATE_30 = 4,
		FRAMERATE_60 = 5,
		FRAMERATE_120 = 6,
		FRAMERATE_240 = 7
	};
	
	enum Color
	{
		COLOR_RGB = 0,
		COLOR_GREYSCALE = 1,
		COLOR_YUV422 = 2
	};
	
protected:
	/** @brief The video format for this Firewire Camera. */
	Format videoFormat;

	/** @brief True if setVideoFormat was called, false otherwise.*/
	bool videoFormatSet;

	/** @brief The video mode for this Firewire Camera. */
	Mode videoMode;

	/** @brief True if setVideoMode was called, false otherwise.*/
	bool videoModeSet;

	/** @brief The video framerate for this Firewire Camera. */
	Framerate videoFramerate;

	/** @brief True if setVideoFramerate was called, false otherwise.*/
	bool videoFramerateSet;
	
	/** @brief The Bayer filter for color conversion. */
	tt::process::Bayer::Filter bayerFilter;

public:
	FirewireCamera();
	virtual ~FirewireCamera();
	
	/**
	 * @brief Return the number of connected Firewire Cameras.
	 */ 
	static int getNumberOfFirewireCameras();

	/**
	 * @brief Factory function to create a FirewireCamera instance.
	 * @param index of the camera to connect to
	 * @return The created Camera object.
	 */
	static FirewireCamera* createFirewireCamera(int cameraNumber = 0);

	/**
	 * @brief Destroys a camera object by calling its virtual destructor.
	 * @param camera The camera object to destroy.
	 */
	static void deleteFirewireCamera(FirewireCamera* camera);

	/**
	 * @brief Set the video format.
	 * @param format The desired video format.
	 * 
	 * This method sets the protected videoFormat attribute. However this method
	 * can be overwritten to provide custom functionality. After completion the 
	 * following attributes are set to
	 * videoFormatSet = true;
	 * videoModeSet = false;
	 * videoFramerateSet = false;
	 */
	virtual void setVideoFormat(input::FirewireCamera::Format format);
	
	/**
	 * @brief Return the current video format.
	 * 
	 * This method returns the protected videoFormat attribute. However this
	 * method can be overwritten to provide custom functionality.
	 */
	virtual Format getVideoFormat() const;

	/**
	 * @brief Return a string for a given format enum variable.
	 * @param format the value for the string determination.
	 */ 
	static const std::string& getVideoFormatString(Format format);

	/**
	 * @brief Set the video mode.
	 * @param mode The desired video mode.
	 * 
	 * This method sets the protected videoMode attribute. However this method
	 * can be overwritten to provide custom functionality. After completion the 
	 * following attributes are set to
	 * videoModeSet = true;
	 * videoFramerateSet = false;
	 */
	virtual void setVideoMode(Mode mode);
	
	/**
	 * @brief Return the current video mode.
	 * 
	 * This method returns the protected videoMode attribute. However this
	 * method can be overwritten to provide custom functionality.
	 */
	virtual Mode getVideoMode() const;
	
	/**
	 * @brief Return a string for a given mode enum variable.
	 * @param mode the value for the string determination.
	 */ 
	static const std::string& getVideoModeString(Mode mode);

	/**
	 * @brief Set the video framerate.
	 * @param mode The desired video framerate.
	 * 
	 * This method sets the protected videoFramerate attribute. However this method
	 * can be overwritten to provide custom functionality. After completion the 
	 * following attributes are set to
	 * videoFramerateSet = true;
	 */
	virtual void setVideoFramerate(Framerate framerate);
	
	/**
	 * @brief Return the current video framerate.
	 * 
	 * This method returns the protected videoFramerate attribute. However this
	 * method can be overwritten to provide custom functionality.
	 */
	virtual Framerate getVideoFramerate() const;

	/**
	 * @brief Return a string for a given framerate enum variable.
	 * @param framerate the value for the string determination.
	 */ 
	static const std::string& getVideoFramerateString(Framerate framerate);

	/**
	 * @brief Return the current Bayer filter.
	 * @param The desired Bayer filter.
	 * 
	 * This method sets the protected bayerFilter attribute. However this
	 * method can be overwritten to provide custom functionality. By default
	 * bayerFilter is initialised with Bayer::BayerRG2BGR.
	 */
	virtual void setBayerFilter(process::Bayer::Filter filter);

	/**
	 * @brief Return the current Bayer filter.
	 * 
	 * This method returns the protected bayerFilter attribute. However this
	 * method can be overwritten to provide custom functionality.
	 */
	virtual process::Bayer::Filter getBayerFilter() const;

	virtual void getCaptureParameters(int& width, int& height, 
		ds::Image::Channels& channels, ds::Image::BitsPerChannel& bpc) = 0;
	virtual void enableWhiteBalanceOnePush(bool enable) = 0;
	virtual void enableWhiteBalanceAuto(bool enable) = 0;
	virtual void getWhiteBalance(unsigned int* ubValue, unsigned int* vrValue) = 0;
	virtual void setWhiteBalance(unsigned int ubValue, unsigned int vrValue) = 0;
	virtual void enableShutterAuto(bool enable) = 0;
	virtual void enableGainAuto(bool enable) = 0;

	
};

} // namespace input

} // namespace tt

#endif /*TT_INPUT_FIREWIRECAMERA_H*/
