#ifdef GL_ES
precision mediump float;
#endif

// Inputs
attribute vec4 a_positioin;
attribute vec2 a_texcoord;

// Varyings
#ifdef GL_ES
varying mediump vec2 v_texcoord;
#else
varying vec2 v_texCoord;
#endif

uniform vec2 u_size;

void main()
{
    gl_Position = CC_MVPMatrix * vec4(a_positioin.x * u_size.x, a_positioin.y * u_size.y, a_positioin.z, a_positioin.w);

    v_texcoord = a_texcoord;
}