#version 330

in float rand;

layout(location = 0) out vec4 out_color;
 
void main()
{
    out_color = vec4(0.2 + (rand / 1.8), 0.2, 0.3, 0);
}
