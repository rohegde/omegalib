@use computeShadow

varying vec3 lightVec;/*!< Light direction vector passed from the vertex program.*/
varying vec3 viewVec;/*!< View vector passed from the vertex program.*/
varying vec2 texCoord;/*!< Texture coordinates passed from the vertex program.*/

uniform sampler2D colorMap;/*!< Diffuse texture sampler.*/
uniform sampler2D normalMap;/*!< Normal texture sampler.*/
uniform sampler2D heightMap;/*!< Height map sampler.*/

///////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	float shadow = compteShadow();
	
	vec3 lVec = normalize(lightVec);
	vec3 vVec = normalize(viewVec);
		
	// Calculate offset, scale & biais
	float height = texture2D(heightMap, texCoord).x ;
	vec2 newTexCoord = texCoord + ((height * 0.06 - 0.03) * (vVec.xy));
		
	vec4 base = texture2D(colorMap, newTexCoord);
	//vec4 base = vec4(0.8, 0.8, 0.8, 1);
	
	vec3 bump = normalize(texture2D(normalMap, newTexCoord).xyz * 2.0 - 1.0);
	bump = normalize(bump);
	
	float diffuse = max( dot(lVec, bump), 0.3 );
		
	float specular = pow(clamp(dot(reflect(-vVec, bump), lVec), 0.0, 1.0), gl_FrontMaterial.shininess );
	
	vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	vec4 vDiffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * diffuse;	
	vec4 vSpecular = gl_LightSource[0].specular * gl_FrontMaterial.specular *specular;	
	//gl_FragColor = vAmbient*base + (vDiffuse*base + vSpecular);	
	
	
    // apply shadow, modulo the ambient bias
    gl_FragColor = (vAmbient*base + (vDiffuse*base + vSpecular)) * shadow; 
    // gl_FragColor = vec4(diffuse, diffuse, diffuse, diffuse) * (osgShadow_ambientBias.x + shadow * osgShadow_ambientBias.y); 
}
