#ifndef TT_INPUT_INPUTDEVICE_H
#define TT_INPUT_INPUTDEVICE_H

namespace tt
{

/**
 * @brief Namespace for all input devices.
 */
namespace input
{

/**
 * @class InputDevice InputDevice.h tt/input/InputDevice.h
 * @brief Base class for all input devices.
 * @author Martin Wojtczyk <wojtczyk@in.tum.de>
 */
class InputDevice
{
public:
	InputDevice();
	virtual ~InputDevice();
};

} // namespace input

} // namespace tt

#endif /*TT_INPUT_INPUTDEVICE_H*/
