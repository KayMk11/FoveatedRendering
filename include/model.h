#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include "OBJ_Loader.h"

class Model
{
public:
    std::string name;
    std::vector<Mesh> meshes;
    std::string directory;

    Model(std::string const &path)
    {
        loadModel(path);
    }

    void draw(Shader &shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw(shader);
    }

private:
    void loadModel(std::string const &path)
    {
        objl::Loader loader;
        if (!loader.LoadFile(path))
            std::cout << "unable to load model" << std::endl;
        for (auto objmesh : loader.LoadedMeshes)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            Material material;
            for (int i = 0; i < objmesh.Vertices.size(); i++)
            {
                glm::vec3 position;
                glm::vec3 normal;
                glm::vec2 tex_coord;

                position.x = objmesh.Vertices[i].Position.X;
                position.y = objmesh.Vertices[i].Position.Y;
                position.z = objmesh.Vertices[i].Position.Z;

                normal.x = objmesh.Vertices[i].Normal.X;
                normal.y = objmesh.Vertices[i].Normal.Y;
                normal.z = objmesh.Vertices[i].Normal.Z;

                tex_coord.s = objmesh.Vertices[i].TextureCoordinate.X;
                tex_coord.t = objmesh.Vertices[i].TextureCoordinate.Y;
                vertices.push_back({position, normal, tex_coord});
            }
            for (int i = 0; i < objmesh.Indices.size(); i++)
            {
                indices.push_back(objmesh.Indices[i]);
            }

            material.ambient.r = objmesh.MeshMaterial.Ka.X;
            material.ambient.g = objmesh.MeshMaterial.Ka.Y;
            material.ambient.b = objmesh.MeshMaterial.Ka.Z;

            material.diffuse.r = objmesh.MeshMaterial.Kd.X;
            material.diffuse.g = objmesh.MeshMaterial.Kd.Y;
            material.diffuse.b = objmesh.MeshMaterial.Kd.Z;

            material.specular.r = objmesh.MeshMaterial.Ks.X;
            material.specular.b = objmesh.MeshMaterial.Ks.Y;
            material.specular.g = objmesh.MeshMaterial.Ks.Z;

            material.shininess = objmesh.MeshMaterial.Ns;

            material.refractive_index = objmesh.MeshMaterial.Ni;

            material.alpha = objmesh.MeshMaterial.d;

            material.illumination = objmesh.MeshMaterial.illum;

            material.map_ambient_path = objmesh.MeshMaterial.map_Ka;
            material.map_diffuse_path = objmesh.MeshMaterial.map_Kd;
            material.map_specular_path = objmesh.MeshMaterial.map_Ks;
            material.map_shininess_path = objmesh.MeshMaterial.map_Ns;
            material.map_alpha_path = objmesh.MeshMaterial.map_d;
            material.map_bump_path = objmesh.MeshMaterial.map_bump;

            material.setup_material();

            Mesh mesh(vertices, indices, material);
            meshes.push_back(mesh);
        }
    }
};