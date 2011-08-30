#version 120

uniform vec2 lightpos;

void main(void)
{
	vec2 lpos = (lightpos - gl_PointCoord.st); 
	
	float val = smoothstep(0.5, 1.5, 1 - length(gl_PointCoord.st - vec2(0.5f, 0.5f)));
	float lval = (0.6 - length(lpos) / 2);//lpos.x + lpos.y;
	float col = lval;
    gl_FragColor.r = col;
    gl_FragColor.g = col;
    gl_FragColor.b = col + gl_PointCoord.t / 8;
    gl_FragColor.a = val;
}