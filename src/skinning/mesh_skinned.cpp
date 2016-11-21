/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "mesh_skinned.hpp"

#include "../lib/common/error_handling.hpp"
#include "../lib/mesh/mesh_io.hpp"
#include "skeleton_geometry.hpp"

#include "../lib/opengl/glutils.hpp"
#include "../lib/common/error_handling.hpp"

#include <sstream>
#include <fstream>


namespace cpe
{

int mesh_skinned::size_vertex_weight() const
{
    return weight_data.size()/WEIGHTS_PER_VERTEX;
}

float const* mesh_skinned::pointer_weight() const
{
  return &weight_data[0];
}

int const* mesh_skinned::pointer_jointID() const
{
  return &joint_id[0];
}

void mesh_skinned::draw() const
{
    // Create temporary VBO to store data
    GLuint vbos[5], vboi, vao;                                                                             PRINT_OPENGL_ERROR();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(5, vbos);                                                                                 PRINT_OPENGL_ERROR();

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);                                                                PRINT_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER , sizeof(vec3)*size_vertex() , this->pointer_vertex() , GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();
    glEnableVertexAttribArray(0);                                                                          PRINT_OPENGL_ERROR();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);                                                 PRINT_OPENGL_ERROR();

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);                                                                PRINT_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER , sizeof(vec4)*size_normal() , this->pointer_normal() , GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();
    glEnableVertexAttribArray(1);                                                                          PRINT_OPENGL_ERROR();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);                                                 PRINT_OPENGL_ERROR();

    // Texture coords
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);                                                                              PRINT_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER , sizeof(vec4)*size_texture_coord() , this->pointer_texture_coord() , GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();
    glEnableVertexAttribArray(2);                                                                                        PRINT_OPENGL_ERROR();
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Weights
    glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);                                                                PRINT_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER , sizeof(vec4)*size_vertex() , this->pointer_weight() , GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();
    glEnableVertexAttribArray(3);                                                                          PRINT_OPENGL_ERROR();
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);                                                 PRINT_OPENGL_ERROR();

    // Joint IDs
    glBindBuffer(GL_ARRAY_BUFFER, vbos[4]);                                                           PRINT_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER , sizeof(vec4)*size_vertex() , pointer_jointID() , GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();
    glEnableVertexAttribArray(4);                                                                     PRINT_OPENGL_ERROR();
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &vboi);                                                                                          PRINT_OPENGL_ERROR();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboi);                                                                     PRINT_OPENGL_ERROR();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(int)*size_connectivity(),pointer_triangle_index(),GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    glDrawElements(GL_TRIANGLES, 3*size_connectivity(), GL_UNSIGNED_INT, 0);  PRINT_OPENGL_ERROR();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glDeleteBuffers(5,vbos);  PRINT_OPENGL_ERROR();

}

void mesh_skinned::add_vertex_weight(float const& w)
{
    weight_data.push_back(w);
}

void mesh_skinned::add_jointID(int const& j)
{
    joint_id.push_back(j);
}

void mesh_skinned::load(std::string const& filename)
{
    //Warning: Can only handle meshes with same connectivity for vertex and textures
    //(Format de fichier de David Odin)

    std::string data_dir = DATA_DIR;
    data_dir.append( "/" );
    
    std::ifstream fid( (data_dir + filename).c_str() );
    if(!fid.good())
        throw exception_cpe("Cannot open file "+filename,MACRO_EXCEPTION_PARAMETER);

    std::string buffer;

    //read the whole file
    while(fid.good()==true)
    {
        //read line
        std::getline(fid,buffer);

        if(buffer.size()>0)
        {
            std::stringstream tokens(buffer);
            std::string first_word;
            tokens >> first_word;

            //skip comments
            if(first_word.length()>0 && first_word[0]!='#')
            {
                //vertices
                if(first_word=="v")
                {
                  vec3 vertex;
                  tokens >> vertex.x();
                  tokens >> vertex.y();
                  tokens >> vertex.z();

                  add_vertex(vertex);
                }

                //texture
                if(first_word=="vt")
                {
                  vec2 texture;
                  tokens >> texture.x();
                  tokens >> texture.y();

                  add_texture_coord(texture);
                }

                // normal
                if(first_word=="vn")
                {
                  vec3 normal;
                  tokens >> normal.x();
                  tokens >> normal.y();
                  tokens >> normal.z();

                  add_normal(normal);
                }

                //skinning
                if(first_word=="sk")
                {
                    int tmpJointID;
                    float tmpWeight;
                    int const N_bone = WEIGHTS_PER_VERTEX;
                    for(int k_bone=0 ; k_bone<N_bone ; ++k_bone)
                    {
                        tokens >> tmpJointID;
                        tokens >> tmpWeight;
                        weight_data.push_back(tmpWeight);
                        joint_id.push_back(tmpJointID);
                    }
                }


                //read connectivity
                if(first_word=="f")
                {
                  std::string u0_str,u1_str,u2_str;
                  tokens >> u0_str >> u1_str >> u2_str;

                  int const u0 = std::stoi(u0_str)-1;
                  int const u1 = std::stoi(u1_str)-1;
                  int const u2 = std::stoi(u2_str)-1;

                  add_triangle_index({u0,u1,u2});

                }
            }

        }

    }

    fid.close();

    ASSERT_CPE(size_vertex_weight()==size_vertex(),"Mesh skinned seems to have the wrong number of skinning weights");

}

void mesh_skinned::add_vertex(vec3 const& p)
{
  mesh::add_vertex(p);
}

}
