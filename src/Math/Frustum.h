#pragma once 
#include "../Enviroment/CV_Enviroment.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "../Enviroment/RendererEnviroment.h"

struct Plane {
    glm::vec3 normal;
    float distance;
};

struct Frustum {

public:
    void Update(const glm::mat4& vpMatrix);
   // void UpdateFromTile(const glm::mat4& viewMatrix, float fov, float nearPlane, float farPlane, int x1, int y1, int x2, int y2, int viewportWidth, int viewportHeight);
   // bool IntersectsAABB(const AABB& aabb);
   //// bool IntersectsAABB(const RenderItem3D& renderItem);
   // bool IntersectsAABBFast(const AABB& aabb);
   // bool IntersectsAABBFast(const RenderItem3D& renderItem);
   // bool IntersectsSphere(const Sphere& sphere);
   // bool IntersectsPoint(const glm::vec3 point);
    bool IntersectsCube(const glm::vec3& cubeCenter, float halfSize);
//std::vector<glm::vec3> GetFrustumCorners();

    std::array<Plane, 6> GetPlanes();

private:
 //   Plane CreatePlane(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
  //  float SignedDistance(const glm::vec3& point, const Plane& plane) const;
  //  static glm::vec3 IntersectPlanes(const glm::vec3& n1, float d1, const glm::vec3& n2, float d2, const glm::vec3& n3, float d3);

	std::array<Plane, 6> m_planes;
};