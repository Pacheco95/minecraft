#version 330 core

in vec3 nearPoint;
in vec3 farPoint;

layout(location = 0) out vec4 outColor;

uniform mat4 view;
uniform mat4 projection;

// Grid Configuration
float gridScale = 1.0;        // Size of the small grid cells
float chunkScale = 16.0;      // Size of the chunk cells

// Colors
vec4 colorGrid = vec4(0.5, 0.5, 0.5, 0.5); // Light Gray
vec4 colorChunk = vec4(0.2, 0.2, 0.2, 0.8); // Darker, more opaque

// Function to compute the depth buffer value manually
// This allows the grid to properly interact with other objects in the scene
float computeDepth(vec3 pos) {
    vec4 clip_space_pos = projection * view * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w); // Return NDC z
}

// Logic to calculate grid lines with anti-aliasing
float grid(vec3 worldPos, float scale) {
    vec2 coord = worldPos.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1.0);
    float minimumx = min(derivative.x, 1.0);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

    return 1.0 - min(line, 1.0);
}

void main() {
    // 1. Solves the equation: origin + t * direction = worldPos
    // We are looking for where the ray hits the plane y = 0
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    // 2. If t < 0, the intersection is behind the camera (or looking at sky)
    // If t > 1, the intersection is beyond the far plane (not technically needed for infinite but good for z-buffer)
    // Here we mainly care if we are looking "up" vs "down"
    if (t < 0.0) {
        discard;
    }

    // 3. Reconstruct the actual world position on the ground
    vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

    // 4. Calculate Grid Factors
    // Basic (Unit) Grid
    float linearDepth = computeDepth(worldPos);
    float fading = max(0, (0.5 - linearDepth));

    // Draw the small grid (every 1 unit)
    float g1 = grid(worldPos, 1.0 / gridScale);

    // Draw the chunk grid (every 16 units)
    float g2 = grid(worldPos, 1.0 / chunkScale);

    // 5. Mixing Logic
    // We want the chunk grid to draw on top of the small grid
    // Use the alpha of the grid lines to mix
    vec4 finalColor = mix(vec4(0.0), colorGrid, g1); // Base grid
    finalColor = mix(finalColor, colorChunk, g2);    // Chunk grid over base

    // 6. Fade out the grid at the horizon
    // Adjust the '100.0' divisor to control how far out the grid is visible
    float d = distance(nearPoint, worldPos);
    float alphaFade = 1.0 - smoothstep(0.0, 100.0, d);
    finalColor.a *= alphaFade;

    outColor = finalColor;

    // 7. Write Depth
    // Since we are drawing a quad on the screen, the default depth is the near plane.
    // We must manually write the depth of the calculated world position.
    gl_FragDepth = (computeDepth(worldPos) + 1.0) / 2.0;
}
