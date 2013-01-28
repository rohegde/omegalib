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
struct LightData
{
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec3 dir;
	vec3 halfDir;

	float shadow;
	float distance;
	vec3 attenuation;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
struct LitSurfaceData
{
	vec4 luminance;
};

@fsinclude lightFunctions

///////////////////////////////////////////////////////////////////////////////////////////////////
$@fragmentLightSection
{ 
	LightData ld;
	
	ld.diffuse = gl_LightSource[@lightIndex].diffuse;
	ld.specular = gl_LightSource[@lightIndex].specular;
	ld.ambient = gl_LightSource[@lightIndex].ambient;
	ld.dir = normalize(var_LightVector[@lightIndex]);
	ld.halfDir = normalize(var_LightHalfVector[@lightIndex]);
	ld.distance = 1.0;
	ld.attenuation = vec3(0, 0, 0);
	if(@lightIndex == 0) ld.shadow = shadow;
	else ld.shadow = 1.0;
	
	litSurfData.luminance += pointLightFunction(surf, ld);
} 	
$

///////////////////////////////////////////////////////////////////////////////////////////////////
LitSurfaceData computeLighting(SurfaceData surf)
{
	LitSurfaceData litSurfData;
	float shadow = computeShadowMap();
	
	// If we are rendering a back-facing fragment, invert the normal by default
	if(!gl_FrontFacing) surf.normal = -surf.normal;

	litSurfData.luminance = vec4(0, 0, 0, 0);

	@fragmentLightSection
	
	litSurfData.luminance.a = surf.albedo.a * unif_Alpha;
	// Add emissive surface component to final luminance.
	litSurfData.luminance += surf.emissive;
	
	return litSurfData;
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
