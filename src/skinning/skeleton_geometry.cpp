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


#include "skeleton_geometry.hpp"

#include "skeleton_parent_id.hpp"
#include "../lib/common/error_handling.hpp"

#include <sstream>
#include <fstream>

namespace cpe
{


skeleton_geometry::skeleton_geometry()
    :data()
{}

int skeleton_geometry::size() const
{
    return data.size();
}
void skeleton_geometry::clear()
{
    data.clear();
}

void skeleton_geometry::push_back(skeleton_joint const& joint)
{
    data.push_back(joint);
}

skeleton_joint const& skeleton_geometry::operator[](int index) const
{
    ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
    ASSERT_CPE(index<int(data.size()) , "Index ("+std::to_string(index)+") must be less than the current data size ("+std::to_string(data.size())+")");

    return data[index];
}
skeleton_joint& skeleton_geometry::operator[](int index)
{
    ASSERT_CPE(index>=0,"Index ("+std::to_string(index)+") must be positive");
    ASSERT_CPE(index<int(data.size()) , "Index ("+std::to_string(index)+") must be less than the current data size ("+std::to_string(data.size())+")");

    return data[index];
}

void skeleton_geometry::save(std::string const& filename,skeleton_parent_id const& parent_id)
{
    ASSERT_CPE(size()==parent_id.size(),"Incorrect size for skeleton");

    std::ofstream fid(filename);
    if(!fid.good())
      throw exception_cpe("Cannot open file "+filename,EXCEPTION_PARAMETERS_CPE);

    int const N_joint = size();
    for(int k=0 ; k<N_joint ; ++k)
    {
        skeleton_joint const& joint = (*this)[k];

        fid << parent_id[k] << " ";

        fid << joint.position.x() << " ";
        fid << joint.position.y() << " ";
        fid << joint.position.z() << " ";

        fid << joint.orientation.x() << " ";
        fid << joint.orientation.y() << " ";
        fid << joint.orientation.z() << " ";
        fid << joint.orientation.w() << std::endl;
    }

    fid.close();
}

void skeleton_geometry::load(std::string const& filename)
{
    clear();
    
    std::string data_dir = DATA_DIR;
    data_dir.append( "/" );
    
    std::ifstream fid( data_dir + filename );
    if(!fid.good())
      throw exception_cpe("Cannot open file "+filename,EXCEPTION_PARAMETERS_CPE);

    std::string buffer;

    //read the whole file
    while(fid.good()==true)
    {
      //read line
      std::getline(fid,buffer);

      if( buffer.size()>0 && buffer[0] != '#')
      {

          skeleton_joint joint;
          int joint_id=0;

          std::stringstream tokens(buffer);

          tokens >> joint_id;

          tokens >> joint.position.x();
          tokens >> joint.position.y();
          tokens >> joint.position.z();

          tokens >> joint.orientation.x();
          tokens >> joint.orientation.y();
          tokens >> joint.orientation.z();
          tokens >> joint.orientation.w();

          joint.orientation = normalized(joint.orientation);

          data.push_back(joint);

      }
    }
    fid.close();
}

std::vector<mat4> skeleton_geometry::to_mat4()
{
  transform_data.clear();

  for( unsigned int i = 0; i<data.size();i++ )
    {
    transform_data.push_back(data[i].to_mat4());
    }

  return transform_data;
}

float const* skeleton_geometry::to_mat4_pointer()
{
  to_mat4();
  return transform_data[0].pointer();
}




std::vector<skeleton_joint>::iterator skeleton_geometry::begin() {return data.begin();}
std::vector<skeleton_joint>::iterator skeleton_geometry::end() {return data.end();}
std::vector<skeleton_joint>::const_iterator skeleton_geometry::begin() const {return data.begin();}
std::vector<skeleton_joint>::const_iterator skeleton_geometry::end() const {return data.end();}
std::vector<skeleton_joint>::const_iterator skeleton_geometry::cbegin() const {return data.cbegin();}
std::vector<skeleton_joint>::const_iterator skeleton_geometry::cend() const {return data.cend();}

std::ostream& operator<<(std::ostream& stream , skeleton_geometry const& skeleton)
{
    for(skeleton_joint const& joint : skeleton)
        stream << joint.position << " ; " <<joint.orientation<<std::endl;
    return stream;
}



skeleton_geometry local_to_global(skeleton_geometry const& sk_local,skeleton_parent_id const& parent_id)
{
    ASSERT_CPE(sk_local.size()==parent_id.size() , "Incorrect skeleton size");

    skeleton_geometry sk_global;


    int const N = sk_local.size();
    if(N<=0)
        return sk_global;

    for (int i = 0; i < N; i++)
    {
    //TO Do: Completez la structure sk_global en exprimant les reperes dans les coordonnees globales.
    // question 9
      skeleton_joint global_joint;
      int p_i = parent_id[i];
      if (p_i == -1)
        global_joint = sk_local[i]; // No parent => this is a global joint
      else
      {
         global_joint.position = sk_global[p_i].orientation * sk_local[i].position + sk_global[p_i].position;
         global_joint.orientation = sk_global[p_i].orientation * sk_local[i].orientation;
      }
      sk_global.push_back(global_joint);
    }

    return sk_global;


}
skeleton_geometry inversed(skeleton_geometry const& skeleton)
{
    skeleton_geometry sk_inversed;

    for(skeleton_joint const& joint : skeleton)
    {
        //TO DO: calculer l'inverse de chaque repere (question 16)
    skeleton_joint inversedJoint;
    inversedJoint.position = -joint.position;
    inversedJoint.orientation = conjugated( joint.orientation );
    sk_inversed.push_back( inversedJoint );
    }

    return sk_inversed;
}

skeleton_geometry multiply(skeleton_geometry const& skeleton_1,skeleton_geometry const& skeleton_2)
{
    ASSERT_CPE(skeleton_1.size()==skeleton_2.size(),"Incorrect size");

    skeleton_geometry sk;
    int const N_joint = skeleton_1.size();
    for(int k=0 ; k<N_joint ; ++k)
    {
        //TO DO: calculer la composition pour chaque joint entre celui du squelette 1 et celui du squelette 2
        // question 17
        skeleton_joint joint;
         joint.position = skeleton_1[k].orientation * skeleton_2[k].position + skeleton_1[k].position;
         joint.orientation = skeleton_1[k].orientation * skeleton_2[k].orientation;

        sk.push_back(joint);

    }

    return sk;
}

std::vector<vec3> extract_bones(skeleton_geometry const& skeleton,skeleton_parent_id const& parent_id)
{
    ASSERT_CPE(skeleton.size()==parent_id.size(),"Incorrect size");

    std::vector<vec3> positions;
    int const N_joint = parent_id.size();
    for(int k=1 ; k<N_joint ; ++k)
    {
        int const parent = parent_id[k];

        //TO DO: completez la structure position avec la position des extermitees des os.
        // question 10
        positions.push_back( skeleton[parent].position );
        positions.push_back( skeleton[k].position );
    }

    return positions;
}

skeleton_geometry interpolated(skeleton_geometry const& skeleton_1,skeleton_geometry const& skeleton_2,float const alpha)
{
    ASSERT_CPE(skeleton_1.size()==skeleton_2.size(),"Incorrect size");

    skeleton_geometry sk;
    int const N_joint = skeleton_1.size();

    for(int k=0 ; k<N_joint ; ++k)
    {

        skeleton_joint const& joint_1 = skeleton_1[k];
        skeleton_joint const& joint_2 = skeleton_2[k];

        //TO DO: completez la methode d'interpolation entre les reperes des joints du squelette 1 et du squelette 2
        // question 21
        skeleton_joint joint;
        joint.position = ( 1- alpha ) * joint_1.position + alpha * joint_2.position;
        joint.orientation = slerp( joint_1.orientation, joint_2.orientation, alpha );

        sk.push_back( joint );
    }
    return sk;
}

}
