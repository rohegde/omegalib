///////////////////////////////////////////////////////////////////////////////////////////////////
uniform int unif_Eye;

uniform sampler2D unif_PanoMapL;
uniform sampler2D unif_PanoMapR;

varying vec3 var_ReflectDir;
varying vec2 var_TexCoord;

///////////////////////////////////////////////////////////////////////////////////////////////////
void main (void)
{
	vec4 color;
	if(unif_Eye == 0) color = texture2D(unif_PanoMapR, var_TexCoord);
	else color = texture2D(unif_PanoMapL, var_TexCoord);
	color.a = 1.0;
	gl_FragColor = color;
}