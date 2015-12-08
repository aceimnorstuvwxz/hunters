#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 a_positioin;
attribute vec4 a_color;
attribute vec3 a_cbf_normal;
attribute float a_bone_index;

varying mediump vec4 v_color;

uniform vec4 u_mixcolor;
uniform mat4 u_bone_matrixs[11]; //与 DynamicPixelNode.h 内定义一致

const vec3 light_dir = vec3(1.0,0.0,1.0); //与光射向反向

void main()
{
    gl_Position = CC_MVPMatrix * (u_bone_matrixs[int(a_bone_index)] * a_positioin);

    // 由于face的色彩是一致的，为了减少fsh的压力，全部色彩计算移到vsh进行。
    vec3 lightDir = normalize(light_dir);
    vec3 normal = normalize(CC_NormalMatrix * a_cbf_normal);
    float lambertian = max(dot(lightDir, normal), 0.0);
    vec3 mixedColor = mix(a_color.rgb, u_mixcolor.rgb, u_mixcolor.a);
    v_color = vec4(mixedColor + 0.2*mixedColor*lambertian, a_color.a);

}