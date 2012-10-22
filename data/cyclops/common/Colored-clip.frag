@surfaceShader 

uniform float unif_Shininess;
uniform float unif_Gloss;

//uniform float unif_ClipX;
//uniform bool unif_Trans;

varying float deform;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
	
    sd.albedo.rgb =  vec3(0.4, 0.0, 0.0); 
	sd.albedo.r=deform/8.92658e-07;
	sd.albedo.a = 1.0;
	sd.emissive = vec4(0, 0, 0, 0);
	

	sd.shininess = unif_Shininess;
	sd.gloss = unif_Gloss;
		return sd;
}
