@fsinclude shadowMap

varying vec3 var_EyeVector;

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
@customFragmentFunctions

///////////////////////////////////////////////////////////////////////////////////////////////////
$@fragmentLightSection
{ 
	LightData ld;
	
	ld.diffuse = gl_LightSource[@lightIndex].diffuse;
	ld.specular = gl_LightSource[@lightIndex].specular;
	ld.ambient = gl_LightSource[@lightIndex].ambient;
	ld.dir = normalize(vec3(gl_LightSource[@lightIndex].position.xyz + var_EyeVector));
	ld.halfDir = reflect(-ld.dir, surf.normal);
	ld.distance = 1.0;
	ld.attenuation = vec3(0, 0, 0);
	if(@lightIndex == 0) ld.shadow = shadow;
	else ld.shadow = 1.0;
	
	litSurfData.luminance += @lightFunction(surf, ld);
} 	
$

///////////////////////////////////////////////////////////////////////////////////////////////////
LitSurfaceData computeLighting(SurfaceData surf)
{
	LitSurfaceData litSurfData;
	float shadow = computeShadowMap();
	
	surf.normal = normalize(surf.normal);
	
	// If we are rendering a back-facing fragment, invert the normal by default
	if(!gl_FrontFacing) surf.normal = -surf.normal;

	litSurfData.luminance = vec4(0, 0, 0, 0);

	@fragmentLightSection

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
