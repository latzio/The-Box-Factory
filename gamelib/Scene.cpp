#include "Scene.h"
#include <iostream>

typedef std::list<SceneNode*> ChildList;

int SceneNode::SCENE_NODE_COUNTER = 0;
int SceneNode::DL_INDEX = 3;

SceneNode::SceneNode(const std::string& name)
  : m_id(SCENE_NODE_COUNTER++)
  , m_name(name)
  , m_bPicked( false )
  , m_parent( 0 )
  , m_nRadius( 0.0 )
  , m_dirty( true )
{
}

SceneNode::~SceneNode()
{
  if (m_children.size() > 0) {
	  // I can't remember if this is legit
	  ChildList::iterator it;
	  for (it = m_children.begin(); it != m_children.end(); it++) {
	    delete *it;
	  }
  }
  
}

SceneNode* SceneNode::clone( )
{
  SceneNode * pNode = new SceneNode( *this );
  pNode->remove_children();

  // recursively call on my children
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    pNode->add_child((*it)->clone());
  }
  
  return pNode;
}

void SceneNode::get_centre(Point3D &p) {

  p = m_trans * p;

  if (m_parent) {
    m_parent->get_centre( p );
  }

}

void SceneNode::render_shadow_volume( Point3D cube[][4], 
                                      Point3D& centre, 
                                      Point3D& light)
{
		//int i;
		//float v[4][3];

}


double SceneNode::get_radius() {
  return m_nRadius;
}

// SPHERE STUFF
#define SPHERE_RADIUS 1.0
#define SPHERE_SLICES 12
#define SPHERE_STACKS 8

// CUBE STUFF
#define LBOUND -1
#define HBOUND 1

void SceneNode::setupDL() {
  // DISPLAY LIST
  // create one display list
  SceneNode::DL_INDEX = glGenLists( DL_INDEX );
  
  GLUquadric* gluq = gluNewQuadric();

  // compile the display list, for a sphere
  glNewList(DL_INDEX + DL_SPHERE, GL_COMPILE);
    // Draw a simple sphere
    gluSphere(  gluq,
                SPHERE_RADIUS,
                SPHERE_SLICES,
                SPHERE_STACKS );
  glEndList();

  // compile the display list, for a plane
  glNewList(DL_INDEX + DL_PLANE, GL_COMPILE);
  
    glBegin(GL_QUADS);

    glNormal3d( 0.0, 1.0, 0.0 );
    glTexCoord2f( 0, 0 );
    glVertex3d( -1, 0, -1 );

    glTexCoord2f( 0, 1 );
    glVertex3d( -1, 0, 1 );

    glTexCoord2f( 1, 1 );
    glVertex3d( 1, 0, 1 );

    glTexCoord2f( 1, 0 );
    glVertex3d( 1, 0, -1 );

    glEnd();

  glEndList();

  // compile the display list, for a cube
  glNewList(DL_INDEX + DL_CUBE, GL_COMPILE);
  glBegin(GL_QUADS);

  // Front
  glNormal3d( 0, 0, 1 );
  glTexCoord2f(0,0);
  glVertex3d(LBOUND, LBOUND, HBOUND);
  glTexCoord2f(0,1);
  glVertex3d(HBOUND, LBOUND, HBOUND);
  glTexCoord2f(1,1);
  glVertex3d(HBOUND, HBOUND, HBOUND);
  glTexCoord2f(1,0);
  glVertex3d(LBOUND, HBOUND, HBOUND);
  
  // Back
  glNormal3d( 0, 0, -1 );
  glTexCoord2f( 0, 0 );
  glVertex3d(LBOUND, LBOUND, LBOUND);
  glTexCoord2f( 1, 0 );
  glVertex3d(LBOUND, HBOUND, LBOUND);
  glTexCoord2f( 1, 1 );
  glVertex3d(HBOUND, HBOUND, LBOUND);
  glTexCoord2f( 0, 1 );
  glVertex3d(HBOUND, LBOUND, LBOUND);
  
  // Left
  glNormal3d( -1, 0, 0 );
  glTexCoord2f( 0, 0 );
  glVertex3d(LBOUND, LBOUND, HBOUND);
  glTexCoord2f( 1, 0 );
  glVertex3d(LBOUND, HBOUND, HBOUND);
  glTexCoord2f( 1, 1 );
  glVertex3d(LBOUND, HBOUND, LBOUND);
  glTexCoord2f( 0, 1 );
  glVertex3d(LBOUND, LBOUND, LBOUND);

  // Right
  glNormal3d( 1, 0, 0);
  glTexCoord2f( 0, 0 );
  glVertex3d(HBOUND, LBOUND, HBOUND);
  glTexCoord2f( 0, 1 );
  glVertex3d(HBOUND, LBOUND, LBOUND);
  glTexCoord2f( 1, 1 );
  glVertex3d(HBOUND, HBOUND, LBOUND);
  glTexCoord2f( 1, 0 );
  glVertex3d(HBOUND, HBOUND, HBOUND);

  // Bottom
  glNormal3d( 0, -1, 0);
  glTexCoord2f( 0, 0 );
  glVertex3d(LBOUND, LBOUND, LBOUND);
  glTexCoord2f( 0, 1 );
  glVertex3d(HBOUND, LBOUND, LBOUND);
  glTexCoord2f( 1, 1 );
  glVertex3d(HBOUND, LBOUND, HBOUND);
  glTexCoord2f( 1, 0 );
  glVertex3d(LBOUND, LBOUND, HBOUND);
  
  // Top
  glNormal3d( 0, 1, 0);
  glTexCoord2f( 0, 0 );
  glVertex3d(LBOUND, HBOUND, LBOUND);
  glTexCoord2f( 1, 0 );
  glVertex3d(LBOUND, HBOUND, HBOUND);
  glTexCoord2f( 1, 1 );
  glVertex3d(HBOUND, HBOUND, HBOUND);
  glTexCoord2f( 0, 1 );
  glVertex3d(HBOUND, HBOUND, LBOUND);

  glEnd();
  glEndList();

}

