varying vec3 lightVec;/*!< Light direction vector.*/
varying vec3 viewVec;/*!< View vector passed to the fragment program.*/
varying vec2 texCoord;/*!< Texture coordinates passed to the fragment program.*/
attribute vec4 Tangent;/*!< Tangent space vector.*/
	                     
//!Main function.
/*!
   Calculates the necessary data to use in the fragment shader, light position, eye vector,
   diffuse texture coordinates and shadow map texture coordinates.
*/	
void main(void)
{
	gl_Position = ftransform();
	texCoord = gl_MultiTexCoord0.xy*vec2(1.0, 1.0);
		
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * Tangent.xyz);
	//vec3 b = normalize(gl_NormalMatrix * cross(gl_Normal, Tangent.xyz) * Tangent.w);
	vec3 b = cross(n, t);
		
	vec3 v;
	
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 vVec = -vVertex;
	v.x = dot(vVec, t);
	v.y = dot(vVec, b);
	v.z = dot(vVec, n);
	viewVec = v;

	vec3 lVec = normalize(vec3(gl_LightSource[0].position));
	
	v.x = dot(lVec, t);
	v.y = dot(lVec, b);
	v.z = dot(lVec, n);
	lightVec = v;
	
	vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;

	//ShadowMap Texcoords
    gl_TexCoord[4].s = dot( ecPosition, gl_EyePlaneS[4] );
    gl_TexCoord[4].t = dot( ecPosition, gl_EyePlaneT[4] );
    gl_TexCoord[4].p = dot( ecPosition, gl_EyePlaneR[4] );
    gl_TexCoord[4].q = dot( ecPosition, gl_EyePlaneQ[4] );
    	
	
}
