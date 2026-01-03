#version 330 core

// Input: Standard full-screen quad vertices (-1 to 1 range)
layout (location = 0) in vec3 aPos;

// Uniforms: view and projection matrices
uniform mat4 view;
uniform mat4 projection;

// Outputs to Fragment Shader
out vec3 nearPoint;
out vec3 farPoint;

void main() {
    // 1. Calculate the inverse matrices to unproject screen coordinates
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);

    // 2. Unproject the current vertex to get the ray in world space
    // The Z component is -1.0 for the near plane and 1.0 for the far plane
    vec4 unprojectedNear = viewInv * projInv * vec4(aPos.xy, -1.0, 1.0);
    vec4 unprojectedFar  = viewInv * projInv * vec4(aPos.xy,  1.0, 1.0);

    // 3. Perspective division to get actual world coordinates
    nearPoint = unprojectedNear.xyz / unprojectedNear.w;
    farPoint  = unprojectedFar.xyz / unprojectedFar.w;

    // 4. Output the position as-is (full screen quad)
    gl_Position = vec4(aPos, 1.0);
}
