#version 120
uniform sampler2D text;
uniform vec4 textColor;
varying vec2 UV;

void main() 
{
    float alpha = texture2D(text, UV).r;
    gl_FragColor = vec4(textColor.rgb, textColor.a * alpha);
}