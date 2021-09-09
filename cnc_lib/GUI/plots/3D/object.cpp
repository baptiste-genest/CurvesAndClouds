#include "object.h"

cnc::graphics::Object::Object(cnc::graphics::Primitive *p)
    :
      m_p(p)
{

}

cnc::graphics::Object::~Object()
{
    delete m_p;
}
