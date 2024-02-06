uniform sampler2D texture;
uniform vec2 chunk_size;
uniform vec2 texture_size;
uniform vec2 output_size;
uniform float threshold;

void main() {
    for (float i = 0.0; i < chunk_size.x; i += 1.0) {
        for (float j = 0.0; j < chunk_size.y; j += 1.0) {
            vec2 neighbor = gl_TexCoord[0].xy - vec2(0.5, 0.5) / output_size 
                + vec2(i / texture_size.x, j / texture_size.y);
            clamp(neighbor.x, 0.0, 1.0);
            clamp(neighbor.y, 0.0, 1.0);
            if (texture2D(texture, neighbor).a > threshold) {
                gl_FragColor = vec4(0, 1, 0, 1);
                return;
            }
        }
    }
    gl_FragColor = vec4(0, 0, 0, 0);
}