void SceneNode::tick() {
  // recursively call on my children
  if (!m_children.size())
    return;

  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    (*it)->tick( );
  }
}

void SceneNode::colour(const Colour &c) {
  // recursively call on my children
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    (*it)->colour( c );
  }
}

void SceneNode::set_shadow( bool b ) {
  // recursively call on my children
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    (*it)->set_shadow( b );
  }
}

void SceneNode::walk_gl() const
{ 
  // Apply my transformation
  glPushMatrix();

  if (m_dirty) {
      m_transTranspose = m_trans.transpose();
      m_dirty = false;
  }
  glMultMatrixd( m_transTranspose.begin() );

  /*
  if (is_joint() && m_bPicked) {
    //std::cout << m_name << " is a joint and is picked. " << std::endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else if (is_joint()) {
    //std::cout << m_name << " is a joint and is not picked. " << std::endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  */

  draw_gl( );

  // Default assumption - walk_gl on my children
  ChildList::const_iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    (*it)->walk_gl( );
  }

  glPopMatrix();
}

SceneNode* SceneNode::find( const std::string& aName ) {
	SceneNode* node = 0;

	if (aName == m_name) {
		return this;
	}
	
  // Default assumption - walk_gl on my children
  ChildList::const_iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    node = (*it)->find( aName );

	 if (node) {
		 return node;
	 }
  }

  return node;
}

void SceneNode::find_all( std::vector<SceneNode*> &v, const std::string &name ) {
  
  if (m_name == name) { // add me to vector
    v.push_back( this );
  }

  // walk the children
  ChildList::const_iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    (*it)->find_all( v, name );
  }

}

void SceneNode::draw_gl( ) const 
{

}

int SceneNode::get_id() const {
  return m_id;
}

SceneNode* SceneNode::get_parent() const {
  return m_parent;
}

const std::string& SceneNode::get_name() const {
  return m_name;
}

void SceneNode::toggle_picked() {
  
  m_bPicked = !m_bPicked;
  // std::cout << "I am " << m_name << ". I am picked? " << m_bPicked << std::endl;
   
  // Set my children to be picked
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    if ( ! (*it)->is_joint() ) {
      (*it)->set_picked( m_bPicked );
    }
  }

}

void SceneNode::get_picked_joints( std::list< SceneNode * > * pickedJoints ) {

  if (is_joint() && m_bPicked) {
    pickedJoints->push_back( this );
  }
  // Set my children to be picked
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    if ( (*it)->is_joint() ) {
      (*it)->get_picked_joints( pickedJoints );
    }
  }
}


