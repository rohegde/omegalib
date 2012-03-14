//varying vec3 lightVec;/*!< Light direction vector.*/
//varying vec3 viewVec;/*!< View vector passed to the fragment program.*/
varying vec3 normal,lightDir,halfVector;
varying float dist;
	                     
//!Main function.
void main(void)
{
	gl_Position = ftransform();
	
	vec4 ecPosition;
	vec3 aux;
	normal = normalize(gl_NormalMatrix * gl_Normal);
		
	/* these are the new lines of code to compute the light's direction */
	ecPosition = gl_ModelViewMatrix * gl_Vertex;
	aux = vec3(gl_LightSource[0].position-ecPosition);
	lightDir = normalize(aux);
	dist = length(aux);
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);
	
	//ShadowMap Texcoords
    gl_TexCoord[4].s = dot( ecPosition, gl_EyePlaneS[4] );
    gl_TexCoord[4].t = dot( ecPosition, gl_EyePlaneT[4] );
    gl_TexCoord[4].p = dot( ecPosition, gl_EyePlaneR[4] );
    gl_TexCoord[4].q = dot( ecPosition, gl_EyePlaneQ[4] );
    	
	gl_FrontColor = gl_Color;
	
}
