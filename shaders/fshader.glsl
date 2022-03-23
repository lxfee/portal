#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexture;
out vec4 fColor;

struct Material {
    int diffuseNum;
    int specularNum;
    sampler2D textureDiffuse[4];
    sampler2D textureSpecular[4];
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  


uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 eyePos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    float shininess = 32;
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.textureDiffuse[0], fTexture));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.textureDiffuse[0], fTexture));
    vec3 specular;
    if(material.specularNum == 0) specular = light.specular * spec * vec3(1.0, 1.0, 1.0);
    else specular = light.specular * spec * vec3(texture(material.textureSpecular[0], fTexture));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main() {
    vec3 viewDir = normalize(eyePos - fPosition);
    vec3 result = vec3(0);
    
    result += CalcPointLight(pointLight, fNormal, fPosition, viewDir);
    
    fColor = vec4(result, 1.0);
}

