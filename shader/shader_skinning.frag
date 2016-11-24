#version 130

uniform sampler2D texture;
uniform bool debugWeights;
in vec2 v_text_coord;
in vec3 v_position;
in vec3 v_normal;

in vec4 v_weight;
flat in ivec4 v_jointID;

out vec4 color;

vec3 light=vec3(0.5,0.3,5.0);

void main (void)
{
  // Color mesh using weights and joint ids
  if( debugWeights )
    {
    color = vec4(v_weight * v_jointID/10.0 + 0.01*texture2D(texture,v_text_coord));
    return;
    }

  // Color mesh using weights and joint ids
  vec3 normal=normalize(v_normal);

  vec3 vertex_to_light=normalize(light-v_position);
  vec3 reflected_light=reflect(-vertex_to_light,normal);
  vec3 user_to_vertex=normalize(-v_position);

  float diffuse_term=0.8*clamp(abs(dot(normal,vertex_to_light)),0.0,1.0);
  float specular_term=0.2*pow(clamp(dot(reflected_light,user_to_vertex),0.0,1.0),128.0);
  float ambiant_term=0.4;

  vec4 white=vec4(1.0,1.0,1.0,0.0);
  vec4 color_texture=texture2D(texture,v_text_coord);

  color = (ambiant_term+diffuse_term)*color_texture+specular_term*white;
}

