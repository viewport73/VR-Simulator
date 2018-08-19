#version 410

in vec2 textureCoord;
uniform sampler2D textureSampler;
out vec4 fragmentColour;

void main ()
{
	// invert colour of right-hand side
	vec3 colour;
	if (textureCoord.s >= 0.5)
    {
		colour = 1.0 - texture (textureSampler, textureCoord).rgb;
	}
    else
    {
		colour = texture (textureSampler, textureCoord).rgb;
	}
	fragmentColour = vec4 (colour, 1.0);
	//fragmentColour = texture (tex, textureCo);
}

#version 150
uniform sampler2D tex; //this is the texture
in vec2 fragTexCoord; //this is the texture coord
out vec4 finalColor; //this is the output color of the pixel

void main() {
    finalColor = texture(tex, fragTexCoord);
}
