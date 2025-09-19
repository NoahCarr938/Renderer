#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Transform
{
public:

	// get/set parent
	void SetParent(Transform* newParent);
	Transform* GetParent() const;

	// get children
	Transform* GetChildAtIndex(size_t index) const;
	size_t GetChildCount() const;

	// get local model matrix
	glm::mat4 LocalMat() const;
	// get global model matrix
	glm::mat4 WorldMat() const;

	glm::vec3 LocalPos;
	glm::quat LocalRot;
	glm::vec3 LocalScl;

	glm::vec3 WorldPos;
	glm::quat WorldRot;
	glm::vec3 WorldScl;

	Transform();

	// matrix used for converting transforming local-space coords to world-space
	glm::mat4 LocalToWorldMatrix() const;
	// matrix used for converting transforming world-space coords to local-space
	glm::mat4 WorldToLocalMatrix() const;

private:
	// can add private funcs to help with above\

public:

	// get/set world-space position
	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);

	// get/set world-space rotation
	glm::quat GetRotation() const;
	void SetRotation(glm::quat rotation);

	// get/set world-space forward
	glm::vec3 GetForward() const;
	void SetForward(glm::vec3 forward);

	// get world-space scale (sort of)
	glm::vec3 GetLossyScale() const;
};

