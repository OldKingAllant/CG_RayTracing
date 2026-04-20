#pragma once //hope this works on other compilers

#include <filesystem>
#include <vector>
#include <utility>

namespace cg_raytracing {
	namespace fs = std::filesystem;

	class Shader {
	public :
		/// <summary>
		/// Enum of shader stages
		/// </summary>
		enum class ShaderStage {
			VERTEX,
			GEOMETRY,
			FRAGMENT,
			COMPUTE
		};

		/// <summary>
		/// Construct from r-value reference 
		/// by move constructor. After this
		/// call, _prev no longer contains 
		/// a valid reference to a shader
		/// </summary>
		/// <param name="_prev">The shader to move</param>
		Shader(Shader&& _prev);

		/// <summary>
		/// Compile and link a shader from the specified source files,
		/// with the given stages
		/// </summary>
		/// <param name="_stages">Vector of pairs of (path, stage_id)</param>
		/// <returns>Shader handle</returns>
		Shader CreateShaderFromFiles(std::vector<std::pair<fs::path, ShaderStage>> const& _stages);


		/// <summary>
		/// Compile and link a shader from the given buffers,
		/// with the given stages
		/// </summary>
		/// <param name="_stages">Vector of pairs of (shader source, stage_id)</param>
		/// <returns>Shader handle</returns>
		Shader CreateShaderFromBuffers(std::vector<std::pair<std::string, ShaderStage>> const& _stages);

		~Shader();

	private :
		// Allow only the implementation to create an empty shader
		Shader() = default;
		// Delete copy constructor
		Shader(Shader const& other) = delete;
	};
}