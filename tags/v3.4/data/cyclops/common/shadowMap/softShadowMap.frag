#define SAMPLECOUNT 64 
#define SAMPLECOUNT_FLOAT 64.0 
#define SAMPLECOUNT_D2 32 
#define SAMPLECOUNT_D2_FLOAT 32.0 
#define INV_SAMPLECOUNT (1.0 / SAMPLECOUNT_FLOAT) 

uniform sampler2DShadow osgShadow_shadowTexture;/*!< ShadowMap sampler.*/
uniform sampler3D osgShadow_jitterTexture;/*!< Jittering texture sampler.*/

uniform vec2 osgShadow_ambientBias; /*!< Ambient bias vector.*/
uniform float osgShadow_softnessWidth;/*!< Width softness value.*/ 
uniform float osgShadow_jitteringScale;/*!< Jittering scale value.*/

///////////////////////////////////////////////////////////////////////////////////////////////////
float computeShadowMap()
{
	vec4 sceneShadowProj = gl_TexCoord[4]; 
	float softFactor = osgShadow_softnessWidth * sceneShadowProj.w; 
	vec4 smCoord  = sceneShadowProj; 
	vec3 jitterCoord = vec3( gl_FragCoord.xy / osgShadow_jitteringScale, 0.0 ); 
	float shadow = 0.0; 
      
	// First "cheap" sample test
	const float pass_div = 1.0 / (2.0 * 4.0); 
	for ( int i = 0; i < 4; ++i ) 
	{ 
		// Get jitter values in [0,1]; adjust to have values in [-1,1]
		vec4 offset = 2.0 * texture3D( osgShadow_jitterTexture, jitterCoord ) -1.0; 
		jitterCoord.z += 1.0 / SAMPLECOUNT_D2_FLOAT; 

		smCoord.xy = sceneShadowProj.xy  + (offset.xy) * softFactor; 
		shadow +=  shadow2DProj( osgShadow_shadowTexture, smCoord ).x * pass_div; 

		smCoord.xy = sceneShadowProj.xy  + (offset.zw) * softFactor; 
		shadow +=  shadow2DProj( osgShadow_shadowTexture, smCoord ).x *pass_div; 
	} 
      
	// skip all the expensive shadow sampling if not needed
	if ( shadow * (shadow -1.0) != 0.0 ) 
	{ 
		shadow *= pass_div; 
		for (int i=0; i<SAMPLECOUNT_D2 -4; ++i)
		{ 
		  vec4 offset = 2.0 * texture3D( osgShadow_jitterTexture, jitterCoord ) - 1.0; 
		  jitterCoord.z += 1.0 / SAMPLECOUNT_D2_FLOAT; 

		  smCoord.xy = sceneShadowProj.xy  + offset.xy * softFactor; 
		  shadow +=  shadow2DProj( osgShadow_shadowTexture, smCoord ).x * INV_SAMPLECOUNT; 

		  smCoord.xy = sceneShadowProj.xy  + offset.zw * softFactor; 
		  shadow +=  shadow2DProj( osgShadow_shadowTexture, smCoord ).x * INV_SAMPLECOUNT; 
		} 
	} 
	return (osgShadow_ambientBias.x + shadow * osgShadow_ambientBias.y);
}
