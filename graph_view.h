// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#if !defined(__graph_view_h)
#define __graph_view_h
#include <mono.h>

class GraphView : public mono::ui::View
{
public:
	GraphView (uint8_t y, uint8_t height);
	void setNextPoint (float point);
protected:
    float oldData[176];
	virtual void repaint ();
private:
	void updateMinMax (float point);
	void deletePoints ();
	void drawPoints ();
	uint16_t convertPointToY (float point);
	void deletePoint (int16_t x, float value);
	void drawPoint (int16_t x, float value);
	uint8_t y;
	uint8_t height;
	float * data;
    size_t ixBegin, drawedIxBegin;
	size_t ixPastLastElement, drawedIxPastLastElement;
	float minData;
	float maxData;
};

#endif // __graph_view_h
