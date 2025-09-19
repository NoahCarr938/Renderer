#pragma once
#include "Renderer.h"
#include "Transform.h"

using namespace aie;

class Object
{
public:
	// Should we name it the same as the name type?
	Transform MyTransform;
	Geometry * Geo;

	Shader * Shad;

	Object();

	virtual void Tick(float deltaTime) {}
	virtual void Draw() const;
};

