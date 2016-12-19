#include "node.hpp"
#include "transform.hpp"
#include "primitives/rectangle.hpp"
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

int Node::transformUniform = -1;

Node::Node() :
  Node("", glm::vec3(0.0f), glm::vec3(0.0f))
{}

Node::Node(const std::string& name, const glm::vec3& translation, const glm::vec3& angles) :
  isVisible(false),
  translation(translation),
  angles(angles),
  geometryOrigin(0.0f),
  size(1.0f),
  name(name),
  mChildren()
{}

Node::Node(const std::string& name, const glm::vec3& translation, const glm::vec3& angles, const glm::vec3& geometryOrigin, const glm::vec3& size) :
  isVisible(true),
  translation(translation),
  angles(angles),
  geometryOrigin(geometryOrigin),
  size(size),
  name(name),
  mChildren()
{}

void Node::render(glm::mat4 transform) const
{
  transform = transform * ogl::translate(translation);

  if (angles.z != 0.0f)
    transform = transform * ogl::rotateZ(angles.z);
  if (angles.y != 0.0f)
    transform = transform * ogl::rotateY(angles.y);
  if (angles.x != 0.0f)
    transform = transform * ogl::rotateX(angles.x);

  if (isVisible) {
    // Scale and model origin are not propagated to children
    glm::mat4 localTransform(transform);

    localTransform = localTransform * ogl::translate(geometryOrigin);
    // Divide by the initial size of the rectange (2.0) to get the actual requested size
    localTransform = localTransform * ogl::scale(size / 2.0f);

    glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(localTransform));
    glDrawElements(GL_TRIANGLES, rectangle::numIndices, GL_UNSIGNED_SHORT, 0);
  }

  for (const Node& child : mChildren) {
    child.render(transform);
  }
}

Node& Node::addChild(const Node& child)
{
  mChildren.push_back(child);
  return mChildren.back();
}

Node* Node::find(const std::string& nodeName)
{
  if (name == nodeName) {
    return this;
  }
  // Depth-first search
  for (Node& child : mChildren) {
    Node* node = child.find(nodeName);
    if (node) {
      return node;
    }
  }

  return nullptr;
}