void SceneNode::set_picked(bool picked) {
  m_bPicked = picked;
}

// Toggle the picked status of this node
bool SceneNode::pick( int id )
{
  // I AM THIS ID
  if (m_id == id) {
    //std::cout << "Id " << id << " found. I am the " << m_name << ". Who is my parent? " << std::endl;
    // I am a jointnode
    if (is_joint()) {
      //std::cout << "This thing is a joint and is getting toggled." << std::endl;
      toggle_picked();
      return true;
    }

    // Parent is a jointnode ?
    // Parent is not a jointnode ?
    SceneNode * node = m_parent;
    while ( node != 0 ) {
      if (node->is_joint()) {
        //std::cout << " -> Parent found! Parent is " << node->get_name() << std::endl;
        node->toggle_picked( );
        return true;
      }
      node = node->get_parent();
    }
    
    return false;
  }

  // Not my id, check my children
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    if ( (*it)->pick( id ) ) {
      return true;
    }
  }

  // Should not get here
  // std::cerr << "ID " << id << " not found !?!?!" << std::endl;
  return false;
}

void SceneNode::applyAction( Matrix4x4* pTransform ) {
      m_trans = m_trans * *pTransform;
      m_dirty = true;
}

void SceneNode::rotatePicked( int degrees ) {

  if (is_joint() && m_bPicked) {
    rotate( 'x', degrees );
  }

  // Now check children
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    (*it)->rotatePicked( degrees );
  }
  
}

void SceneNode::rotateHead( int degrees ) {

  if (is_joint() && m_bPicked && m_name == "head") {
    rotate( 'y', degrees );
    return;
  }

  // Now check children
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    (*it)->rotateHead( degrees );
  }
  
}

void SceneNode::resetOrientation() {

  Matrix4x4 reset;
  m_trans = reset;
  m_dirty = true;

}

void SceneNode::rotate(char axis, double angle)
{
  // std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
  // Fill me in
  Matrix4x4 r;
  size_t topRow, bottomRow;
  size_t leftCol, rightCol;

  // Obtain the four locations for the trig functions
  switch (axis) {

    case 'x':
      topRow = 1;
      bottomRow = 2;
      leftCol = 1;
      rightCol = 2;
      break;

    case 'y':
      topRow = 0;
      bottomRow = 2;
      leftCol = 0;
      rightCol = 2;
      break;

     case 'z':
      topRow = 0;
      bottomRow = 1;
      leftCol = 0;
      rightCol = 1;
      break;   
     
     default:
      std::cerr << "Bad params!" << std::endl;

    }

  double radAngle = -(angle / 180 * M_PI);
  // Add the classic cos and sin requirements
  r[topRow][leftCol] = cos( radAngle );
  r[bottomRow][leftCol] = sin( radAngle );
  r[topRow][rightCol] = sin( radAngle ) * -1;
  r[bottomRow][rightCol] = cos( radAngle );

  m_trans = m_trans * r;
  m_dirty = true;
}

void SceneNode::scale(const Vector3D& amount)
{
  // std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
  // Fill me in
  Matrix4x4 s;
  
  // Add the scale factors in to the main diagonal
  for(int i = 0; i < 3; i++) {
    s[i][i] = amount[i];
  }
  
  m_trans = m_trans * s;
  m_dirty = true;
}

void SceneNode::translate(const Vector3D& amount)
{
  // std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;

  // Fill me in
  Matrix4x4 t;

  // Add the displacements to the right side of the matrix
  for(int i = 0; i < 3; i++) {
    t[i][3] = amount[i];
  }

  m_trans = m_trans * t;
  m_dirty = true;
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
  , m_axis('x')
  , m_nAngle(0) 
  , m_pDestFrame(0) 
{
}

JointNode::~JointNode()
{
}

SceneNode* JointNode::clone( )
{

  SceneNode * pNode = new JointNode( *this );
  pNode->remove_children();

  // recursively call on my children
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    pNode->add_child((*it)->clone());
  }
  
  return pNode;
}

void JointNode::draw_gl() const
{
  //if (picking) {
    // return;
  //}


}

