#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out vec3 LocalPos;

uniform mat4 viewProj;
uniform mat4 model;
uniform mat3 normalMatrix;
uniform ivec2 lm;


const float pi = 3.1415926;

float Y00(vec3 dir)
{
    return 1.0/2.0 * sqrt(1.0 / pi);
}

float Y10(vec3 dir)
{
    return 1.0/2.0 * sqrt(3.0 / pi) * dir.z;
}

float Y11(vec3 dir)
{
    return 1.0/2.0 * sqrt(3.0 / pi) * dir.x;
}

float Y1_1(vec3 dir)
{
    return 1.0/2.0 * sqrt(3.0 / pi) * dir.y;
}

float Y20(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/4.0 * sqrt(5.0 / pi) * (2.0*z*z - x*x - y*y);
}

float Y21(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/2.0 * sqrt(15.0 / pi) * (x*z);
}

float Y2_1(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/2.0 * sqrt(15.0 / pi) * (y*z);
}

float Y22(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/4.0 * sqrt(15.0 / pi) * (x*x - y*y);
}

float Y2_2(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/2.0 * sqrt(15.0 / pi) * (x*y);
}

float Y30(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/4.0 * sqrt(7.0 / pi) * z * (2.0*z*z - 3.0*x*x - 3.0*y*y);
}

float Y31(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/4.0 * sqrt(10.5 / pi) * x * (5.0*z*z - 1.0);
}

float Y3_1(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/4.0 * sqrt(10.5 / pi) * y * (5.0*z*z - 1.0);
}

float Y32(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/4.0 * sqrt(105 / pi) * z * (x*x - y*y);
}

float Y3_2(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/2.0 * sqrt(105 / pi) * x * y * z;
}

float Y33(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/4.0 * sqrt(17.5 / pi) * x * (x*x - 3.0*y*y);
}

float Y3_3(vec3 dir)
{
    float x = dir.x, y = dir.y, z = dir.z;
    return 1.0/4.0 * sqrt(17.5 / pi) * y * (3*x*x - y*y);
}


void main()
{
//    vec3 pos = vec3(-aPos.z, -aPos.x, aPos.y);
    vec3 pos = vec3(aPos.x, aPos.y, aPos.z);

    float c = 0.0;
    if (lm.x == 0 && lm.y == 0)
    {
        c = Y00(normalize(pos));
    }
    else if (lm.x == 1 && lm.y == 0)
    {
        c = Y10(normalize(pos));
    }
    else if (lm.x == 1 && lm.y == 1)
    {
        c = Y11(normalize(pos));
    }
    else if (lm.x == 1 && lm.y == -1)
    {
        c = Y1_1(normalize(pos));
    }
    else if (lm.x == 2 && lm.y == 0)
    {
        c = Y20(normalize(pos));
    }
    else if (lm.x == 2 && lm.y == 1)
    {
        c = Y21(normalize(pos));
    }
    else if (lm.x == 2 && lm.y == 2)
    {
        c = Y22(normalize(pos));
    }
    else if (lm.x == 2 && lm.y == -1)
    {
        c = Y2_1(normalize(pos));
    }
    else if (lm.x == 2 && lm.y == -2)
    {
        c = Y2_2(normalize(pos));
    }
    else if (lm.x == 3 && lm.y == 0)
    {
        c = Y30(normalize(pos));
    }
    else if (lm.x == 3 && lm.y == 1)
    {
        c = Y31(normalize(pos));
    }
    else if (lm.x == 3 && lm.y == 2)
    {
        c = Y32(normalize(pos));
    }
    else if (lm.x == 3 && lm.y == 3)
    {
        c = Y33(normalize(pos));
    }
    else if (lm.x == 3 && lm.y == -1)
    {
        c = Y3_1(normalize(pos));
    }
    else if (lm.x == 3 && lm.y == -2)
    {
        c = Y3_2(normalize(pos));
    }
    else if (lm.x == 3 && lm.y == -3)
    {
        c = Y3_3(normalize(pos));
    }

    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos*abs(c*2.0), 1.0));
    Normal = normalize(normalMatrix*aNormal);
    LocalPos = pos;

    gl_Position =  viewProj * vec4(WorldPos, 1.0);
}