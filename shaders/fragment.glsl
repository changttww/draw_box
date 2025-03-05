#version 330 core

// 从顶点着色器接收的颜色插值
in vec3 OurColor;
in vec2 TexCoord;

// 片段着色器的输出颜色
out vec4 color;

uniform sampler2D ourTexture;

void main() {
    // 将插值后的颜色作为片段的最终颜色，alpha 设为 1.0 (不透明)
    //color = texture(ourTexture, TexCoord) * vec4(OurColor, 1.0);
    vec4 texColor = texture(ourTexture, TexCoord);  // 采样纹理颜色
    color = mix(texColor, vec4(OurColor, 1.0), 0.5); // 颜色和纹理混合
}
