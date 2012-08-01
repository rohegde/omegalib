@surfaceShader 

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
    sd.albedo = vec4(0, 0, 0, 0); 
	sd.emissive = gl_Color;
	sd.shininess = 0;
	sd.gloss = 0;
	
	return sd;
}
