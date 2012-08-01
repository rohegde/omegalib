@surfaceShader 
@fsinclude envMap

uniform float unif_Shininess;
uniform float unif_Gloss;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
    sd.albedo = gl_Color; 
	sd.shininess = unif_Shininess;
	sd.gloss = unif_Gloss;
	
	// Modulate environment map contribution by material glossyness and color
	sd.emissive = getEnvMapColor()	* gl_Color * unif_Gloss;
	
	return sd;
}
