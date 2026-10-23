
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
flat in float brightness;

uniform sampler2D ourTexture;

void main()
{
    vec4 tex = texture(ourTexture, TexCoord);

    FragColor = vec4(tex.r * brightness, tex.g * brightness, tex.b * brightness, tex.a);
}
