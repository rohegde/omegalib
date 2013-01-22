@surfaceShader 

// The diffuse texture
uniform sampler2D unif_DiffuseMap;
varying vec2 var_TexCoord;
varying float def;

uniform float unif_Shininess;
uniform float unif_Gloss;

//uniform bool unif_Trans;
//uniform float unif_ClipX;

//varying float var_CurrentX;

varying vec3 var_Normal;

///////////////////////////////////////////////////////////////////////////////////////////////////
SurfaceData getSurfaceData(void)
{
	SurfaceData sd;
	
	float mydef;

//	d=def/(3.76*pow(10.0,-5));
//	mydef=def/(8.92658*pow(10.0,-7.0));
	mydef=length(def)/(9.0574*pow(10.0,-5.0));
  	
	sd.emissive.rgba =  vec4(mydef, 1.0-abs(2.0*(0.5-mydef)),1.0-mydef, 1.0); 

	float vx = pow(abs((var_TexCoord.x - 0.5) * 2), unif_Shininess);
	float vy = pow(abs((var_TexCoord.y - 0.5) * 2), unif_Shininess);
	if(unif_Gloss == 0.0) sd.emissive.a = (vx + vy);
	else sd.emissive.a = (vx + vy) * min(unif_Gloss*mydef, 1.0);
	if(sd.emissive.a <= 0) discard;

	sd.albedo.a=0.0;
	
//	if(var_CurrentX > unif_ClipX) discard;
	

	sd.shininess = 0.0;
	sd.gloss = unif_Gloss;
	sd.normal = var_Normal;
	
	return sd;
}
