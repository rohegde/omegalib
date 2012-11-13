@fsinclude shadowMap

uniform vec4 unif_Ambient;
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
LitSurfaceData computeLighting(SurfaceData surf)
{
	LitSurfaceData ld;
	float shadow = computeShadowMap();
	
	vec3 N = surf.normal;
	
	// If we are rendering a back-facing fragment, invert the normal by default
	if(!gl_FrontFacing) N = -N;

	ld.luminance = vec4(0, 0, 0, 0);

	@tangentSpaceFragmentLightSection

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
