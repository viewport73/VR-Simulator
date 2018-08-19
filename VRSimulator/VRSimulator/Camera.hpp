//
//  Camera.hpp
//  VRSimulator
//
//  Created by Pramod S on 18/08/18.
//  Copyright © 2018 Studio73. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#define PI 3.14159265358979323846264338327950288
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
using namespace glm;

class Camera
{
public:
    
    Camera();
    mat4 SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    mat4 GetRotationMatrix();
    mat4 GetProjectionMatrix() { return ProjectionMatrix; }
    mat4 GetViewMatrix();
    
    void SetPosition(vec3 position)    { Position = position; }
    vec3 GetPosition()    { return Position; }
    vec3 GetView();
    vec3 GetUp();
    vec3 GetRight();
    float GetYaw() { return Yaw; }
    void SetYaw(float yaw)    { Yaw = yaw; }
    float GetPitch() { return Pitch; }
    void SetPitch(float pitch) { Pitch = pitch; }
    
    void SetSpeed(double speed) { Speed = speed;  }
    double GetSpeed() { return Speed; }
    
    void SetRotationSpeed(double speed) { RotationSpeed = speed; }
    double GetRotationSpeed() { return RotationSpeed; }
    
    void SetViewByMouse(float mouseX, float mouseY);
    void PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch);
    void MoveCamera(float speed);
    void Strafe(float speed);
    
protected:
    
    mat4 ProjectionMatrix;
    vec3 Position;
    
    float Speed = 5.0;
    double MouseSpeed = 0.001;
    float RotationSpeed = 2;
    float Yaw = 0;
    float Pitch = 0;
};
#endif /* Camera_hpp */
