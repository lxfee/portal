#pragma once
#include "Global.h"
#include "Shader.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
    public:
        /*  网格数据  */
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        /*  函数  */
        Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures);
        void Draw(ShaderPtr shader, int amount = 1) const ;
        void setInstanceMatrix(int locate);
    private:
        /*  渲染数据  */
        unsigned int VAO, VBO, EBO;
        /*  函数  */
        virtual void setupMesh();
};

class Model {
    public:
        /*  函数   */
        Model(const char *path) : translation(0), rotation(0), scale(1), buffer(0) {
            loadModel(path);
        }
        Model(vector<Mesh> meshes) : translation(0), rotation(0), scale(1), buffer(0) {
            this->meshes = meshes;
        }
        void Draw(ShaderPtr shader, glm::mat4 model, int amount = 1);  // 注意，如果设置了变换矩阵，函数会先应用模型自身的变换，再应用变换矩阵 
        void Draw(ShaderPtr shader, int amount = 1);   
        void addTexture(Texture tex);
        void setInstanceMatrix(int locate, int amount, glm::mat4 InstanceMatrix[]);
        glm::mat4 getModelMatrix();
        glm::vec3 translation;
        glm::vec3 rotation; // 绕各个轴旋转的角度 
        glm::vec3 scale; 
    private:
        unsigned int buffer; // 实例化矩阵缓存
        vector<Texture> textures_loaded;
        /*  模型数据  */
        vector<Mesh> meshes;
        string directory;
        /*  函数   */
        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName); 
};

typedef shared_ptr<Model> ModelPtr;

