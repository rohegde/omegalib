struct Agent
{
	float x, y, z;
	float vx, vy, vz;
	float s1, s2;
};

__kernel void main(__global struct Agent* agent, __private float dt)
{
    uint i = get_global_id(0);
    uint j = get_group_id(0);
	
	agent[i].x = (float)i / 500 + sin(dt * agent[i].z);
	agent[i].y = 2f - (float)i / 500 + sin(dt * agent[i].z);
}
