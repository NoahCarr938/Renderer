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
	Shader * TexShad;
	Texture * Tex;


	Object();
	virtual void Tick(float deltaTime);
	virtual void Draw() const;
};

