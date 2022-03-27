#version 330 core

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 position;
    vec4 dirLightFpos;      // 平行光视角
    vec4 pointLightFpos;    // 点光源视角
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
uniform sampler2D dirDepMap;
uniform sampler2D pointDepMap;


float ShadowCalculation(vec4 lightFpos, sampler2D depMap) {
    // 执行透视除法
    vec3 projCoords = lightFpos.xyz / lightFpos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    float bias = 0.00005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(depMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

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
    ambient = light.ambient  * vec3(texture(material.textureDiffuse[0], fs_in.texCoords));
    diffuse = light.diffuse  * diff * vec3(texture(material.textureDiffuse[0], fs_in.texCoords));
    if(material.specularNum == 0) specular = light.specular * spec * vec3(1.0, 1.0, 1.0);
    else specular = light.specular * spec * vec3(texture(material.textureSpecular[0], fs_in.texCoords));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    float shadow = ShadowCalculation(fs_in.pointLightFpos, pointDepMap);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
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
    float shadow = ShadowCalculation(fs_in.dirLightFpos, dirDepMap);
    return (ambient + (1.0 - shadow) * (diffuse + specular));     
}



void main() {
    vec3 viewDir = normalize(eyePos - fs_in.position);
    vec3 result = vec3(0);
    
    result += CalcPointLight(pointLight, fs_in.normal, fs_in.position, viewDir);
    result += CalcDirLight(dirLight, fs_in.normal, viewDir);

    // fColor = vec4(result, 1.0);
    fColor = texture(material.textureDiffuse[0], fs_in.texCoords);
}

