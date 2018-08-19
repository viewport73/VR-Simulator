//
//  TImeManager.hpp
//  VRSimulator
//
//  Created by Pramod S on 18/08/18.
//  Copyright © 2018 Studio73. All rights reserved.
//

#ifndef TImeManager_hpp
#define TImeManager_hpp

#include <stdio.h>
#include <chrono>
#include <thread>
class TimeManager
{
public:
    static TimeManager& Instance()
    {
        static TimeManager instance;
        
        return instance;
    }
    
    double CalculateFrameRate(bool writeToConsole);
    double GetTime();
    void Sleep(int milliseconds);
    double DeltaTime = 0;
    double CurrentTime = 0;
    
private:
    TimeManager(){};                                // Private so that it can  not be called
    TimeManager(TimeManager const&);                // copy constructor is private
    TimeManager& operator=(TimeManager const&);        // assignment operator is private
};

#endif /* TImeManager_hpp */
