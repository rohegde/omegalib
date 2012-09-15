@surfaceShader 

// The diffuse texture
uniform sampler2D unif_DiffuseMap;
varying vec2 var_TexCoord;

uniform float unif_Shininess;
uniform float unif_Gloss;

uniform bool unif_Trans;
uniform float unif_ClipX;

varying float var_CurrentX;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
	
	sd.emissive.rgb = gl_Color.rgb;	
	float vx = pow(abs((var_TexCoord.x - 0.5) * 2), unif_Shininess);
	float vy = pow(abs((var_TexCoord.y - 0.5) * 2), unif_Shininess);
	sd.emissive.a = (vx + vy);
	if(sd.emissive.a <= 0) discard;

	sd.albedo.a=0.0;
	
	if(var_CurrentX > unif_ClipX) discard;
	

	sd.shininess = 0.0;
	sd.gloss = unif_Gloss;
	
	return sd;
}
