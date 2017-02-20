#ifndef app_controller_h
#define app_controller_h

#include <mono.h>
#include "icon_view.h"
#include "running_average_filter.h"
#include "graph_view.h"
#include <Ticker.h>

using namespace mono;
using namespace mono::ui;


class AppController : public mono::IApplication {

    Timer tim;
    Timer graphTim;
    TextLabelView accelLbl;
    ButtonView muteBtn, unmuteBtn, startStopBtn;
    IconView muteIcon, unmuteIcon, startStopIcon;
    ::GraphView graph;
    PowerSaver saver;
    mono::io::RunningAverageFilter<16, int16_t> filter;
    int16_t lastVal;
    bool _mute;
    
    mono::sensor::IAccelerometer *accel;
    
public:

    AppController();
    
    void sample();
    void paintGraph();
    void toggleAccel();
    
    void mute();
    void unmute();

    void monoWakeFromReset();
    void monoWillGotoSleep();
    void monoWakeFromSleep();

};

#endif /* app_controller_h */
