#include "Transform.h"

void Transform::SetParent(Transform* newParent)
{
}

Transform* Transform::GetParent() const
{
	return nullptr;
}

Transform* Transform::GetChildAtIndex(size_t index) const
{
	return nullptr;
}

size_t Transform::GetChildCount() const
{
	return size_t();
}

glm::mat4 Transform::LocalMat() const
{
	glm::mat4 trs = glm::translate(glm::mat4(1.0f), LocalPos); // translation
	trs *= glm::toMat4(LocalRot);                              // rotation
	trs = glm::scale(trs, LocalScl);                           // scale
	return trs;
}

glm::mat4 Transform::WorldMat() const
{
	glm::mat4 trs = glm::translate(glm::mat4(1.0f), WorldPos);
	trs *= glm::toMat4(WorldRot);
	trs = glm::scale(trs, WorldScl);
	return trs;
}

Transform::Transform()
{
}

glm::mat4 Transform::LocalToWorldMatrix() const
{
	return glm::mat4();
}

glm::mat4 Transform::WorldToLocalMatrix() const
{
	return glm::mat4();
}

glm::vec3 Transform::GetPosition() const
{
	//return glm::vec3();
	return glm::vec3();
}

void Transform::SetPosition(glm::vec3 position)
{
}

glm::quat Transform::GetRotation() const
{
	return glm::quat();
}

void Transform::SetRotation(glm::quat rotation)
{
}

glm::vec3 Transform::GetForward() const
{
	return glm::vec3();
}

void Transform::SetForward(glm::vec3 forward)
{
}

glm::vec3 Transform::GetLossyScale() const
{
	return glm::vec3();
}
