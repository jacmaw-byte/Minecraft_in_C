
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
flat in float brightness;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * brightness;
    FragColor.a = 1.0;
}
