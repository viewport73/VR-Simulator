#version 330

uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat4 mvrMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec2 vtexcoord;

vec3 cylinderAxis = vec3(0.0f, 1.0f, 0.0f);
float cylinderRadius = 0.5f;
vec3 vertLocalCoordinate;

out vec4 vertexViewspace;
out float cylinderRadiusv;
out float cylinderLengthv;
out vec3 U, V, H;
out vec4 cylinderOrigin;
out vec3 localCoords;
out vec2 ftexcoord;


void main()
{
    vertLocalCoordinate = vposition.xyz;
    float cylinderLength = length(cylinderAxis);
    
    cylinderLengthv = cylinderLength;
    cylinderRadiusv = cylinderRadius;
    
    // We compute the bounding box
    
    // We receive 8 points, and we should place this 8 points
    // at their bounding box position
    vec4 cylinderBase = vec4(vposition.xyz, 1.0);
    cylinderOrigin = mvMatrix * cylinderBase;
    cylinderOrigin /= cylinderOrigin.w;
    
    // We receive from the program the origin that is the cylinder start
    // point. To this guy we have to add the local coordinates.
    
    // Local vectors, u, v, h
    vec3 u, h, v;
    
    h = normalize(cylinderAxis);
    u = cross(h, vec3(1.0, 0.0, 0.0));
    if (length(u) < 0.001)
    {
        u = cross(h, vec3(0.0, 0.0, 1.0));
    }
    u = normalize(u);
    v = normalize(cross(u, h));
    
    // We do the addition in object space
    vec4 vertex = cylinderBase;
    vertex.xyz += u * (vertLocalCoordinate.x * 2.0 - 1.0) * cylinderRadius;
    vertex.xyz += v * (vertLocalCoordinate.y * 2.0 - 1.0) * cylinderRadius;
    vertex.xyz += h * vertLocalCoordinate.z * cylinderLength;
    
    // Vertex in view space
    vertexViewspace = vertex * mvMatrix;
    //vertex_viewspace /= vertex_viewspace.w;
    
    // Base vectors of cylinder in view space
    U = normalize(mat3(mvrMatrix) * u);
    V = normalize(mat3(mvrMatrix) * v);
    H = normalize(mat3(mvrMatrix) * h);
    
    // Projecting
    vertex = mvpMatrix * vertex;
    
    // To reconstruct the current fragment position, I pass the local
    // coordinates
    localCoords = vertLocalCoordinate;
    
    gl_Position = vposition;
    ftexcoord = vtexcoord;
}
