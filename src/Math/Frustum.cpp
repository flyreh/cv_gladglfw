#include "../Math/Frustum.h"
#include "../Util.hpp"



void Frustum::Update(const glm::mat4& vpMatrix) {


    // Izquierdo
    m_planes[0].normal = glm::vec3(vpMatrix[0][3] + vpMatrix[0][0], vpMatrix[1][3] + vpMatrix[1][0],
        vpMatrix[2][3] + vpMatrix[2][0]);
    m_planes[0].distance = vpMatrix[3][3] + vpMatrix[3][0];

    // Derecho
    m_planes[1].normal = glm::vec3(vpMatrix[0][3] - vpMatrix[0][0], vpMatrix[1][3] - vpMatrix[1][0],
        vpMatrix[2][3] - vpMatrix[2][0]);
    m_planes[1].distance = vpMatrix[3][3] - vpMatrix[3][0];

    // Inferior
    m_planes[2].normal = glm::vec3(vpMatrix[0][3] + vpMatrix[0][1], vpMatrix[1][3] + vpMatrix[1][1],
        vpMatrix[2][3] + vpMatrix[2][1]);
    m_planes[2].distance = vpMatrix[3][3] + vpMatrix[3][1];

    // Superior
    m_planes[3].normal = glm::vec3(vpMatrix[0][3] - vpMatrix[0][1], vpMatrix[1][3] - vpMatrix[1][1],
        vpMatrix[2][3] - vpMatrix[2][1]);
    m_planes[3].distance = vpMatrix[3][3] - vpMatrix[3][1];

    // Cercano
    m_planes[4].normal = glm::vec3(vpMatrix[0][3] + vpMatrix[0][2], vpMatrix[1][3] + vpMatrix[1][2],
        vpMatrix[2][3] + vpMatrix[2][2]);
    m_planes[4].distance = vpMatrix[3][3] + vpMatrix[3][2];

    // Lejano
    m_planes[5].normal = glm::vec3(vpMatrix[0][3] - vpMatrix[0][2], vpMatrix[1][3] - vpMatrix[1][2],
        vpMatrix[2][3] - vpMatrix[2][2]);
    m_planes[5].distance = vpMatrix[3][3] - vpMatrix[3][2];

    // Normalizar los planos
    for (auto& plane : m_planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distance /= length;
    }

}

bool Frustum::IntersectsCube(const glm::vec3& cubeCenter, float halfSize) {
    for (const auto& plane : m_planes) {
        glm::vec3 positiveVertex = cubeCenter;

        positiveVertex.x += (plane.normal.x >= 0) ? halfSize : -halfSize;
        positiveVertex.y += (plane.normal.y >= 0) ? halfSize : -halfSize;
        positiveVertex.z += (plane.normal.z >= 0) ? halfSize : -halfSize;

        if (glm::dot(plane.normal, positiveVertex) + plane.distance < 0) {
            return false;
        }
    }
    return true;
}

std::array <Plane, 6> Frustum::GetPlanes() {
    return m_planes;
}