#version 330
uniform mat4 projMatrix;
uniform vec3 lightDir;
uniform vec3 cameraPosition;
//uniform int shadingType;
// texture samplers
uniform sampler2D tex;

in vec4 vertexViewspace; // this guy should be the surface point.

in vec3 U, V, H;
in float cylinderRadiusv;
in float cylinderLengthv;

in vec4 cylinderOrigin;
in vec3 localCoords;
in vec2 ftexcoord;

out vec4 FragColor;

void main()
{
    // First of all, I need the correct point that we're pointing at
    
    vec3 surfacePoint = cylinderOrigin.xyz;
    surfacePoint += U * (localCoords.x * 2.0 - 1.0) * cylinderRadiusv;
    surfacePoint += V * (localCoords.y * 2.0 - 1.0) * cylinderRadiusv;
    surfacePoint += H * (localCoords.z * cylinderLengthv);
    
    // We need to raytrace the cylinder (!)
    // we can do the intersection in impostor space
    
    // Calculate the ray direction in viewspace
    vec3 rayOrigin = vec3(0.0, 0.0, 0.0);
    vec3 rayTarget = surfacePoint;
    vec3 rayDirection = normalize(rayTarget - rayOrigin);
    
    // basis = local coordinate system of cylinder
    mat3 basis = transpose(mat3(U, V, H));
    
    vec3 base = cylinderOrigin.xyz;
    vec3 endCylinder = cylinderOrigin.xyz + H * cylinderLengthv;
    
    // Origin of the ray in cylinder space
    vec3 P = - cylinderOrigin.xyz;
    P = basis * P;
    
    // Direction of the ray in cylinder space
    vec3 D = basis * rayDirection;
    
    // Now the intersection is between z-axis aligned cylinder and a ray
    float c = P.x * P.x + P.y * P.y - cylinderRadiusv * cylinderRadiusv;
    float b = 2.0 * (P.x * D.x + P.y * D.y);
    float a = D.x * D.x + D.y * D.y;
    
    float d = b * b - 4 * a * c;
    
    if (d < 0.0)
        discard;
    
    float t = (-b - sqrt(d)) / (2 * a);
    vec3 newPoint = rayOrigin + t * rayDirection;
    // Discarding points outside cylinder
    float outsideTop = dot(newPoint - endCylinder, -H);
    if (outsideTop < 0.0) {
        discard;
    }
    float outsideBottom = dot(newPoint - base, H);
    if (outsideBottom < 0.0) {
        discard;
    }
    
    vec3 tempPoint = newPoint - cylinderOrigin.xyz;
    vec3 normal = normalize(tempPoint - H * dot(tempPoint, H));
    
    // Extracting the z component
    vec4 projectedPoint = projMatrix * vec4(newPoint, 1.0);
    projectedPoint /= projectedPoint.w;
    
    gl_FragDepth = projectedPoint.z * 0.5 + 0.5;
    
    FragColor = texture(tex, ftexcoord);
}
