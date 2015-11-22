#ifdef GL_ES
precision mediump float;
#endif

// Inputs
attribute vec4 a_positioin;
attribute vec3 a_color;
attribute vec3 a_normal2;

// Varyings
#ifdef GL_ES
varying mediump vec3 v_color;
#endif

uniform float u_move_line;
uniform float u_move_width;
uniform float u_x_diff;

const vec3 light_dir = vec3(1.0,0.0,1.0); //与光射向反向

void main()
{

    float nn = floor(u_move_line/u_move_width);
    float x  = a_positioin.x + u_x_diff*a_positioin.y*a_positioin.y; //X diff here
    float z = a_positioin.z;
    float fxnd = fract(x/u_move_width);
    float fxnd2 = fract(u_move_line/u_move_width);
    if (abs(fxnd2-fxnd) < 0.01) {
        z = -1000000.0;
    }
    if (fxnd < fxnd2) {
        x = (nn+1.0+fxnd)*u_move_width;
    } else {
        x = (nn+fxnd)*u_move_width;
    }

    gl_Position = CC_MVPMatrix * vec4(x,a_positioin.y,z,a_positioin.w);


    // 光照计算从fsh移到了vsh
    vec3 lightDir = normalize(light_dir);
    vec3 normal = normalize(CC_NormalMatrix * a_normal2);
    float lambertian = max(dot(lightDir, normal), 0.0);

    v_color = a_color + 0.2*a_color*lambertian;
}