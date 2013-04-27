/*  BCC.cpp

        This is the OSG Version, Written by John Bell July 2012
*/

//#include <cstdlib>
//#include <iostream>
//#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Group>
//#include <osg/ref_ptr>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
//#include <osgGA/UFOManipulator>

#define OMEGA_NO_GL_HEADERS
#include <omega.h>
#include <omegaToolkit.h>
#include <omegaOsg.h>

#include <btBulletCollisionCommon.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbCollision/Utils.h>
#include <osgwTools/Shapes.h>
#include <osgwTools/Version.h>

using namespace omega;
using namespace omegaOsg;

//using namespace std;

/*
class MoveManipulator : public osgGA::GUIEventHandler
{
public:
    MoveManipulator() : _co( NULL ), _mt( NULL ) {}
    MoveManipulator( const MoveManipulator& mm, osg::CopyOp copyop ) : _co( mm._co ), _mt( mm._mt ) {}
    ~MoveManipulator() {}
#if( OSGWORKS_OSG_VERSION > 20800 )
    META_Object(osgBulletExample,MoveManipulator);
#endif

    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        if( ( ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL ) == 0 )
        {
            return( false );
        }
        else if( ea.getEventType() == osgGA::GUIEventAdapter::PUSH )
        {
            _lastX = ea.getXnormalized();
            _lastY = ea.getYnormalized();
            return( true );
        }
        else if( ea.getEventType() == osgGA::GUIEventAdapter::DRAG )
        {
            double deltaX = ea.getXnormalized() - _lastX;
            double deltaY = ea.getYnormalized() - _lastY;
            _lastX = ea.getXnormalized();
            _lastY = ea.getYnormalized();

            deltaX *= 6.;
            deltaY *= 6.;
            osg::Matrix trans = osgbCollision::asOsgMatrix( _co->getWorldTransform() );
            trans = trans * osg::Matrix::translate( deltaX, 0., deltaY );
            _mt->setMatrix( trans );
            _co->setWorldTransform( osgbCollision::asBtTransform( trans ) );
            return( true );
        }
        return( false );
    }

    void setCollisionObject( btCollisionObject* co ) { _co = co; }
    void setMatrixTransform( osg::MatrixTransform* mt ) { _mt = mt; }

protected:
    btCollisionObject* _co;
    osg::MatrixTransform* _mt;
    double _lastX, _lastY;
};
*/

osg:: ref_ptr<osg::Node> createScene( btCollisionWorld* );
osg:: ref_ptr<osg::Node> createAxes( void );

// OMEGALIB STUFF BEGIN -------------------------------------------------------

class BCCApplication : public EngineModule
{
public:
        void initialize()
		{
                // Create and setup the omegalib osg integration module
                myOsg = new OsgModule();
                ModuleServices::addModule(myOsg); 
				myColWorld = initCollision();

				//myMoveManipulator = new MoveManipulator;

                // Create an omegalib scene node and attach the root osg node to it. This is used to interact with the 
                // osg object through omegalib interactors.
                OsgSceneObject* oso = new OsgSceneObject( createScene(myColWorld).get() );
                //const AlignedBox3* bbox = oso->getBoundingBox();
                myOsg->setRootNode(oso->getTransformedNode());

                // Resize the entire scene
                myRoot = SceneNode::create("osgRoot");
                myRoot->addComponent(oso);
                myRoot->setScale(0.05f, 0.05f, 0.05f);
        }

