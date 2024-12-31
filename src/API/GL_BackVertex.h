#pragma once

#include "../Enviroment/CV_Enviroment.h"

namespace GLBackVertex {
	

	//Para Cubo, por ahora
	void UploadCubeVertexData(std::vector<Vertex>&);

	void UploadCubeMapVertexData(const float (&skyboxVertices)[24],const unsigned int (&skyboxIndices)[36] );

	void UploadVertexLight(std::vector<Vertex>);

	GLuint GetCubeLightVAO();

	GLuint GetCubeVAO();

	GLuint GetCubeVBO();

	GLuint GetCubeMapVAO();

	GLuint GetCubeMapVBO();


}