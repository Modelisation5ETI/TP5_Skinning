#version 130

in vec3 position;
in vec3 normal;
in vec2 text_coord;

in vec4 weight;
in ivec4 jointID;

uniform mat4 camera_projection;
uniform mat4 camera_modelview;
uniform mat4 skeleton_geometry[39];//We limit geometry to 39 bones

out vec2 v_text_coord;
out vec3 v_position;
out vec3 v_normal;

out vec4 v_weight;
flat out ivec4 v_jointID;

void main (void)
{
  vec4 p = vec4(0,0,0,0);
  p = p + weight.x * ( skeleton_geometry[jointID.x] * vec4(position,1) );
  p = p + weight.y * ( skeleton_geometry[jointID.y] * vec4(position,1) );
  p = p + weight.z * ( skeleton_geometry[jointID.z] * vec4(position,1) );
  p = p + weight.w * ( skeleton_geometry[jointID.w] * vec4(position,1) );

  v_position = p.xyz;
  v_normal = normal;
  v_text_coord = text_coord;
  v_weight = weight;
  v_jointID = jointID;

  gl_Position = camera_projection*camera_modelview*p;
}