void JointNode::walk_gl( ) const
{
  SceneNode::walk_gl( );
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

void JointNode::add_frame(AnnimationFrame * pFrame) {
	m_KeyFrames.push_back( pFrame );
}

void JointNode::freeze() {
	FrameList::iterator it;
	for (it = m_KeyFrames.begin(); it != m_KeyFrames.end(); it++) {
		(*it)->m_nAngle = m_nAngle;
	   (*it)->m_nRemainingFrames = 0;
	 	(*it)->m_bLoopBack = false;
	}

	if (m_pDestFrame) {
		m_pDestFrame->m_bLoopBack = false;
		m_pDestFrame->m_nAngle = m_nAngle;
		m_pDestFrame->m_nRemainingFrames = 0;
	}

	m_KeyFrames.clear();
}

void JointNode::tick() {

	// Call parent to call children
	SceneNode::tick();

  while (true) {  
    // If no frame, but we have frames get one
  	if (!m_pDestFrame && m_KeyFrames.size() > 0) {
	  	m_pDestFrame = m_KeyFrames.front();
	  	m_KeyFrames.pop_front();
	  }
    // If no frame, quit
  	else if (!m_pDestFrame) {
	  	return;
	  }

    // Make sure this frame has frames remaining.
    if ( m_pDestFrame->m_nRemainingFrames == 0 ) {
      delete m_pDestFrame;
      m_pDestFrame = 0;
    } else {
      break;
    }
  }

	if	(m_pDestFrame->m_nRemainingFrames > 0) {
		// Prepare our next angle
		if ((m_pDestFrame->m_nAngle - m_nAngle) > 185) {
			m_pDestFrame->m_nAngle -= 360;
		} else if ((m_pDestFrame->m_nAngle - m_nAngle) < -185) {
			m_pDestFrame->m_nAngle += 360;
		}

		double nDiffStep = (m_pDestFrame->m_nAngle - m_nAngle)
					 / (double)m_pDestFrame->m_nRemainingFrames;
		
		rotate( m_axis, nDiffStep );

		// Add this to our current known angle;
		m_nAngle += nDiffStep;

		// Avoid overwrapping
		if (m_nAngle < 0) {
			m_nAngle += 360;
		} else if (m_nAngle > 360) {
			m_nAngle -= 360;
		}

		m_pDestFrame->m_nRemainingFrames--;

		if (m_pDestFrame->m_nRemainingFrames == 0) {

			// Either requeue or delete
			if (m_pDestFrame->m_bLoopBack) {
				m_pDestFrame->m_nRemainingFrames = m_pDestFrame->m_nFrames;
				m_KeyFrames.push_back( m_pDestFrame );
			} else {
				delete m_pDestFrame;
			}

			m_pDestFrame = 0;
		} 

	}

}

const double& JointNode::get_angle() {
	return m_nAngle;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name)
  , m_material(0)
  , m_map(0)
  , m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
  /*
  if (m_material) {
    delete m_material;
    m_material = 0;
  }
  if (m_primitive) {
    delete m_primitive;
    m_primitive = 0;
  }*/
}

SceneNode* GeometryNode::clone( )
{
  GeometryNode * pNode = new GeometryNode( *this );
  pNode->remove_children();
  pNode->set_material( m_material );
  pNode->set_primitive( m_primitive );

  // recursively call on my children
  ChildList::iterator it;
  for (it = m_children.begin(); it != m_children.end(); it++) {
    pNode->add_child((*it)->clone());
  }
  
  return pNode;
}

void GeometryNode::draw_gl() const
{

  // Blend if we have a bump map
  if ( false )  {
    // Choose fifty fifty
    //glBlendFunc(GL_ONE, GL_ZERO);

    // Set my bumpmap
    m_map->apply_gl();
    
    // Choose fifty fifty
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

    // Draw my primitive
    m_primitive->walk_gl( );
    
  }

  // Default blending
  // glBlendFunc( GL_ONE, GL_ZERO );
    
  // Set my texture
  m_material->apply_gl();
    
  // Draw my primitive
  m_primitive->walk_gl( );
}
 
void GeometryNode::set_shadow( bool b ) {
  m_primitive->set_shadow( b );

  SceneNode::set_shadow( b );
}

void GeometryNode::colour(const Colour &d) {

  ((PhongMaterial*)m_material)->m_kd = d;
  
  SceneNode::colour( d );
}
