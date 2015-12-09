#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 a_cbf_center_pos;
attribute vec3 a_cbf_diff_pos;
attribute vec4 a_cbf_scale_life;
attribute vec4 a_cbf_color;
attribute vec3 a_cbf_speed;
attribute vec3 a_cbf_normal;

varying mediump vec4 v_color;

uniform float u_cbf_time;
const vec3 light_dir = vec3(1.0,0.0,1.0); //与光射向反向

void main()
{
    float timeRadio = smoothstep(a_cbf_scale_life.z,a_cbf_scale_life.w,u_cbf_time);
    float timeStep = 1.0 - step(a_cbf_scale_life.w, u_cbf_time);
    float scale = timeStep* (a_cbf_scale_life.x + timeRadio* (a_cbf_scale_life.y-a_cbf_scale_life.x));
    gl_Position = CC_MVPMatrix * vec4((u_cbf_time-a_cbf_scale_life.z) * a_cbf_speed + a_cbf_center_pos + scale * a_cbf_diff_pos, 1.0); //todo scale speed life


    // 由于face的色彩是一致的，为了减少fsh的压力，全部色彩计算移到vsh进行。
    vec3 lightDir = normalize(light_dir);
    vec3 normal = normalize(CC_NormalMatrix * a_cbf_normal);
    float lambertian = max(dot(lightDir, normal), 0.0);
    vec3 mixedColor = a_cbf_color.rgb;
    v_color = vec4(mixedColor + 0.2*mixedColor*lambertian, a_cbf_color.a);

}