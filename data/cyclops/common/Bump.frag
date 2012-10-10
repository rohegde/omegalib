@tangentSpaceSurfaceShader

// The diffuse texture
uniform sampler2D unif_DiffuseMap;
uniform sampler2D unif_NormalMap;
varying vec2 var_TexCoord;

uniform float unif_Shininess;
uniform float unif_Gloss;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
    sd.albedo = gl_Color * texture2D(unif_DiffuseMap, var_TexCoord);
	sd.emissive = vec4(0, 0, 0, 0);
	sd.shininess = unif_Shininess;
	sd.gloss = unif_Gloss;
	sd.normal = texture2D(unif_NormalMap, var_TexCoord);
	
	return sd;
}
