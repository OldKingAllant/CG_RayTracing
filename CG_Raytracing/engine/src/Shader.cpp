#include <Shader.hpp>

#include <GL/glew.h>

namespace cg_raytracing {
	using ShaderStage = Shader::ShaderStage;

	static GLuint GetOpengGLShaderStageFromEnum(ShaderStage _stage) {
		switch (_stage)
		{
		case ShaderStage::VERTEX:
			return GL_VERTEX_SHADER;
		case ShaderStage::GEOMETRY:
			return GL_GEOMETRY_SHADER;
		case ShaderStage::FRAGMENT:
			return GL_FRAGMENT_SHADER;
		case ShaderStage::COMPUTE:
			return GL_COMPUTE_SHADER;
		default:
			break;
		}
	}
}