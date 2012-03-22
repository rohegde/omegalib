@use computeShadow

varying vec3 lightVec;/*!< Light direction vector passed from the vertex program.*/
varying vec3 viewVec;/*!< View vector passed from the vertex program.*/
varying vec2 texCoord;/*!< Texture coordinates passed from the vertex program.*/

///////////////////////////////////////////////////////////////////////////////////////////////////
void main (void)
{
	float shadow = computeShadow();
	vec4 vDiffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;	
    gl_FragColor = vDiffuse * shadow; 
}
