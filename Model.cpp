#include "Model.h"
#include "Shader.h"
#include "Global.h"

Mesh::Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);   
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
} 

void Mesh::Draw(ShaderPtr shader, int amount) const {
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        string number;
        string tpye = textures[i].type;
        if(tpye == "textureDiffuse") {
            number = std::to_string(diffuseNr++);
            shader->setInt(("material." + tpye + "[" + number + "]").c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        } else if(tpye == "textureSpecular") {
            number = std::to_string(specularNr++);
            shader->setInt(("material." + tpye + "[" + number + "]").c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
    }
    glActiveTexture(GL_TEXTURE0);
    
    shader->setInt("material.diffuseNum", diffuseNr);
    shader->setInt("material.specularNum", specularNr);

    // 绘制网格
    glBindVertexArray(VAO);
    if(amount > 1) {
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, amount);
    } else {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void Model::Draw(ShaderPtr shader, glm::mat4 model, int amount) {
    shader->setMat4("model", model * getModelMatrix()); // 应用变换。
    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader, amount);
    }
}

void Model::Draw(ShaderPtr shader, int amount) {
    Draw(shader, glm::mat4(1.0), amount);
}

void Model::loadModel(string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);    

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // 处理节点所有的网格（如果有的话）
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));         
    }
    // 接下来对它的子节点重复这一过程
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vec;
        // 处理顶点位置、法线和纹理坐标
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z; 
        vertex.Position = vec;

        // cout << vec.x << " " << vec.y << " " << vec.z << endl;
        
        vec.x = mesh->mNormals[i].x;
        vec.y = mesh->mNormals[i].y;
        vec.z = mesh->mNormals[i].z;
        vertex.Normal = vec;
        
        if(mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // 处理索引
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }


    // 处理材质
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "textureDiffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "textureSpecular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].name.c_str(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip) {   // 如果纹理还没有被加载，则加载它
            Texture texture = Texture::TextureFromFile(directory + "/" + str.C_Str(), str.C_Str(), typeName);
            textures.push_back(texture);
            textures_loaded.push_back(texture); // 添加到已加载的纹理中
        }
    }
    return textures;
}

glm::mat4 Model::getModelMatrix() {
    glm::mat4 trans(1.0f);
    trans = glm::translate(trans, translation);
    
    trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
    trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
    trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

    trans = glm::scale(trans, scale);
    
    return trans;
}

// locate: 代表矩阵在着色器中的起点。顶点属性最大允许的数据大小等于一个vec4。因为一个mat4本质上是4个vec4，需要为这个矩阵预留4个顶点属性。
// 这意味着下一个可用位置是 locate + 4
void Model::setInstanceMatrix(int locate, int amount, glm::mat4 InstanceMatrix[]) {
    // 顶点缓冲对象
    if(!buffer) glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &InstanceMatrix[0], GL_STATIC_DRAW);

    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].setInstanceMatrix(locate);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::addTexture(Texture tex) {
    for(unsigned int j = 0; j < textures_loaded.size(); j++) {
        if(std::strcmp(textures_loaded[j].name.c_str(), tex.name.c_str()) == 0) {
            return ;
        }
    }
    textures_loaded.push_back(tex);
    for(auto& mesh : meshes) {
        mesh.textures.push_back(tex);
    }
}


void Mesh::setInstanceMatrix(int locate) {
    glBindVertexArray(VAO);
    // 顶点属性
    GLsizei vec4Size = sizeof(glm::vec4);
    for(int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(locate + i); 
        glVertexAttribPointer(locate + i, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(i * vec4Size));
        glVertexAttribDivisor(locate + i, 1);
    }
    glBindVertexArray(0);
}


