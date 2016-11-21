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


#pragma once

#ifndef MESH_SKINNED_HPP
#define MESH_SKINNED_HPP

#include "GL/glew.h"

#include "../lib/3d/quaternion.hpp"
#include "../lib/mesh/mesh.hpp"

#define WEIGHTS_PER_VERTEX 4

namespace cpe
{

class skeleton_geometry;

/** A derived class of mesh with skinning weight information per vertex
    Note that the class store twice the vertices:
     - Once to store the deformed vertices after appliccation of the skinning (access with standard .vertex(index)).
     - Another time to always store the original position of the vertices to enable several application of the skinning.
*/
class mesh_skinned : public mesh
{
public:

    using mesh::mesh;

    /** Pointer access to the weights*/
    float const* pointer_weight() const;
    /** Pointer access to the joint IDs*/
    int const* pointer_jointID() const;

    /** Draw skinned mesh */
    void draw() const;

    /** Add a vertex both as an original position and in the default vertex storage
        \note overloading of the add_vertex method of mesh
    */
    void add_vertex(vec3 const& p);

    /** Add skinning weights to the data structure (in the same order than the vertices) */
    void add_vertex_weight(float const& w);

    /** Add joint ID to the data structure (in the same order than the vertices) */
    void add_jointID(int const& j);

    /** Size of the vertex weights information (should be equals to size_vertex() when all the informations are provided) */
    int size_vertex_weight() const;

    /** Load a mesh with its skinning information from a given file
     * \note Only handle custom 'obj' file with same connectivity for vertex, normals, texture, and skinning weights.
    */
    void load(std::string const& filename);

private:
    /** Internal storage for the vertex weight information*/
    std::vector<float> weight_data;
    /** Internal storage for the vertex jointID information*/
    std::vector<int> joint_id;




};



}

#endif
