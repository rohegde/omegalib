@use computeShadow

varying vec3 normal,lightDir,halfVector;
varying float dist;

///////////////////////////////////////////////////////////////////////////////////////////////////
void main (void)
{
	float shadow = computeShadow();
	  
	float specular = 0.0;

	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color = gl_FrontMaterial.ambient;
	float att;
	
	n = normalize(normal);

	NdotL = max(dot(n,normalize(lightDir)),0.0);
	if (NdotL > 0.0) 
	{
		// att = 1.0 / (gl_LightSource[0].constantAttenuation +
			// gl_LightSource[0].linearAttenuation * dist +
			// gl_LightSource[0].quadraticAttenuation * dist * dist);
		att = 1.0;
		color += att * NdotL * gl_Color;
		halfV = normalize(halfVector);

		NdotHV = max(dot(n,halfV),0.0);
		// color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular *
					// pow(NdotHV,gl_FrontMaterial.shininess);
	}

    // apply shadow, modulo the ambient bias
    gl_FragColor = color * shadow; 
	//gl_FragColor.a = 0.0;
}
