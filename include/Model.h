#pragma once
#include "Global.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
    aiString path;  // 我们储存纹理的路径用于与其它纹理进行比较
};


class Mesh {
    public:
        /*  网格数据  */
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        /*  函数  */
        Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures);
        void Draw(Shader* shader);
    private:
        /*  渲染数据  */
        unsigned int VAO, VBO, EBO;
        /*  函数  */
        virtual void setupMesh();
};

class Model {
    public:
        /*  函数   */
        Model(char *path) : translation(0), rotation(0), scale(1) {
            loadModel(path);
        }
        Model(vector<Mesh> meshes) : translation(0), rotation(0), scale(1) {
            this->meshes = meshes;
        }
        void Draw(Shader* shader);   
        static unsigned int TextureFromFile(string path, string directory); 
        glm::mat4 getModelMatrix();
        glm::vec3 translation;
        glm::vec3 rotation; // 绕各个轴旋转的角度 
        glm::vec3 scale; 

    private:
        vector<Texture> textures_loaded;
        /*  模型数据  */
        vector<Mesh> meshes;
        string directory;
        /*  函数   */
        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             string typeName); 
};


