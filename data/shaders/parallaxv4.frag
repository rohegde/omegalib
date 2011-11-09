/*******************************************************************************/
/*
 * Author: Ing. Jose Larios Delgado.
 * Project:  glsl shaders implementation with OpenSceneGraph.
 * Institutions: DGSCA, UNAM, Virtual Reality Department.
 * Description: Parallax Map for a directional light with soft shadow.   
 */
/*******************************************************************************/

#define SAMPLECOUNT 64 
#define SAMPLECOUNT_FLOAT 64.0 
#define SAMPLECOUNT_D2 32 
#define SAMPLECOUNT_D2_FLOAT 32.0 
#define INV_SAMPLECOUNT (1.0 / SAMPLECOUNT_FLOAT) 

varying vec3 lightVec;/*!< Light direction vector passed from the vertex program.*/
varying vec3 viewVec;/*!< View vector passed from the vertex program.*/
varying vec2 texCoord;/*!< Texture coordinates passed from the vertex program.*/

uniform sampler2DShadow osgShadow_shadowTexture;/*!< ShadowMap sampler.*/
uniform sampler3D osgShadow_jitterTexture;/*!< Jittering texture sampler.*/

uniform vec2 osgShadow_ambientBias; /*!< Ambient bias vector.*/
uniform float osgShadow_softnessWidth;/*!< Width softness value.*/ 
uniform float osgShadow_jitteringScale;/*!< Jittering scale value.*/

uniform sampler2D colorMap;/*!< Diffuse texture sampler.*/
uniform sampler2D normalMap;/*!< Normal texture sampler.*/
uniform sampler2D heightMap;/*!< Height map sampler.*/

//!Main function.
/*!
   Calculates per pixel illumination for a directional light and the color components 
   for the parallax map effect, filters the shadow map texture and obtains the final 
   fragment color.  
*/
void main (void)
{

	  vec4 sceneShadowProj  = gl_TexCoord[4]; 
      float softFactor = osgShadow_softnessWidth * sceneShadowProj.w; 
      vec4 smCoord  = sceneShadowProj; 
      vec3 jitterCoord = vec3( gl_FragCoord.xy / osgShadow_jitteringScale, 0.0 ); 
      vec4 shadow = vec4(0.0, 0.0, 0.0, 0.0); 
      
      // First "cheap" sample test
      const float pass_div = 1.0 / (2.0 * 4.0); 
      for ( int i = 0; i < 4; ++i ) 
      { 
      
        // Get jitter values in [0,1]; adjust to have values in [-1,1]
        vec4 offset = 2.0 * texture3D( osgShadow_jitterTexture, jitterCoord ) -1.0; 
        jitterCoord.z += 1.0 / SAMPLECOUNT_D2_FLOAT; 

        smCoord.xy = sceneShadowProj.xy  + (offset.xy) * softFactor; 
        shadow +=  shadow2DProj( osgShadow_shadowTexture, smCoord ) * pass_div; 

        smCoord.xy = sceneShadowProj.xy  + (offset.zw) * softFactor; 
        shadow +=  shadow2DProj( osgShadow_shadowTexture, smCoord ) *pass_div; 
      } 
      
      // skip all the expensive shadow sampling if not needed
      if ( shadow * (shadow -1.0) != 0.0 ) 
      { 
        shadow *= pass_div; 
        for (int i=0; i<SAMPLECOUNT_D2 -4; ++i){ 
          vec4 offset = 2.0 * texture3D( osgShadow_jitterTexture, jitterCoord ) - 1.0; 
          jitterCoord.z += 1.0 / SAMPLECOUNT_D2_FLOAT; 

          smCoord.xy = sceneShadowProj.xy  + offset.xy * softFactor; 
          shadow +=  shadow2DProj( osgShadow_shadowTexture, smCoord ) * INV_SAMPLECOUNT; 

          smCoord.xy = sceneShadowProj.xy  + offset.zw * softFactor; 
          shadow +=  shadow2DProj( osgShadow_shadowTexture, smCoord ) * INV_SAMPLECOUNT; 
        } 
      } 




	vec3 lVec = normalize(lightVec);
	vec3 vVec = normalize(viewVec);
		
	// Calculate offset, scale & biais
	float height = texture2D(heightMap, texCoord).a ;
	vec2 newTexCoord = texCoord + ((height * 0.2 - 0.1) * (vVec.xy));
		
		
	vec4 base = texture2D(heightMap, newTexCoord);
	vec3 bump = normalize(texture2D(normalMap, newTexCoord).xyz * 2.0 - 1.0);
	bump = normalize(bump);
	
	float diffuse = max( dot(lVec, bump), 0.3 );
		
	float specular = pow(clamp(dot(reflect(-vVec, bump), lVec), 0.0, 1.0), gl_FrontMaterial.shininess );
	
	vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	vec4 vDiffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * diffuse;	
	vec4 vSpecular = gl_LightSource[0].specular * gl_FrontMaterial.specular *specular;	
	//gl_FragColor = vAmbient*base + (vDiffuse*base + vSpecular);	
	
	
    // apply shadow, modulo the ambient bias
    //gl_FragColor = (vAmbient*base + (vDiffuse*base + vSpecular)) * (osgShadow_ambientBias.x + shadow * osgShadow_ambientBias.y); 
    gl_FragColor = base * (osgShadow_ambientBias.x + shadow * osgShadow_ambientBias.y); 
   
}
