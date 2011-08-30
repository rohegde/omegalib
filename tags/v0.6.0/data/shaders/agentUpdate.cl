struct Agent
{
	float3 pos;
	float3 vel;
	float2 state;
}

__kernel void main(
							 __global  Agent* agent
                             __global float dt)
{
    uint i = get_global_id(0);
    uint j = get_group_id(0);
}
