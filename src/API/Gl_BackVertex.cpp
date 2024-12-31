#include "../Enviroment/CV_Enviroment.h"
#include "GL_BackVertex.h"
#include <iostream>
#include <string>

namespace GLBackVertex {

    GLuint _vertexDataVAO = 0;
    GLuint _vertexDataVBO = 0;
    GLuint _vertexDataEBO = 0;

    GLuint _weightedVertexDataVAO = 0;
    GLuint _weightedVertexDataVBO = 0;
    GLuint _weightedVertexDataEBO = 0;

    GLuint g_skinnedVertexDataVAO = 0;
    GLuint g_skinnedVertexDataVBO = 0;
    GLuint g_allocatedSkinnedVertexBufferSize = 0;

    GLuint g_pointCloudVAO = 0;
    GLuint g_pointCloudVBO = 0;

    GLuint g_constructiveSolidGeometryVAO = 0;
    GLuint g_constructiveSolidGeometryVBO = 0;
    GLuint g_constructiveSolidGeometryEBO = 0;

    GLuint g_triangle2DVAO = 0;
    GLuint g_triangle2DVBO = 0;

    GLuint g_cubeVAO = 0;
    GLuint g_cubeVBO = 0;

    GLuint g_cubeMapVAO = 0;
    GLuint g_cubeMapVBO = 0;
    GLuint g_cubeMapEBO = 0;

    GLuint g_cubeLightVAO = 0;
	GLuint g_cubeLightVBO = 0;



    GLuint GetVertexDataVAO() {
        return _vertexDataVAO;
    }

    GLuint GetVertexDataVBO() {
        return _vertexDataVBO;
    }

    GLuint GetVertexDataEBO() {
        return _vertexDataEBO;
    }

    GLuint GetWeightedVertexDataVAO() {
        return _weightedVertexDataVAO;
    }

    GLuint GetWeightedVertexDataVBO() {
        return _weightedVertexDataVBO;
    }

    GLuint GetWeightedVertexDataEBO() {
        return _weightedVertexDataEBO;
    }

    GLuint GetSkinnedVertexDataVAO() {
        return g_skinnedVertexDataVAO;
    }

    GLuint GetSkinnedVertexDataVBO() {
        return g_skinnedVertexDataVBO;
    }

    GLuint GetPointCloudVAO() {
        return g_pointCloudVAO;
    }

    GLuint GetPointCloudVBO() {
        return g_pointCloudVBO;
    }

    GLuint GetCSGVAO() {
        return g_constructiveSolidGeometryVAO;
    }

    GLuint GetCSGVBO() {
        return g_constructiveSolidGeometryVBO;
    }

    GLuint GetCSGEBO() {
        return g_constructiveSolidGeometryEBO;
    }
	GLuint GetCubeLightVAO() {
		return g_cubeLightVAO;
	}

	GLuint GetCubeLightVBO() {
		return g_cubeLightVBO;
	}

	GLuint GetCubeVAO() {
		return g_cubeVAO;
	}

	GLuint GetCubeVBO() {
		return g_cubeVBO;
	}

    GLuint GetCubeMapVAO() {
      return g_cubeMapVAO;
    }

    GLuint GetCubeMapVBO() {
      return g_cubeMapVBO;
    }

    GLuint GetCubeMapEBO() {
      return g_cubeMapEBO;
    }


}

void GLBackVertex::UploadCubeVertexData(std::vector<Vertex>& vertices) {
    static int allocatedBufferSize = 0;
    if (g_cubeVAO == 0) {
        glGenVertexArrays(1, &g_cubeVAO);
        glGenBuffers(1, &g_cubeVBO);
    }
    if (vertices.empty()) {
        return;
    }
    glBindVertexArray(g_cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_cubeVBO);

    // Gestiona el tamaño del buffer, reutilizando memoria si es posible
    if (vertices.size() * sizeof(Vertex) <= allocatedBufferSize) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    }
    else {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        allocatedBufferSize = vertices.size() * sizeof(Vertex);
    }

    // Configura los atributos de los vértices basados en la estructura Vertex
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Texture coordinates (UV)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    // Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    // Desenlaza el VAO y el buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



  // Desvincular el VAO
  //glBindVertexArray(0);

    /*static int allocatedBufferSize = 0;
    if (g_cubeVAO == 0) {
        glGenVertexArrays(1, & g_cubeVAO);
        glGenBuffers(1, &g_cubeVBO);
    }
    if (vertices.empty()) {
        return;
    }
    glBindVertexArray(g_cubeVAO);
    if (vertices.size() * sizeof(glm::vec2) <= allocatedBufferSize) {
        glBindBuffer(GL_ARRAY_BUFFER, g_cubeVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (GLuint)vertices.size() * sizeof(glm::vec2), &vertices[0]);
    }
    else {
        glDeleteBuffers(1, &g_cubeVBO);
        glGenBuffers(1, &g_cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, g_cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    allocatedBufferSize = vertices.size() * sizeof(glm::vec2);*/

}

void GLBackVertex::UploadVertexLight(std::vector<Vertex> vertices) {
    static int allocatedBufferSize = 0;
    if (g_cubeLightVAO == 0) {
        glGenVertexArrays(1, &g_cubeLightVAO);
        glGenBuffers(1, &g_cubeLightVBO);
    }
    if (vertices.empty()) {
        return;
    }
    glBindVertexArray(g_cubeLightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_cubeLightVBO);

    if (vertices.size() * sizeof(Vertex) <= allocatedBufferSize) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    }
    else {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        allocatedBufferSize = vertices.size() * sizeof(Vertex);
    }

    // Configura los atributos de los vértices
    // Posición de los vértices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Texture coordinates (UV)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    // Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void GLBackVertex::UploadCubeMapVertexData(const float (&skyboxVertices)[24], const unsigned int (&skyboxIndices)[36]) {


  glGenVertexArrays(1, &g_cubeMapVAO);
  glGenBuffers(1, &g_cubeMapVBO);
  glGenBuffers(1, &g_cubeMapEBO);
  glBindVertexArray(g_cubeMapVAO);
  glBindBuffer(GL_ARRAY_BUFFER, g_cubeMapVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_cubeMapEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}




