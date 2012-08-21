///////////////////////////////////////////////////////////////////////////////////////////////////
uniform sampler2D unif_PanoMap;
varying vec3 var_ReflectDir;
varying vec2 var_TexCoord;

///////////////////////////////////////////////////////////////////////////////////////////////////
void main (void)
{
	//vec2 tx = vec2(var_ReflectDir.x / 2.0 + 0.5, var_ReflectDir.y - 2.0 + 0.5);
	//Cubemap
	vec4 color;
	color = texture2D(unif_PanoMap, var_TexCoord);
	color.a = 1.0;
	gl_FragColor = color;
	//gl_FragColor = vec4(1, 0, 0, 1);
}