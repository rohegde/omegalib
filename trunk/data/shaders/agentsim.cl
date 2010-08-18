struct Agent
{
	float x, y, z;
	float vx, vy, vz;
	float s1, s2;
};

struct InteractorRay
{
	float x, y, z;
	float dx, dy, dz;
	float s1, s2;
};

__kernel void behavior(
	__global struct Agent* agent, 
	__private float dt,
	__private float4 center, 
	__private int numAgents, 
	__private int totGroups, 
	__private int groupId, 
	__private int numInteractors,
	__constant struct InteractorRay* interactor)
{
    uint i = get_global_id(0);
	
	i = i * totGroups + groupId;

	// Compute attraction vector.
	float4 pos = (float4)(agent[i].x, agent[i].y, agent[i].z, 0);
	
	float4 attractDir = normalize(center - pos);
	//float4 attractDir = (float4)(0, 0, 0, 0);
	
	// Compute avoidance vector.
	float4 avoidDir = (float4)(0, 0, 0, 0);
	float4 coordDir = (float4)(0, 0, 0, 0);
	for(int j = 0; j < numAgents; j++)
	{
		float4 pos2 = (float4)(agent[j].x, agent[j].y, agent[j].z, 0);
		float4 dv = pos2 - pos;
		if(length(dv) < 0.03f)
		{
			avoidDir -= normalize(dv);
		}
		if(length(dv) < 0.15f)
		{
			coordDir += (float4)(agent[j].vx, agent[j].vy, agent[j].vz, 0);
		}
		//attractDir += normalize(dv);
	}
	
	//attractDir = normalize(attractDir);
	coordDir = normalize(coordDir);
	//avoidDir = -normalize(avoidDir);
	
	float4 dir = normalize(attractDir + avoidDir + coordDir);
	
	int friction = 0.1f / dt;
	
	float4 vel = (float4)(agent[i].vx, agent[i].vy, agent[i].vz, 0);
	vel = (vel * friction + dir) / (friction + 1);
	vel = normalize(vel);
	
	for(int j = 0; j < numInteractors; j++)
	{
		float4 iorig = (float4)(interactor[j].x, interactor[j].y, interactor[j].z, 0);
		float4 idir = (float4)(interactor[j].dx, interactor[j].dy, interactor[j].dz, 0);
		float t0num = dot(idir, (pos - iorig));
		float t0den = dot(idir, idir);
		float t0 = t0num / t0den;
		float4 ptol = pos - (iorig + t0 * idir);
		float idist = length(ptol);
		if(idist < 0.2)
		{
			vel += (ptol / idist) / 2;
		}
	}
	
	agent[i].vx = vel.x;
	agent[i].vy = vel.y;
	agent[i].vz = vel.z;
}

__kernel void update(__global struct Agent* agent, __private float dt)
{
    uint i = get_global_id(0);
	
	agent[i].x += agent[i].vx * dt;
	agent[i].y += agent[i].vy * dt;
	agent[i].z += agent[i].vz * dt;
}
