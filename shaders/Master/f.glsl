#version 330 core

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 position;
    vec4 dirLightFpos;      // 平行光视角
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

    float farPlane;
};  

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

uniform Material material;
uniform DirLight dirLight;
uniform int pointLightNumber;
uniform PointLight pointLight[5];
uniform vec3 eyePos;

// 平行光深度贴图
uniform sampler2D dirDepMap;

// 点光源深度贴图
uniform samplerCube pointDepMap;


float ShadowCalculation(vec3 fragPos, vec3 lightPos, float farPlane, samplerCube depthMap) {
     // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // Use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= farPlane;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float bias = 0.1; 
    float shadow = 0.0;
    float samples = 4.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; 
                closestDepth *= farPlane;   // Undo mapping [0;1]
                if(currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);

    return shadow;
}

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
            float pcfDepth = texture(depMap, projCoords.xy + vec2(x, y) * texelSize).x; 
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
    float attenuation = min(1.0, 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)));    
    
    // 合并结果
    vec3 ambient, diffuse, specular;

    for(int i = 0; i < material.diffuseNum; i++) {
        ambient += light.ambient  * vec3(texture(material.textureDiffuse[i], fs_in.texCoords));
        diffuse += light.diffuse  * diff * vec3(texture(material.textureDiffuse[i], fs_in.texCoords));
    }
    
    for(int i = 0; i < material.specularNum; i++) {
        specular += light.specular * spec * vec3(texture(material.textureSpecular[i], fs_in.texCoords));
    }
    
    // 默认材质
    if(material.diffuseNum == 0) {
        ambient += light.ambient  * vec3(0.8, 0.8, 0.8);
        diffuse += light.diffuse  * diff * vec3(0.5, 0.5, 0.5);
    }
    if(material.specularNum == 0) {
        specular = light.specular * spec * vec3(1.0, 1.0, 1.0);
    }
    
    // ambient  *= attenuation;
    // diffuse  *= attenuation;
    // specular *= attenuation;
    float shadow = ShadowCalculation(fs_in.position, light.position, light.farPlane, pointDepMap);
    return ( (1.0 - shadow) * (specular + diffuse));
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

    for(int i = 0; i < material.diffuseNum; i++) {
        ambient  = light.ambient  * vec3(texture(material.textureDiffuse[0], fs_in.texCoords));
        diffuse  = light.diffuse  * diff * vec3(texture(material.textureDiffuse[0], fs_in.texCoords));
    }

    for(int i = 0; i < material.specularNum; i++) {
        specular = light.specular * spec * vec3(texture(material.textureSpecular[0], fs_in.texCoords));
    }
    
    // 默认材质
    if(material.diffuseNum == 0) {
        ambient += light.ambient  * vec3(0.8, 0.8, 0.8);
        diffuse += light.diffuse  * diff * vec3(0.5, 0.5, 0.5);
    }
    if(material.specularNum == 0) {
        specular = light.specular * spec * vec3(1.0, 1.0, 1.0);
    }

    float shadow = ShadowCalculation(fs_in.dirLightFpos, dirDepMap);
    return (ambient + (1.0 - shadow) * (diffuse + specular));     
}



void main() {
    vec3 viewDir = normalize(eyePos - fs_in.position);
    vec3 result = vec3(0);
    
    // for(int i = 0; i < pointLightNumber; i++) {
    //     result += CalcPointLight(pointLight[i], fs_in.normal, fs_in.position, viewDir);
    // }
    
    result += CalcDirLight(dirLight, fs_in.normal, viewDir);

    
    fColor = vec4(result, 1.0);
}

