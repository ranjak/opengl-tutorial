#ifndef NODE_HPP
#define NODE_HPP

#include <glm/vec3.hpp>
#include <glm/fwd.hpp>
#include <vector>
#include <string>

struct Node
{
  // Make an invisible node
  Node();
  Node(const std::string& name, const glm::vec3& translation, const glm::vec3& angles);
  // Make a drawable node
  Node(const std::string& name, const glm::vec3& translation, const glm::vec3& angles, const glm::vec3& geometryOrigin, const glm::vec3& size);

  static int transformUniform;

  void render(glm::mat4 transform) const;

  Node& addChild(const Node& child);

  Node* find(const std::string& nodeName);

  bool isVisible;
  glm::vec3 translation;
  glm::vec3 angles;
  glm::vec3 geometryOrigin;
  glm::vec3 size;
  std::string name;

private:
  std::vector<Node> mChildren;
};

#endif // NODE_HPP
