#include "app_controller.h"

#include <icons/mute-24.h>
#include <icons/speaker-24.h>
#include <icons/play-button-24.h>
#include <icons/pause-symbol-24.h>
#include <math.h>

using mono::geo::Rect;
using mono::geo::Point;

AppController::AppController() :
    tim(10),
    graphTim(100),
    accelLbl(Rect(10,30,156,25), ""),
    muteBtn(Rect(10, 65, 35, 35), ""),
    unmuteBtn(Rect(166-35, 65, 35, 35), ""),
    startStopBtn(Rect(176/2-35/2, 65, 35, 35),""),
    muteIcon(Point(15, 70), mute24),
    unmuteIcon(Point(171-35, 70), speaker24),
    startStopIcon(Point(176/2-24/2, 70), pauseSymbol24),
    graph(110,110),
    saver(30000,10000)
{
    accelLbl.setAlignment(TextLabelView::ALIGN_CENTER);
    accelLbl.show();
    muteBtn.show();
    unmuteBtn.show();
    startStopBtn.show();
    muteIcon.show();
    unmuteIcon.show();
    startStopIcon.show();


    graph.show();

    _mute = true;
    
    tim.setCallback(this, &AppController::sample);
    muteBtn.setClickCallback(this, &AppController::mute);
    unmuteBtn.setClickCallback(this, &AppController::unmute);
    startStopBtn.setClickCallback(this, &AppController::toggleAccel);

    graphTim.setCallback(this, &AppController::paintGraph);
    graphTim.start();
}

void AppController::sample()
{
    filter.append(accel->rawXAxis());
    int16_t x = 128 - filter.value()/8;
    if (x < 4)
        x = 4;
    
    //accelLbl.setText(String::Format("%d (%d)", x, filter.value()));
    
    if (!_mute) {
        if (x > 255)
            x = 255;

        int  p = x;
//        if (x < lastVal)
//            p = PWM_ReadPeriod()+2;
//        else
//            p = PWM_ReadPeriod()-2;
//        
//        if (p < 1)
//            p = 1;
//        else if (p > 255)
//            p = 255;
#ifndef EMUNO
        PWM_WritePeriod(p);
        PWM_WriteCompare2(p/2);
#endif
    }
    else{
#ifndef EMUNO
        PWM_WriteCompare2(0);
        PWM_WritePeriod(255);
#endif
    }
    
    lastVal = x;
    static int cnt = 0;
    if (cnt++ >= 1)
    {
        cnt = 0;
        graph.setNextPoint(2600 - x*10);
    }
}

void AppController::paintGraph()
{
    graph.scheduleRepaint();
}

void AppController::toggleAccel()
{
    if (accel->IsActive())
    {
        accel->stop();
        tim.stop();
        graphTim.stop();
        mute();
        startStopIcon = IconView(Point(176/2-24/2, 70), playButton24);
        startStopIcon.show();
    }
    else
    {
        accel->start();
        tim.start();
        graphTim.start();
        startStopIcon = IconView(Point(176/2-24/2, 70), pauseSymbol24);
        startStopIcon.show();
    }

}

void AppController::mute()
{
    _mute = true;
    muteIcon.setForeground(display::GreenColor);
    unmuteIcon.setForeground(display::CloudsColor);
    muteIcon.scheduleRepaint();
    unmuteIcon.scheduleRepaint();
#ifndef EMUNO
    PWM_WriteCompare2(0);
#endif
}
void AppController::unmute()
{
    _mute = false;
    muteIcon.setForeground(display::CloudsColor);
    unmuteIcon.setForeground(display::GreenColor);
    muteIcon.scheduleRepaint();
    unmuteIcon.scheduleRepaint();
}


void AppController::monoWakeFromReset()
{
    accel = IApplicationContext::Instance->Accelerometer;
    tim.start();
    accel->Start();
    filter.clear(0);
    lastVal = 0;
#ifndef EMUNO
    PWM_WritePeriod(255);
#endif

    graph.setNextPoint(filter.value()*10);
}

void AppController::monoWillGotoSleep()
{
    accel->Stop();
}

void AppController::monoWakeFromSleep()
{
    accel->Start();
    saver.undim();
    
    muteBtn.scheduleRepaint();
    muteIcon.scheduleRepaint();
    unmuteBtn.scheduleRepaint();
    unmuteIcon.scheduleRepaint();
    startStopIcon.scheduleRepaint();
    startStopBtn.scheduleRepaint();
    graph.scheduleRepaint();
}
