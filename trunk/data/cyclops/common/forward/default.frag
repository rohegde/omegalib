@fsinclude shadowMap

uniform vec4 unif_Ambient;
uniform float unif_Alpha;

varying vec3 var_Normal;
varying vec3 var_EyeVector;

///////////////////////////////////////////////////////////////////////////////////////////////////
struct SurfaceData
{
	vec4 albedo;
	vec4 emissive;
	float shininess;
	float gloss;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct LitSurfaceData
{
	vec4 luminance;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
LitSurfaceData computeLighting(SurfaceData surf)
{
	LitSurfaceData ld;
	float shadow = computeShadowMap();
	vec3 N = normalize(var_Normal); 

	ld.luminance = vec4(0, 0, 0, 0);

	int i; 
	for (i=0; i< @numLights; ++i) 
	{ 
		// Compute light direction
		vec3 lightDir = vec3(gl_LightSource[i].position.xyz + var_EyeVector);
		
		// Compute lambert term
		vec3 L = normalize(lightDir);
		float lambertTerm = dot(N,L); 
		
		if(i == 0) lambertTerm *= shadow;
		if (lambertTerm > 0.0) 
		{ 
			ld.luminance += surf.albedo * gl_LightSource[i].diffuse * lambertTerm; 
			
			// Compute specular factor
			vec3 E = normalize(var_EyeVector); 
			vec3 R = reflect(-L, N);
			float specular = pow( max(dot(R, E), 0.0), surf.shininess ); 
			
			ld.luminance += gl_LightSource[i].specular * specular * surf.gloss; 
		} 
	} 	

	// Add ambient component from main light.
	ld.luminance += surf.albedo * unif_Ambient;
	
	ld.luminance.a = surf.albedo.a * unif_Alpha;
	// Add emissive surface component to final luminance.
	ld.luminance += surf.emissive;
	
	return ld;
}

// Surface shader main function declaration
SurfaceData getSurfaceData(void);

///////////////////////////////////////////////////////////////////////////////////////////////////
void main (void)
{
	SurfaceData sd = getSurfaceData();
	LitSurfaceData lsd = computeLighting(sd);
    gl_FragColor = lsd.luminance; 
}
