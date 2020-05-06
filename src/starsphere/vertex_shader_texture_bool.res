#version 330

// This is a vertex shader. Texture coordinates pass through.
// A boolean is set depending on angle b/w sun and position
// vector to a point.

in vec3 position;
in vec2 aTexCoord;

out vec2 TexCoord;
out float day;

uniform mat4 CameraMatrix;
uniform vec3 toSun;

void main() {
    // Assume night time at this vertex.
    day = 0.0;

    // Check for angle b/w position and the
    // direction to the Sun. If positive dot
    // product then they lie on the same side
    // of the origin ie. the vertex is in
    // "daylight'.
    day = dot(normalize(position), normalize(toSun));
    //if(dot(position, toSun) > 0.0) {
    //    day = 1.0;
    //    }

    // Transform to camera/eye coordinates.
    gl_Position = CameraMatrix * vec4(position, 1.0f);
    TexCoord = aTexCoord;
    }
