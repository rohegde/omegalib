@use computeShadow

varying vec3 lightVec;/*!< Light direction vector passed from the vertex program.*/
varying vec3 viewVec;/*!< View vector passed from the vertex program.*/
varying vec2 texCoord;/*!< Texture coordinates passed from the vertex program.*/

uniform sampler2D unif_ColorMap;/*!< Diffuse texture sampler.*/
uniform sampler2D unif_NormalMap;/*!< Normal texture sampler.*/
uniform sampler2D unif_HeightMap;/*!< Height map sampler.*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	float shadow = computeShadow();
	
	vec3 lVec = normalize(lightVec);
	vec3 vVec = normalize(viewVec);
		
	// Calculate offset, scale & biais
	float height = texture2D(unif_HeightMap, texCoord).x ;
	vec2 newTexCoord = texCoord; //+ ((height * 0.02 - 0.01) * (vVec.xy));
		
	vec4 base = texture2D(unif_ColorMap, newTexCoord);
	//vec4 base = vec4(0.8, 0.8, 0.8, 1);
	
	vec3 bump = normalize(texture2D(unif_NormalMap, newTexCoord).xyz * 2.0 - 1.0);
	bump = normalize(bump);
	
	float diffuse = dot(lVec, bump);
		
	//float specular = pow(clamp(dot(reflect(-vVec, bump), lVec), 0.0, 1.0), gl_FrontMaterial.shininess );
	
	vec4 vAmbient = vec4(0.4, 0.4, 0.4, 1.0);
	vec4 vDiffuse = gl_LightSource[0].diffuse * diffuse;	
	//vec4 vDiffuse = gl_Color;	
	//vec4 vSpecular = gl_LightSource[0].specular * gl_FrontMaterial.specular *specular;	
	//gl_FragColor = vAmbient*base + (vDiffuse*base + vSpecular);	
	
	
    // apply shadow, modulo the ambient bias
    //gl_FragColor = (vAmbient*base + (vDiffuse*base + vSpecular)) * shadow; 
    gl_FragColor = (vAmbient * base + vDiffuse * base) * shadow; 
    //gl_FragColor = vec4(diffuse, diffuse, diffuse, diffuse) * (osgShadow_ambientBias.x + shadow * osgShadow_ambientBias.y); 
	//gl_FragColor = vec4(0, 1, 1, 1);
}
