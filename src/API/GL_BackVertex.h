#pragma once

#include "../Enviroment/CV_Enviroment.h"

namespace GLBackVertex {
	

	//Para Cubo, por ahora
	void UploadCubeVertexData(std::vector<float>);

	void UploadCubeMapVertexData(std::vector<float>&, std::vector<unsigned int>&);

	GLuint GetCubeVAO();

	GLuint GetCubeVBO();

	GLuint GetCubeMapVAO();

	GLuint GetCubeMapVBO();


}