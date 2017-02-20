#include "graph_view.h"

using mono::geo::Rect;

namespace {
	const uint8_t xMin = 1;
	const uint8_t xMax = 175;
}

GraphView::GraphView (uint8_t y_, uint8_t height_)
:
	View(Rect(xMin-1,y_,xMax-xMin+1,height_)),
	y(y_),
	height(height_),
	ixBegin(0),
    drawedIxBegin(0),
	ixPastLastElement(0),
    drawedIxPastLastElement(0)
{
	static float d[176];
	data = d;
    memcpy(oldData, data, sizeof(float)*176);
}

void GraphView::repaint ()
{
	maxData = 2600.0;
	minData = 0.0;

    painter.DisplayController()->setWindow(viewRect.X(), viewRect.Y(), viewRect.Width(), viewRect.Height());
    deletePoints();
	drawPoints();
    drawedIxPastLastElement = ixPastLastElement;
    drawedIxBegin = ixBegin;
    memcpy(oldData, data, sizeof(float)*176);
    
	painter.setForegroundColor(mono::display::MidnightBlueColor);
	painter.drawRect(viewRect);
}

void GraphView::deletePoints ()
{
    if (drawedIxPastLastElement < 1)
        return;

	int16_t x = 175;
	for (size_t i = drawedIxPastLastElement; i > 0; --i)
	{
		deletePoint(x,oldData[i-1]);
		--x;
	}
	if (drawedIxBegin == drawedIxPastLastElement) for (size_t i = 176; i > drawedIxBegin; --i)
	{
		if (x < 1) break;
		deletePoint(x,oldData[i-1]);
		--x;
	}
}

void GraphView::drawPoints ()
{
	int16_t x = 175;
	for (size_t i = ixPastLastElement; i > 0; --i)
	{
		drawPoint(x,data[i-1]);
		--x;
	}
	if (ixBegin == ixPastLastElement) for (size_t i = 176; i > ixBegin; --i)
	{
		if (x < 1) break;
		drawPoint(x,data[i-1]);
		--x;
	}
}

void GraphView::deletePoint (int16_t x, float value)
{
    if (x < 1)
        return;

    mono::display::IDisplayController *ctrl = painter.DisplayController();

	if (value > maxData)
	{
		uint16_t point = convertPointToY(maxData);
        //painter.drawRect(x, point, 2, 2, true);
		//painter.drawPixel(x,point,true);
        ctrl->setCursor(x,point);
        ctrl->write(painter.BackgroundColor());
        ctrl->setCursor(x,point+1);
        ctrl->write(painter.BackgroundColor());
	}
	else if (value <= minData)
	{
		uint16_t point = convertPointToY(minData);
        //painter.drawRect(x, point, 2, 2, true);
        //painter.drawPixel(x,point,true);
        ctrl->setCursor(x,point);
        ctrl->write(painter.BackgroundColor());
        ctrl->setCursor(x,point+1);
        ctrl->write(painter.BackgroundColor());

	}
	else
	{
		uint16_t point = convertPointToY(value);
        //painter.drawRect(x, point, 2, 2, true);
		//painter.drawPixel(x,point,true);
        ctrl->setCursor(x,point);
        ctrl->write(painter.BackgroundColor());
        ctrl->setCursor(x,point+1);
        ctrl->write(painter.BackgroundColor());
	}
}

void GraphView::drawPoint (int16_t x, float value)
{
    if (x < 1)
        return;

    mono::display::IDisplayController *ctrl = painter.DisplayController();

	if (value > maxData)
	{
		uint16_t point = convertPointToY(maxData);
		painter.setForegroundColor(mono::display::RedColor);
        //painter.drawRect(x, point, 2, 2);
		//painter.drawPixel(x,point);
        ctrl->setCursor(x,point);
        ctrl->write(painter.ForegroundColor());
        ctrl->setCursor(x,point+1);
        ctrl->write(painter.ForegroundColor());
	}
	else if (value <= minData)
	{
		uint16_t point = convertPointToY(minData);
		painter.setForegroundColor(mono::display::RedColor);
        //painter.drawRect(x, point, 2, 2);
		//painter.drawPixel(x,point);
        ctrl->setCursor(x,point);
        ctrl->write(painter.ForegroundColor());
        ctrl->setCursor(x,point+1);
        ctrl->write(painter.ForegroundColor());
	}
	else
	{
		uint16_t point = convertPointToY(value);
		painter.setForegroundColor(mono::display::AlizarinColor);
        //painter.drawRect(x, point, 2, 2);
		//painter.drawPixel(x,point);
        ctrl->setCursor(x,point);
        ctrl->write(painter.ForegroundColor());
        ctrl->setCursor(x,point+1);
        ctrl->write(painter.ForegroundColor());
	}
}

uint16_t GraphView::convertPointToY (float point)
{
	float scaledY = (point - minData) / (maxData - minData + 1);
	float p = height + y - (scaledY * height);
	return p - 2;
}

void GraphView::setNextPoint (float point)
{
	if (ixBegin == 0 && ixPastLastElement != 176)
	{
		++ixPastLastElement;
	}
	else
	{
		++ixPastLastElement;
		++ixBegin;
		if (ixPastLastElement > 176) ixPastLastElement = 1;
		if (ixBegin >= 176) ixBegin = 0;
	}
	data[ixPastLastElement-1] = point;
}

void GraphView::updateMinMax (float point)
{
	if (minData > point) minData = point;
	if (maxData < point) maxData = point;
}
