///////////////////////////////////////////////////////////////////////////////////////////////////
vec4 pointLightFunction(SurfaceData sd, LightData ld)
{
	vec4 l = vec4(0, 0, 0, 0);
	float lambertTerm = dot(sd.normal, ld.dir) * ld.shadow; 
	if (lambertTerm > 0.0) 
	{ 
		l += ld.diffuse * lambertTerm; 
		
		// Compute specular factor
		float specular = pow( max(dot(ld.halfDir, sd.normal), 0.0), sd.shininess ); 
		l += ld.specular * specular * sd.gloss; 
	} 
	l += ld.ambient;
	
	vec3 ka = ld.attenuation;
//	float att = clamp(0, 1, 1.0 / (ka[0] + ld.distance * ka[1] + ld.distance * ld.distance * ka[2]));
	float att = 1.0 / (ka[0] + ld.distance * ka[1] + ld.distance * ld.distance * ka[2]);
	
	// Scale luminance by attenuation and return
	vec4 luminance = l * att;
	return luminance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vec4 spotLightFunction(SurfaceData sd, LightData ld)
{
	vec4 l = vec4(0, 0, 0, 0);
	float lambertTerm = dot(sd.normal, ld.dir) * ld.shadow; 
	if (lambertTerm > 0.0) 
	{ 
		float spot = dot(ld.spotDirection, -ld.dir);
		
		if(spot > ld.spotCutoff)
		{
			l += ld.diffuse * lambertTerm; 
			
			// Compute specular factor
			float specular = pow( max(dot(ld.halfDir, sd.normal), 0.0), sd.shininess ); 
			l += ld.specular * specular * sd.gloss; 
			
			vec3 ka = ld.attenuation;
			float att = clamp(0, 1, 1.0 / (ka[0] + ld.distance * ka[1] + ld.distance * ld.distance * ka[2]));
			
			// Scale luminance by attenuation and return
			l *= att * pow(spot, ld.spotExponent);
		}
	} 
	return l;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
vec4 directionalLightFunction(SurfaceData sd, LightData ld)
{
	ld.dir = ld.spotDirection;
	ld.distance = 0;
	ld.halfDir = reflect(-ld.dir, sd.normal);
	return pointLightFunction(sd, ld);
}
