#ifndef TT_OUTPUT_MOVIERECORDER_H
#define TT_OUTPUT_MOVIERECORDER_H

#include "OutputDevice.h"

namespace tt
{

namespace output
{

class MovieRecorder : public tt::output::OutputDevice
{
public:
	MovieRecorder();
	virtual ~MovieRecorder();
};

} // namespace output

} // namespace tt

#endif /*TT_OUTPUT_MOVIERECORDER_H*/
