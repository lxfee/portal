#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 position;
} gs_in[];

out vec2 fTexture;
out vec3 fNormal;
out vec3 fPosition;

uniform float time;

vec4 explode(vec4 position, vec3 normal) {
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal() {
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

void main() {
    // vec3 normal = GetNormal();
    
    for(int i = 0; i < 3; i++) {
        // gl_Position = explode(gl_in[i].gl_Position, normal);
        gl_Position = gl_in[i].gl_Position;
        // fPosition = vec3(explode(vec4(gs_in[i].position, 1.0), gs_in[i].normal));
        fPosition = gs_in[i].position;
        fTexture = gs_in[i].texCoords;
        fNormal = gs_in[i].normal;
        EmitVertex();
    }
    
    EndPrimitive();
}