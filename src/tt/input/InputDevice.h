#ifndef TRAX_INPUT_INPUTDEVICE_H
#define TRAX_INPUT_INPUTDEVICE_H

namespace tt
{

/**
 * @brief Namespace for all input devices.
 */
namespace input
{

/**
 * @class InputDevice InputDevice.h trax/input/InputDevice.h
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

} // namespace trax

#endif /*TRAX_INPUT_INPUTDEVICE_H*/
