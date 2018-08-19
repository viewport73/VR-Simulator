#version 410

in vec2 vertexPos;
in vec2 vertTexCoord;
out vec2 textureCoord;

void main ()
{
    textureCoord = vertTexCoord;
	// transform vertex position to clip space (camera view and perspective)
	gl_Position = vec4 (vertexPos, 0.0, 1.0);
}
