class OsgbBasicDemo : public EngineModule
{

	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*>	myCollisionShapes;

	btBroadphaseInterface*	myDBroadphase;

	btCollisionDispatcher*	myispatcher;

	btConstraintSolver*	mySolver;

	btDefaultCollisionConfiguration* myCollisionConfiguration;

	public:

	OsgbBasicDemo()
	{
	}
	virtual ~OsgbBasicDemo()
	{
		exitPhysics();
	}
	
	void initPhysics();
	void exitPhysics();

	//virtual void clientMoveAndDisplay();

	//virtual void displayCallback();
	void resetScene();
	
	virtual void initialize();
	virtual void update(const UpdateContext& context);

	static DemoApplication* Create()
	{
		OsgbBasicDemo* demo = new OsgbBasicDemo;
		demo->myinit();
		demo->initPhysics();
		return demo;
	}
	
};

void BasicDemo::exitPhysics()
{

	//cleanup in the reverse order of creation/initialization
	//remove the rigidbodies from the dynamics world and delete them
	for (int i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = myDynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	
	delete m_solver;
	
	delete m_broadphase;
	
	delete m_dispatcher;

	delete m_collisionConfiguration;

	
}

void OsgbBasicDemo::resetScene()
{
	exitPhysics();
	initPhysics();
}

void OsgbBasicDemo::update(const UpdateContext& context)
{

}

int main(int argc, char** argv)
{
	Application<OsgbBasicDemo> app("osgbBasicDemo");
	return omain(OsgbBasicDemo, argc, argv);
}