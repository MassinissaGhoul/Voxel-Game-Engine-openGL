#version 330 core
#extension GL_ARB_separate_shader_objects : enable
out vec4 FragColor;  
in vec3 ourColor;
  
void main()
{
    FragColor = vec4(ourColor, 1.0);
}

