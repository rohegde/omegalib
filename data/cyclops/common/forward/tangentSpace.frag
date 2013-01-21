@fsinclude shadowMap

uniform float unif_Alpha;

varying vec3 var_EyeVector;
varying vec3 var_LightVector[@numLights]; 
varying vec3 var_LightHalfVector[@numLights]; 

///////////////////////////////////////////////////////////////////////////////////////////////////
struct SurfaceData
{
	vec4 albedo;
	vec4 emissive;
	vec3 normal;
	float shininess;
	float gloss;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct LitSurfaceData
{
	vec4 luminance;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
$@fragmentLightSection
{
	// Compute light direction
	vec3 lightDir = normalize(var_LightVector[@lightIndex]);
	vec3 halfVec = normalize(var_LightHalfVector[@lightIndex]);

	float lambertTerm = dot(lightDir, N); 

	if(@lightIndex == 0) lambertTerm *= shadow;
	if (lambertTerm > 0.0) 
	{ 
		ld.luminance += surf.albedo * gl_LightSource[@lightIndex].diffuse * lambertTerm; 
		float specular = pow( max(dot(halfVec, N), 0.0), surf.shininess ); 
		ld.luminance += gl_LightSource[@lightIndex].diffuse * specular * surf.gloss; 
	}
	ld.luminance += surf.albedo * gl_LightSource[@lightIndex].ambient;
} 
$

///////////////////////////////////////////////////////////////////////////////////////////////////
LitSurfaceData computeLighting(SurfaceData surf)
{
	LitSurfaceData ld;
	float shadow = computeShadowMap();
	
	vec3 N = surf.normal;
	
	// If we are rendering a back-facing fragment, invert the normal by default
	if(!gl_FrontFacing) N = -N;

	ld.luminance = vec4(0, 0, 0, 0);

	@fragmentLightSection

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
