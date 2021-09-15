#include "object.h"
#ifdef CNC_OPENGL

cnc::graphics::Object::Object(cnc::graphics::Primitive *p,Material& M)
    :
      m_p(p),
      m_M(M)
{

}

cnc::graphics::Object::~Object()
{
    delete m_p;
}

#endif
