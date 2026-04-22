#include <VertexBuffer.hpp>
#include <Utility.hpp>

#include <GL/glew.h>
#include <typeinfo>

namespace cg_raytracing {
	namespace detail {
		std::expected<uint32_t, GLError> CreateVertexArrayObject() {
			uint32_t vao{};
			glGenVertexArrays(1, &vao);

			if (0 == vao) {
				auto maybe_err = ConvertGLErrorToEnum(glGetError());
				return std::unexpected{ maybe_err };
			}

			return vao;
		}

		void DestroyVertexArrayObject(uint32_t _vao) {
			glDeleteVertexArrays(1, &_vao);
		}

		void SetCurrentArrayBuffer(uint32_t _buf) {
			auto ctx = GetCurrentGLContext();
			ctx->BindBuffer(GL_ARRAY_BUFFER, _buf);
		}

		void SetVertexBufferLabel(uint32_t _vao, std::string const& _label) {
			glObjectLabel(GL_VERTEX_ARRAY, _vao, _label.size(), _label.c_str());
		}

		/// <summary>
		/// Convert our vertex attribute type representation 
		/// to OpenGL
		/// </summary>
		/// <param name="_attr">Attribute type</param>
		/// <returns>OpenGL type</returns>
		static uint32_t VertexAttributeToOpenGL(VertexAttributeType _attr) {
			switch (_attr)
			{
			case cg_raytracing::VertexAttributeType::BYTE:
				return GL_BYTE;
			case cg_raytracing::VertexAttributeType::UNSIGNED_BYTE:
				return GL_UNSIGNED_BYTE;
			case cg_raytracing::VertexAttributeType::SHORT:
				return GL_SHORT;
			case cg_raytracing::VertexAttributeType::UNSIGNED_SHORT:
				return GL_UNSIGNED_SHORT;
			case cg_raytracing::VertexAttributeType::INT:
				return GL_INT;
			case cg_raytracing::VertexAttributeType::UNSIGNED_INT:
				return GL_UNSIGNED_INT;
			case cg_raytracing::VertexAttributeType::FLOAT:
				return GL_FLOAT;
			case cg_raytracing::VertexAttributeType::DOUBLE:
				return GL_DOUBLE;
			case cg_raytracing::VertexAttributeType::VEC2:
			case cg_raytracing::VertexAttributeType::VEC3:
			case cg_raytracing::VertexAttributeType::VEC4:
				return GL_FLOAT;
			case cg_raytracing::VertexAttributeType::UVEC2:
			case cg_raytracing::VertexAttributeType::UVEC3:
			case cg_raytracing::VertexAttributeType::UVEC4:
				return GL_UNSIGNED_INT;
			case cg_raytracing::VertexAttributeType::IVEC2:
			case cg_raytracing::VertexAttributeType::IVEC3:
			case cg_raytracing::VertexAttributeType::IVEC4:
				return GL_INT;
			default:
				common::Unreachable();
				break;
			}
			
			common::Unreachable();
			return GL_FLOAT;
		}

		uint32_t SetVertexAttributes(std::vector<VertexAttribute> const& _attributes, 
			uint32_t _instance_step_count,
			uint32_t _vert_size_bytes,
			uint32_t _base_index) {
			uint32_t index{_base_index};

			for (auto const& attribute : _attributes) {
				auto type = VertexAttributeToOpenGL(attribute.type);
				auto offset = (const void*)attribute.offset;
				auto size = VERTEX_ATTRIB_NUM_ELEMENTS[(uint32_t)attribute.type];

				switch (type)
				{
				case GL_FLOAT:
					glVertexAttribPointer(index, size, type, GL_FALSE,
						_vert_size_bytes, offset);
					break;
				case GL_DOUBLE:
					glVertexAttribLPointer(index, size, type,
						_vert_size_bytes, offset);
					break;
				default:
					glVertexAttribIPointer(index, size, type,
						_vert_size_bytes, offset);
					break;
				}

				if (0 != _instance_step_count) {
					glVertexAttribDivisor(index, _instance_step_count);
				}

				glEnableVertexAttribArray(index);
				index++;
			}

			return index;
		}
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& _prev) noexcept :
		m_buf_sizes{ _prev.m_buf_sizes },
		m_vao{ _prev.m_vao },
		m_buffers{ std::move(_prev.m_buffers) },
		m_attrib_types{ std::move(_prev.m_attrib_types) },
		m_attrib_sizes{ std::move(_prev.m_attrib_sizes) },
		m_curr_attrib_index{_prev.m_curr_attrib_index} {
		_prev.m_vao = 0;
		_prev.m_buf_sizes = {};
		_prev.m_buf_sizes = {};
		_prev.m_attrib_types = {};
		_prev.m_curr_attrib_index = 0;
		_prev.m_attrib_sizes = {};
	}

	VertexBuffer::VertexBuffer() :
		m_buf_sizes{},
		m_vao{},
		m_buffers{},
		m_attrib_types{},
		m_attrib_sizes{},
		m_curr_attrib_index{} {}

