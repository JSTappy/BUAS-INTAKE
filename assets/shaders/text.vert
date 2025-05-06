#version 120
attribute vec4 vertex; // x, y, u, v
uniform mat4 projection;
varying vec2 UV;

void main() 
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    UV = vertex.zw;
}