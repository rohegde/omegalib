{
	// Compute light direction
	vec3 lightDir = var_LightVector[@lightIndex];
	vec3 halfVec = var_LightHalfVector[@lightIndex];

	float lambertTerm = dot(lightDir, N); 

	if(@lightIndex == 0) lambertTerm *= shadow;
	if (lambertTerm > 0.0) 
	{ 
		ld.luminance += surf.albedo * gl_LightSource[@lightIndex].diffuse * lambertTerm; 
		float specular = pow( max(dot(halfVec, N), 0.0), surf.shininess ); 
		ld.luminance += gl_LightSource[@lightIndex].specular * specular * surf.gloss; 
	}
} 
