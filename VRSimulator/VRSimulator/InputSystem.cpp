//
//  InputSystem.cpp
//  VRSimulator
//
//  Created by Pramod S on 18/08/18.
//  Copyright © 2018 Studio73. All rights reserved.
//

#include "InputSystem.hpp"

void InputSystem::KeyPressed(InputCodes code)
{
    if ( Camera == nullptr )
        return;
    switch ( code )
    {
        case Up: case W: case w:
            Camera->MoveCamera(Camera->GetSpeed() * TimeManager::Instance().DeltaTime);
            break;
        case Down: case S: case s:
            Camera->MoveCamera(-1 * Camera->GetSpeed() * TimeManager::Instance().DeltaTime);
            break;
        case Left: case A: case a:
            Camera->Strafe(-1 * Camera->GetSpeed() * TimeManager::Instance().DeltaTime);
            break;
        case Right: case D: case d:
            Camera->Strafe(Camera->GetSpeed() * TimeManager::Instance().DeltaTime);
            break;
    }
}

void InputSystem::MouseMoved(float mouseX, float mouseY)
{
    if ( Camera == nullptr )
        return;
    
    Camera->SetViewByMouse(mouseX, mouseY);
}
