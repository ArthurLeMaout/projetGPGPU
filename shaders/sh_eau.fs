#version 330 core

in vec2 uv;
in vec3 pos;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec3 pos_cam;

void main()
{
    vec3 L = vec3(50,-50,0);
    vec4 n4 = texture( textureSampler, uv );
    vec3 n = vec3(n4.x, n4.y, n4.z) - vec3(0.5,0.5,0.5);
    vec3 c0 = vec3(0,0,1.0);
    float Ka =0.6;
    float Kd =0.6;
    float Ks =0.4;
    float e_s=160;
    float I_d = Kd* dot (n, normalize(L));
    float I_s = Ks* pow(dot (normalize(reflect(L,n)),normalize(pos_cam-pos)), e_s);

    vec3 c_fin = (Ka+I_d)*c0 + I_s*vec3(1,1,1);
    color = vec4(c_fin.x, c_fin.y, c_fin.z, 1.0);

};
