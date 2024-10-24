#pragma once

#include "../Enviroment/CV_Enviroment.h"

namespace GLBackVertex {
	

	//Para Cubo, por ahora
	void UploadCubeVertexData(std::vector<float>);

	GLuint GetCubeVAO();

	GLuint GetCubeVBO();
}