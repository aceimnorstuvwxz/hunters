
varying vec2 v_texcoord;

uniform sampler2D u_texture;

uniform float u_cbf_opacity;

void main()
{
    vec4 col = texture2D(u_texture, v_texcoord);
    gl_FragColor = vec4(col.rgb, col.a*u_cbf_opacity);
}