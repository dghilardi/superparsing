#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform vec4 my_color;

varying vec2 v_texcoord;

//! [0]
void main()
{
    // Set fragment color from texture
    gl_FragColor = my_color;//vec4(0.2, 0.4, 0.3, 1.0); //texture2D(texture, v_texcoord);
}
//! [0]

