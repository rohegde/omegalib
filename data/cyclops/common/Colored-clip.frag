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
	
    sd.albedo.rgb =  vec3(0.0, 0.2, 0.2); 
	sd.albedo.r=deform/(9.0574*pow(10.0,-5.0));
	sd.albedo.a = 1.0;
	sd.emissive = vec4(0, 0, 0, 0);
	

	sd.shininess = unif_Shininess;
	sd.gloss = unif_Gloss;
		return sd;
}
