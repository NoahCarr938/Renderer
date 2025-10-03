#include "Object.h"

Object::Object()
{
}

void Object::Tick(float deltaTime)
{
}

void Object::Draw() const
{
	aie::Draw(*Shad, *Geo);
}
