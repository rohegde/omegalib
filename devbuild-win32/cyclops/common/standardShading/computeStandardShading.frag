uniform vec4 unif_Ambient;

varying vec3 var_Normal;
varying vec3 var_LightDir;
varying vec3 var_HalfVector;
varying float var_LightDist;

///////////////////////////////////////////////////////////////////////////////////////////////////
vec4 lightAmbientContribution()
{
	return unif_Ambient;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vec4 lightDiffuseContribution()
{
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	float att;
	
	vec3 N = normalize(var_Normal);
	vec3 L = normalize(var_LightDir);
	
	float diffuseTerm = dot(N, L) * 0.5 + 0.5;
	if(diffuseTerm > 0.0) 
	{
		// att = 1.0 / (gl_LightSource[0].constantAttenuation +
			// gl_LightSource[0].linearAttenuation * dist +
			// gl_LightSource[0].quadraticAttenuation * dist * dist);
		att = 1.0;
		color +=  diffuseTerm * att * gl_LightSource[0].diffuse;
		
		//halfV = normalize(var_HalfVector);

		//NdotHV = max(dot(n,halfV),0.0);
		// color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular *
					// pow(NdotHV,gl_FrontMaterial.shininess);
	}
	
	return color;
}
