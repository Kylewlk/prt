#version 430 core


layout (std430, binding = 0) buffer SHLight {
    vec3 sh[16];
};

in vec3 LocalPos;

out vec4 FragColor;


const float pi = 3.1415926;

void main()
{
    int shCount = 4;

    float basis[16];
    vec3 dir = normalize(LocalPos);
    float x = dir.x, y = dir.y, z = dir.z;
    int index = 0;
    basis[index++] = 1.0/2.0 * sqrt(1.0 / pi);

    basis[index++] = 1.0/2.0 * sqrt(3.0 / pi) * z;
    basis[index++] = 1.0/2.0 * sqrt(3.0 / pi) * x;
    basis[index++] = 1.0/2.0 * sqrt(3.0 / pi) * y;

    basis[index++] = 1.0/4.0 * sqrt(5.0 / pi) * (2.0*z*z - x*x - y*y);
    basis[index++] = 1.0/2.0 * sqrt(15.0 / pi) * (x*z);
    basis[index++] = 1.0/2.0 * sqrt(15.0 / pi) * (y*z);
    basis[index++] = 1.0/4.0 * sqrt(15.0 / pi) * (x*x - y*y);
    basis[index++] = 1.0/2.0 * sqrt(15.0 / pi) * (x*y);

    basis[index++] = 1.0/4.0 * sqrt(7.0 / pi) * z * (2.0*z*z - 3.0*x*x - 3.0*y*y);
    basis[index++] = 1.0/4.0 * sqrt(10.5 / pi) * x * (5.0*z*z - 1.0);
    basis[index++] = 1.0/4.0 * sqrt(10.5 / pi) * y * (5.0*z*z - 1.0);
    basis[index++] = 1.0/4.0 * sqrt(105 / pi) * z * (x*x - y*y);
    basis[index++] = 1.0/2.0 * sqrt(105 / pi) * x * y * z;
    basis[index++] = 1.0/4.0 * sqrt(17.5 / pi) * x * (x*x - 3.0*y*y);
    basis[index++] = 1.0/4.0 * sqrt(17.5 / pi) * y * (3*x*x - y*y);

    vec3 color = vec3(0,0,0);
    for (int i = 0; i < shCount; i++)
        color += sh[i] * basis[i];

    FragColor = vec4(color, 1.0);
}