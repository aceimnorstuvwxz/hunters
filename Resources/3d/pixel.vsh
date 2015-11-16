#ifdef GL_ES
precision mediump float;
#endif

// Inputs
attribute vec4 a_positioin;
attribute vec4 a_color;
attribute vec3 a_normal2;
attribute vec2 a_posdiff;
attribute float a_seed;

// Varyings
#ifdef GL_ES
varying mediump vec4 v_color;
varying mediump float v_seed;
#endif

uniform float u_posexpand;

uniform vec4 u_mixcolor;

uniform float u_cbf_opacity;



const vec3 light_dir = vec3(1.0,0.0,1.0); //与光射向反向

void main()
{

    if (a_seed > u_cbf_opacity) {
        gl_Position = vec4(0,0,100,0);
    } else {
        gl_Position = CC_MVPMatrix * (a_positioin + vec4(u_posexpand*a_seed*a_posdiff,0.0,0.0));

        // 由于face的色彩是一致的，为了减少fsh的压力，全部色彩计算移到vsh进行。
        vec3 lightDir = normalize(light_dir);
        vec3 normal = normalize(CC_NormalMatrix * a_normal2);
        float lambertian = max(dot(lightDir, normal), 0.0);
        vec3 mixedColor = mix(a_color.rgb, u_mixcolor.rgb, u_mixcolor.a);
        v_color = vec4(mixedColor + 0.2*mixedColor*lambertian, a_color.a);
    }
}