	std::expected<VertexBuffer, GLError> VertexBuffer::CreateVertexBuffer() {
		auto maybe_vao = detail::CreateVertexArrayObject();
		if (!maybe_vao.has_value()) {
			return std::unexpected{ maybe_vao.error() };
		}

		auto vao = maybe_vao.value();
		auto vert_buf = VertexBuffer();
		vert_buf.m_vao = vao;
		return vert_buf;
	}

	std::optional<GLError> VertexBuffer::AddBufferImpl(
		std::type_info const* _attrib_type,
		size_t _attrib_size,
		std::vector<VertexAttribute> _attribs, 
		uint32_t _attrib_divisor, 
		size_t _init_size) {
		auto ctx = GetCurrentGLContext();
		Bind();

		auto buf_temp = GpuBuffer::CreateBuffer(_init_size * _attrib_size, 
			BufferMapping::WRITE | 
			BufferMapping::PERSISTENT | 
			BufferMapping::COHERENT |
			BufferMapping::DYNAMIC
		);
		if (!buf_temp.has_value()) {
			return buf_temp.error();
		}
		m_buffers.push_back( std::move(buf_temp.value()) );
		detail::SetCurrentArrayBuffer(m_buffers.back().GetBufferId());

		m_curr_attrib_index = detail::SetVertexAttributes(_attribs, _attrib_divisor, (uint32_t)_attrib_size, 
			m_curr_attrib_index);

		detail::SetCurrentArrayBuffer(0);
		ctx->BindVao(0);

		auto maybe_err = m_buffers.back().MapBuffer(0, m_buffers.back().GetBufferSize(),
			BufferMapping::WRITE | BufferMapping::PERSISTENT | BufferMapping::COHERENT);

		m_buf_sizes.push_back(0);
		m_attrib_types.push_back(_attrib_type);
		m_attrib_sizes.push_back(_attrib_size);
		
		return std::nullopt;
	}

	std::optional<GLError> VertexBuffer::PushVertexDataTypedImpl(
		size_t _buf_index,
		std::type_info const* _type,
		const void* _data,
		size_t _size_bytes
	) {
		if (_buf_index > m_buffers.size()) {
			return GLError::PUSH_FAILED_INVALID_BUFFER;
		}
		auto stored_type = m_attrib_types[_buf_index];
		auto stored_type_size = m_attrib_sizes[_buf_index];

		if (*stored_type != *_type) {
			return GLError::PUSH_FAILED_TYPE_MISMATCH;
		}

		if (0 != _size_bytes % stored_type_size) {
			return GLError::PUSH_FAILED_SIZE_MISMATCH;
		}

		return PushVertexDataUnsafe(_buf_index, _data, _size_bytes);
	}

	std::optional<GLError> VertexBuffer::PushVertexDataUnsafe(size_t _buf_index, const void* _data, size_t _size_bytes) {
		auto& buf = m_buffers[_buf_index];
		auto type_size = m_attrib_sizes[_buf_index];
		auto curr_byte_offset = m_buf_sizes[_buf_index] * type_size;
		auto last_byte_offset = curr_byte_offset + _size_bytes;
		if (buf.GetBufferSize() < last_byte_offset) {
			auto map_prots = buf.GetMapProts().value();
			auto buf_prots = buf.GetBufferProts();

			/// To prevent leaving the vertex buffer in an undefined
			/// state if the remapping of the buffer fails, we create
			/// a new temporary buffer and map it to host memory
			/// If the mapping fails, the new buffer will be destroied
			/// leaving the vertex buffer in a well-defined state,
			/// otherwise, if everyting goes as expected, the
			/// old buffer is replaced 

			auto new_size = std::max(last_byte_offset, buf.GetBufferSize() * 2);
			auto maybe_buf = GpuBuffer::CreateBuffer(new_size, buf_prots);
			if (!maybe_buf.has_value()) {
				return maybe_buf.error();
			}

			auto temp_buf = std::move(maybe_buf.value());
			auto maybe_err = temp_buf.MapBuffer(0, temp_buf.GetBufferSize(), map_prots);
			if (maybe_err.has_value()) {
				/// Error occurred during mapping, new buffer is destroied
				/// and the state of the vertex buffer is well defined
				return maybe_err;
			}

			maybe_err = temp_buf.CopyFromBufferRange(buf, 0, 0, buf.GetBufferSize());
			if (maybe_err.has_value()) {
				return maybe_err;
			}

			buf = std::move(temp_buf);
		}
		auto maybe_ptr = buf.GetHostBuffer();
		auto buf_ptr = maybe_ptr.value();
		std::copy_n((const uint8_t*)_data, _size_bytes, (uint8_t*)(buf_ptr) + curr_byte_offset);
		m_buf_sizes[_buf_index] += _size_bytes / type_size;
		return std::nullopt;
	}

	void VertexBuffer::Bind() const {
		auto ctx = GetCurrentGLContext();
		ctx->BindVao(m_vao);
	}

	VertexBuffer::~VertexBuffer() {
		if (0 == m_vao) {
			return;
		}
		detail::DestroyVertexArrayObject(m_vao);
		m_vao = 0;
	}
}