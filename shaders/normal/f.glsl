#version 330 core

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 position;
    vec4 dirLightFpos;
} fs_in;

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
uniform sampler2D shadowMap;

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
    float attenuation = min(1.0, 1.0 / (0.00005 + light.constant + light.linear * distance + light.quadratic * (distance * distance)));    
    
    // 合并结果
    vec3 ambient, diffuse, specular;
    ambient  = light.ambient  * vec3(texture(material.textureDiffuse[0], fs_in.texCoords));
    diffuse  = light.diffuse  * diff * vec3(texture(material.textureDiffuse[0], fs_in.texCoords));
    if(material.specularNum == 0) specular = light.specular * spec * vec3(1.0, 1.0, 1.0);
    else specular = light.specular * spec * vec3(texture(material.textureSpecular[0], fs_in.texCoords));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    float shininess = 100;
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // 合并结果
    vec3 ambient, diffuse, specular;
    ambient  = light.ambient  * vec3(texture(material.textureDiffuse[0], fs_in.texCoords));
    diffuse  = light.diffuse  * diff * vec3(texture(material.textureDiffuse[0], fs_in.texCoords));
    if(material.specularNum == 0) specular = light.specular * spec * vec3(1.0, 1.0, 1.0);
    else specular = light.specular * spec * vec3(texture(material.textureSpecular[0], fs_in.texCoords));
    return (ambient + diffuse + specular);     
}
float ShadowCalculation(vec4 dirLightFpos) {
    // 执行透视除法
    vec3 projCoords = dirLightFpos.xyz / dirLightFpos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    if(currentDepth > closestDepth) shadow = 1.0;
    return shadow;
}


void main() {
    vec3 viewDir = normalize(eyePos - fs_in.position);
    vec3 result = vec3(0);
    
    // result += CalcPointLight(pointLight, fs_in.normal, fs_in.position, viewDir);
    result += CalcDirLight(dirLight, fs_in.normal, viewDir);
    
    float shadow = ShadowCalculation(fs_in.dirLightFpos);

    // fColor = shadow * vec4(result, 1.0);
    fColor = texture(shadowMap, fs_in.texCoords);
}

