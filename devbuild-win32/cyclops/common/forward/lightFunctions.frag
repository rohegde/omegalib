///////////////////////////////////////////////////////////////////////////////////////////////////
vec4 pointLightFunction(SurfaceData sd, LightData ld)
{
	vec4 luminance = vec4(0, 0, 0, 0);
	float lambertTerm = dot(sd.normal, ld.dir) * ld.shadow; 
	if (lambertTerm > 0.0) 
	{ 
		luminance += sd.albedo * ld.diffuse * lambertTerm; 
		
		// Compute specular factor
		float specular = pow( max(dot(ld.halfDir, sd.normal), 0.0), sd.shininess ); 
		luminance += ld.specular * specular * sd.gloss; 
	} 
	return luminance + sd.albedo * ld.ambient;
}
