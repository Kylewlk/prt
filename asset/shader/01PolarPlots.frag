#version 430 core

uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 cameraPos;

uniform vec3 positive;
uniform vec3 negative;
uniform ivec2 lm;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec3 LocalPos;

out vec4 fragColor;

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
//    fragColor = vec4(0.0, 1.0, 1.0, 1.0);

    vec3 N = normalize(Normal);
    vec3 L = lightDir;
    vec3 V = normalize(cameraPos - WorldPos);
    vec3 H = normalize(L + V);

    float ambient = 0.1;

    float NdotL = dot(N, L);
    float diffuse = (NdotL + 1) * 0.4 + 0.2;

    float NdotH = max(dot(N, H), 0);
    float specular = pow(NdotH, 20) * 0.1;

    float c = 0.0;
    if (lm.x == 0 && lm.y == 0)
    {
        c = Y00(normalize(LocalPos));
    }
    else if (lm.x == 1 && lm.y == 0)
    {
        c = Y10(normalize(LocalPos));
    }
    else if (lm.x == 1 && lm.y == 1)
    {
        c = Y11(normalize(LocalPos));
    }
    else if (lm.x == 1 && lm.y == -1)
    {
        c = Y1_1(normalize(LocalPos));
    }
    else if (lm.x == 2 && lm.y == 0)
    {
        c = Y20(normalize(LocalPos));
    }
    else if (lm.x == 2 && lm.y == 1)
    {
        c = Y21(normalize(LocalPos));
    }
    else if (lm.x == 2 && lm.y == 2)
    {
        c = Y22(normalize(LocalPos));
    }
    else if (lm.x == 2 && lm.y == -1)
    {
        c = Y2_1(normalize(LocalPos));
    }
    else if (lm.x == 2 && lm.y == -2)
    {
        c = Y2_2(normalize(LocalPos));
    }
    else if (lm.x == 3 && lm.y == 0)
    {
        c = Y30(normalize(LocalPos));
    }
    else if (lm.x == 3 && lm.y == 1)
    {
        c = Y31(normalize(LocalPos));
    }
    else if (lm.x == 3 && lm.y == 2)
    {
        c = Y32(normalize(LocalPos));
    }
    else if (lm.x == 3 && lm.y == 3)
    {
        c = Y33(normalize(LocalPos));
    }
    else if (lm.x == 3 && lm.y == -1)
    {
        c = Y3_1(normalize(LocalPos));
    }
    else if (lm.x == 3 && lm.y == -2)
    {
        c = Y3_2(normalize(LocalPos));
    }
    else if (lm.x == 3 && lm.y == -3)
    {
        c = Y3_3(normalize(LocalPos));
    }

    vec3 albedo = mix(negative, positive, step(0, c));
    albedo *= abs(c) * (lm.x * 0.2 + 3.0);



    vec3 color = ((ambient + diffuse) * albedo + specular) * lightColor;

    fragColor = vec4(color, 1.0);
}