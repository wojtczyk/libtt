/*
 * OpenCVCamera
 * by Martin Wojtczyk <wojtczyk@in.tum.de>
 */

#include "OpenCVCamera.h"

namespace tt
{

namespace input
{

OpenCVCamera::OpenCVCamera()
{
}

OpenCVCamera::~OpenCVCamera()
{
}

void OpenCVCamera::open()
{
}

void OpenCVCamera::close()
{
}

void OpenCVCamera::init()
{
}

void OpenCVCamera::captureStart()
{
}

void OpenCVCamera::captureStop()
{
}

void OpenCVCamera::captureNext()
{
}

tt::ds::Image* OpenCVCamera::getImage()
{
	return NULL;
}

const int OpenCVCamera::getImageWidth() const
{
	return -1;
}

const int OpenCVCamera::getImageHeight() const
{
	return -1;
}

} // namespace input

} // namespace tt
