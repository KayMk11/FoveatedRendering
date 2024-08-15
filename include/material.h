#include <glm/glm.hpp>

#include <string>
#include "shader.h"
#include "texture.h"

class Material
{
public:
    std::string name;
    glm::vec3 ambient;      // Ka
    glm::vec3 diffuse;      // Kd
    glm::vec3 specular;     // Ks
    float shininess;        // Ns
    float refractive_index; // Ni
    float alpha;            // dissolve / d
    int illumination;       // illum

    std::vector<Texture> textures;

    std::string map_ambient_path;
    std::string map_diffuse_path;
    std::string map_specular_path;
    std::string map_shininess_path;
    std::string map_alpha_path;
    std::string map_bump_path;

    Material(){};

    void setup_material()
    {
        if (map_ambient_path != "")
        {
            textures.push_back(Texture(map_alpha_path, "ambient"));
        }
        if (map_diffuse_path != "")
        {
            textures.push_back(Texture(map_diffuse_path, "diffuse"));
        }
        if (map_specular_path != "")
        {
            textures.push_back(Texture(map_specular_path, "specular"));
        }
        if (map_shininess_path != "")
        {
            textures.push_back(Texture(map_shininess_path, "shininess"));
        }
        if (map_alpha_path != "")
        {
            textures.push_back(Texture(map_alpha_path, "alpha"));
        }
        if (map_bump_path != "")
        {
            textures.push_back(Texture(map_bump_path, "bump"));
        }
    }

    void activate_material(Shader &shader)
    {
        shader.use();
        if (textures.empty())
        {
            shader.setVec3(name + ".ambient", ambient);
            shader.setVec3(name + ".diffuse", diffuse);
            shader.setVec3(name + ".specular", specular);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else
        {
            for (int i = 0; i < textures.size(); i++)
            {
                shader.setInt(name + "." + textures[i].type, 0);
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, textures[i].ID);
            }
        }

        shader.setFloat(name + ".shininess", shininess);
        // TODO rest of material;
    }
};