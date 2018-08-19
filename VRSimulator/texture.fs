#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ftexcoord;

// texture samplers
uniform sampler2D tex;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = texture(tex, ftexcoord);
}
