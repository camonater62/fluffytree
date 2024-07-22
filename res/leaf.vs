#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

uniform mat4 mvp;

out vec2 fragTexCoord;
out vec4 fragColor;

uniform mat4 modelViewMatrix;

float inverseLerp(float v, float minValue, float maxValue) {
  return (v - minValue) / (maxValue - minValue);
}

float remap(float v, float inMin, float inMax, float outMin, float outMax) {
  float t = inverseLerp(v, inMin, inMax);
  return mix(outMin, outMax, t);
}

mat4 rotateZ(float radians) {
  float c = cos(radians);
  float s = sin(radians);

	return mat4(
    c, -s, 0, 0,
    s, c, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  );
}

vec2 calcInitialOffsetFromUVs() {
  vec2 offset = vec2(
    remap(vertexTexCoord.x, 0.0, 1.0, -1.0, 1.0),
    remap(vertexTexCoord.y, 0.0, 1.0, -1.0, 1.0)
  );

  // Invert the vertex offset so it's positioned towards the camera.
  offset *= vec2(-1.0, 1.0);
  offset = normalize(offset);

  return offset;
}

void main() {
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    vec2 vertexOffset = calcInitialOffsetFromUVs();

    vec4 worldViewPosition = modelViewMatrix * vec4(vertexPosition, 1.0) + vec4(vertexOffset, 0.0, 0.0);

    mat4 projectionMatrix = mvp * inverse(modelViewMatrix);

    gl_Position = projectionMatrix * worldViewPosition;
}
