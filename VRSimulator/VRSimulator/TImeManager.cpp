//
//  TImeManager.cpp
//  VRSimulator
//
//  Created by Pramod S on 18/08/18.
//  Copyright © 2018 Studio73. All rights reserved.
//

#include "TImeManager.hpp"

using namespace std;
using namespace chrono;

double TimeManager::CalculateFrameRate(bool writeToConsole = false)
{
    static double framesPerSecond = 0.0f;
    static double startTime = GetTime();
    static double lastTime = GetTime();
    static char strFrameRate[50] = { 0 };
    static double currentFPS = 0.0f;
    CurrentTime = GetTime();
    
    DeltaTime = CurrentTime - lastTime;
    lastTime = CurrentTime;
    
    ++framesPerSecond;
    
    if ( CurrentTime - startTime > 1.0f )
    {
        startTime = CurrentTime;
        if ( writeToConsole )
            fprintf(stderr, "Current Frames Per Second: %d\n", int(framesPerSecond));
        currentFPS = framesPerSecond;
        
        framesPerSecond = 0;
    }
    
    return currentFPS;
}

double TimeManager::GetTime()
{
    auto beginningOfTime = system_clock::now().time_since_epoch();
    auto ms = duration_cast<milliseconds>(beginningOfTime).count();
    
    return ms * 0.001;
}

void TimeManager::Sleep(int ms)
{
    this_thread::sleep_for( milliseconds(ms) );
}