		virtual btCollisionWorld* initCollision();

private:
	btCollisionWorld* myColWorld;
	//MoveManipulator* myMoveManipulator;
	Ref<OsgModule> myOsg;
	Ref<SceneNode> myRoot;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
btCollisionWorld* BCCApplication::initCollision()
{
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher( collisionConfiguration );

    btVector3 worldAabbMin( -10000, -10000, -10000 );
    btVector3 worldAabbMax( 10000, 10000, 10000 );
    btBroadphaseInterface* inter = new btAxisSweep3( worldAabbMin, worldAabbMax, 1000 );

    btCollisionWorld* collisionWorld = new btCollisionWorld( dispatcher, inter, collisionConfiguration );

    return( collisionWorld );
}

int main(int argc, char** argv) 
{
	Application<BCCApplication> app("bcc");
    return omain(app, argc, argv);

} // main
// OMEGALIB STUFF END -------------------------------------------------------

osg:: ref_ptr<osg::Node> createScene( btCollisionWorld* btcw ) {

        // Building from the top down - Start with the root

        osg::ref_ptr< osg::Group > root = new osg::Group;

        // Declare a single matrix, which will be re-used repeatedly for each Transform

        osg::Matrix m;
        osg::ref_ptr< osg::MatrixTransform > mt = NULL;
        
        // Now to create red and pink Geodes, to hang on the MatrixTransforms

		//osg::Sphere* ball = new osg::Sphere( osg::Vec3f( 0.0, 0.0, 0.0 ), 10.0 );
        //osg::Geode* redGeode = new osg::Geode;
		//osg::ShapeDrawable* redBall = new osg::ShapeDrawable(ball); 
		osg::ref_ptr< osg::Sphere > ball = new osg::Sphere( osg::Vec3f( 0.0, 0.0, 0.0 ), 10.0 );
        osg::ref_ptr< osg::Geode > redGeode = new osg::Geode;
        osg::ref_ptr< osg::ShapeDrawable > redBall = new osg::ShapeDrawable( ball.get() );
		
		redBall->setColor( osg::Vec4( 1.0, 0.0, 0.0, 1.0 ) );
		redGeode->addDrawable( redBall );
        //redGeode->addDrawable( osgwTools::makeBox( osg::Vec3( 15, 15, 15) ) );



        osg::ref_ptr< osg::Geode > pinkGeode = new osg::Geode;
        osg::ref_ptr< osg::ShapeDrawable > pinkBall = new osg::ShapeDrawable( ball );
        pinkBall->setColor( osg::Vec4( 1.0, 0.5, 0.5, 1.0 ) );
        pinkGeode->addDrawable( pinkBall );

        // Now to add a semitransparent box

        //osg::ref_ptr< osg::Box > box = new osg::Box( osg::Vec3f( 150.0, 150.0, 150.0 ), 100.0 ); 
		//osg::ref_ptr< osg::ShapeDrawable > blueBox = new osg::ShapeDrawable( box );
        //blueBox->setColor( osg::Vec4( 0.5, 0.5, 1.0, 0.1 ) );
		//boxGeode->addDrawable( blueBox );
		osg::ref_ptr< osg::Geode > boxGeode = new osg::Geode;
		boxGeode->addDrawable( osgwTools::makeBox( osg::Vec3f( 50.0, 50.0, 50.0 ) ) );
		
		osg::Matrix transMatrix = osg::Matrix::translate( 150., 150., 150. );
		mt = new osg::MatrixTransform( transMatrix );
		mt->addChild( boxGeode.get() );
        root->addChild( mt.get() );

		//btCollisionObject* btBoxObject = new btCollisionObject();
		//btBoxObject->setCollisionShape( osgbCollision::btBoxCollisionShapeFromOSG( boxGeode ) ); // <-- here a Bullet object's shape is defined as a leaf node of OSG
		//btBoxObject->setCollisionFlags( btCollisionObject::CF_STATIC_OBJECT );
		//btcw->addCollisionObject( btBoxObject );

		// Another Box
		boxGeode = new osg::Geode;
		boxGeode->addDrawable( osgwTools::makeWireBox( osg::Vec3( 50, 50, 50 ) ) );
		transMatrix = osg::Matrix::translate( 300., 150., 150. );
		mt = new osg::MatrixTransform( transMatrix );
		mt->addChild( boxGeode.get() );
		root->addChild( mt.get() );

		//btBoxObject = new btCollisionObject();
		//btBoxObject->setCollisionShape( osgbCollision::btBoxCollisionShapeFromOSG( boxGeode ) ); // <-- here a Bullet object's shape is defined as a leaf node of OSG
		//btBoxObject->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT );
		//btBoxObject->setWorldTransform( osgbCollision::asBtTransform( transMatrix ) );
		//btcw->addCollisionObject( btBoxObject );
		// following two lines are why this object is draggable
		//mm->setCollisionObject( btBoxObject );
		//mm->setMatrixTransform( mt.get() );

        /*/ Now to build a whole array of spheres

        for( int ix = 0; ix < 4; ix++ ) {
                for( int iy = 0; iy < 4; iy++ ) {
                        for( int iz = 0; iz < 4; iz++ ) { 

                        // Create the  matrixTransform, set its value, and add it

                        mt = new osg::MatrixTransform;
                        m.setTrans( 100 * ix, 100 * iy, 100 * iz );
                        mt->setMatrix( m );
                        root->addChild( mt.get( ) );

                        if( ix < 1 || ix > 2 || iy < 1 || iy > 2 || iz < 1 || iz > 2 )
                                mt->addChild( pinkGeode );
                        else
                                mt->addChild( redGeode );

                        } // iz
                } // iy
        } // ix

        // Now to build another one in between the first one ( BCC )

        for( int ix = 0; ix < 3; ix++ ) {
                for( int iy = 0; iy < 3; iy++ ) {
                        for( int iz = 0; iz < 3; iz++ ) { 

                        // Create the  matrixTransform, set its value, and add it

                        mt = new osg::MatrixTransform;
                        m.setTrans( 100 * ix + 50, 100 * iy + 50, 100 * iz + 50 );
                        mt->setMatrix( m );
                        root->addChild( mt.get( ) );

                        if( ix == 1 && iy == 1 && iz == 1 )
                                mt->addChild( redGeode );
                        else
                                mt->addChild( pinkGeode );

                        } // iz
                } // iy
        } // ix

		//*/

        root->addChild( createAxes( ).get( ) );

        return root.get( );


} // createScene

osg:: ref_ptr<osg::Node> createAxes( void ) {

        // This method should be made more succinct by using arrays.

        const osg::Vec4f red( 1.0, 0.0, 0.0, 1.0 ), green( 0.0, 1.0, 0.0, 1.0 ), blue ( 0.0, 0.0, 1.0, 1.0 );
        osg::Matrix mR, mT;

        osg::ref_ptr< osg::Group > root = new osg::Group;
        osg::ref_ptr< osg::MatrixTransform > mt = NULL;
        osg::ref_ptr< osg::Geode > geode = NULL;

        osg::ref_ptr< osg::Cylinder > axis = new osg::Cylinder( osg::Vec3f( 0.0, 0.0, 0.0 ), 2.0, 500.0 );
        osg::ref_ptr< osg::Cone > arrow = new osg::Cone( osg::Vec3f( 0.0, 0.0, 0.0 ), 10.0, 20.0 );

        // Draw the X axis in Red

        osg::ref_ptr< osg::ShapeDrawable > xAxis = new osg::ShapeDrawable( axis );
        xAxis->setColor( red );
        geode = new osg::Geode;
        geode->addDrawable( xAxis.get( ) );
        mT.makeTranslate( 250.0, 0.0, 0.0 );
        mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 0.0, 1.0, 0.0 ) );
        mt = new osg::MatrixTransform;
        mt->setMatrix( mR * mT );
        mt->addChild( geode.get( ) );
        root->addChild( mt.get( ) );

        osg::ref_ptr< osg::ShapeDrawable > xArrow= new osg::ShapeDrawable( arrow );
        xArrow->setColor( red );
        geode = new osg::Geode;
        geode->addDrawable( xArrow.get( ) );
        mT.makeTranslate( 500.0, 0.0, 0.0 );
        mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 0.0, 1.0, 0.0 ) );
        mt = new osg::MatrixTransform;
        mt->setMatrix( mR * mT );
        root->addChild( mt.get( ) );
        mt->addChild( geode.get( ) );

        // Then the Y axis in green
        osg::ref_ptr< osg::ShapeDrawable > yAxis = new osg::ShapeDrawable( axis );
        yAxis->setColor( green );
        geode = new osg::Geode;
        geode->addDrawable( yAxis.get( ) );
        mT.makeTranslate( 0.0, 250.0, 0.0 );
        mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 1.0, 0.0, 0.0 ) );
        mt = new osg::MatrixTransform;
        mt->setMatrix( mR * mT );
        mt->addChild( geode.get( ) );
        root->addChild( mt.get( ) );

        osg::ref_ptr< osg::ShapeDrawable > yArrow= new osg::ShapeDrawable( arrow );
        yArrow->setColor( green );
        geode = new osg::Geode;
        geode->addDrawable( yArrow.get( ) );
        mT.makeTranslate( 0.0, 500.0, 0.0 );
        mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( -1.0, 0.0, 0.0 ) );
        mt = new osg::MatrixTransform;
        mt->setMatrix( mR * mT );
        root->addChild( mt.get( ) );
        mt->addChild( geode.get( ) );

        // And the Z axis in blue

        osg::ref_ptr< osg::ShapeDrawable > zAxis = new osg::ShapeDrawable( axis );
        zAxis->setColor( blue );
        geode = new osg::Geode;
        geode->addDrawable( zAxis.get( ) );
        mT.makeTranslate( 0.0, 0.0, 250.0 );
        //mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 0.0, 1.0, 0.0 ) );
        mt = new osg::MatrixTransform;
        mt->setMatrix( mT );
        mt->addChild( geode.get( ) );
        root->addChild( mt.get( ) );

        osg::ref_ptr< osg::ShapeDrawable > zArrow= new osg::ShapeDrawable( arrow );
        zArrow->setColor( blue );
        geode = new osg::Geode;
        geode->addDrawable( zArrow.get( ) );
        mT.makeTranslate( 0.0, 0.0, 500.0 );
        //mR.makeRotate( 3.14159 / 2.0, osg::Vec3f( 0.0, 1.0, 0.0 ) );
        mt = new osg::MatrixTransform;
        mt->setMatrix(mT );
        root->addChild( mt.get( ) );
        mt->addChild( geode.get( ) );

        return root.get( );

} // createAxes