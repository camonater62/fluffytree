#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;

out vec4 finalColor;

void main() {
    vec4 texelColor = texture(texture0, fragTexCoord);
    if (texelColor.r < 0.5) discard;
    vec4 leafColor = vec4(0.82, 0.34, 0.72, 1);
    finalColor = texelColor.r * leafColor;
}