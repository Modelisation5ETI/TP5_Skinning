#version 120

in vec3 position;
in vec4 weight;

uniform mat4 camera_projection;
uniform mat4 camera_modelview;

varying vec4 color;

void main (void)
{
  gl_Position = camera_projection*camera_modelview*vec4(position,1);

  color = vec4( weight);
}
