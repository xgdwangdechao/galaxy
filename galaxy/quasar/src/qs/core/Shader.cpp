///
/// Shader.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include "Shader.hpp"

///
/// Core namespace.
///
namespace qs
{
	Shader::Shader() noexcept
	    : m_id {0}
	{
	}

	Shader::Shader(std::string_view vertex_file, std::string_view frag_file)
	    : m_id {0}
	{
		if (!load_path(vertex_file, frag_file))
		{
			PL_LOG(PL_ERROR, "Failed to construct shader with files: {0} | {1}.", vertex_file, frag_file);
		}
	}

	Shader::~Shader() noexcept
	{
		unbind();
		glDeleteProgram(m_id);
	}

	bool Shader::load_path(std::string_view vertex_file, std::string_view frag_file)
	{
		bool result = true;

		// Convert to proper paths and:
		// Create an input stream of the file from disk in read only mode.
		std::ifstream v_stream(std::filesystem::path(vertex_file).string(), std::ios::in);
		std::ifstream f_stream(std::filesystem::path(frag_file).string(), std::ios::in);

		// Check for errors...
		if (!v_stream)
		{
			PL_LOG(PL_ERROR, "std::ifstream failed to open file: {0}.", vertex_file);
			result = false;
		}

		if (!f_stream)
		{
			PL_LOG(PL_ERROR, "std::ifstream failed to open file: {0}.", frag_file);
			result = false;
		}

		// If no errors occured, continue loading shader...
		if (result)
		{
			// Then read entire input buffer in to a string stream object.
			//  You can't read it directly into a std::string in a method that is resonably performant.
			std::stringstream v_buffer;
			std::stringstream f_buffer;
			v_buffer << v_stream.rdbuf();
			f_buffer << f_buffer.rdbuf();

			if (!v_buffer)
			{
				PL_LOG(PL_ERROR, "std::stringstream failed to read vertexBuffer for: {0}.", vertex_file);
				result = false;
			}

			if (!f_buffer)
			{
				PL_LOG(PL_ERROR, "std::stringstream failed to read fragmentBuffer for: {0}.", frag_file);
				result = false;
			}

			// Once again, if no errors, continue loading...
			if (result)
			{
				// Error reporting for OpenGL.
				char info[1024];
				int success       = 0;
				unsigned int v_id = 0;
				unsigned int f_id = 0;

				// Then we need to convert the stream to a c string because OpenGL requires a refernece to a c string. yeah.
				const char* v_src = v_buffer.str().c_str();
				const char* f_src = f_buffer.str().c_str();

				// Retrieve the ids from opengl when creating the shader, then compile shaders, while checking for errors.
				v_id = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(v_id, 1, &v_src, nullptr);
				glCompileShader(v_id);

				glGetShaderiv(v_id, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(v_id, 1024, nullptr, info);

					PL_LOG(PL_ERROR, "Failed to vertex compile shader. {0}.", info);
					result = false;
				}

				// Now do the same for the fragment shader.
				f_id = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(f_id, 1, &f_src, nullptr);
				glCompileShader(f_id);

				glGetShaderiv(f_id, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(f_id, 1024, nullptr, info);

					PL_LOG(PL_ERROR, "Failed to compile fragment shader. {0}.", info);
					result = false;
				}

				// Hopefully by this point nothing has gone wrong...
				if (result)
				{
					// Create and link program.
					m_id = glCreateProgram();
					glAttachShader(m_id, v_id);
					glAttachShader(m_id, f_id);
					glLinkProgram(m_id);

					glGetProgramiv(m_id, GL_LINK_STATUS, &success);
					if (!success)
					{
						glGetProgramInfoLog(m_id, 1024, nullptr, info);

						PL_LOG(PL_ERROR, "Failed to attach shaders. {0}.", info);
						result = false;
					}
				}

				// Cleanup shaders.
				glDeleteShader(v_id);
				glDeleteShader(f_id);
			}
		}

		// Close the input stream since we are done.
		v_stream.close();
		f_stream.close();

		return result;
	}

	bool Shader::load_raw(const std::string& vertex_str, const std::string& fragment_str)
	{
		// Set up vars.
		bool result = true;

		if (vertex_str.empty())
		{
			PL_LOG(PL_ERROR, "Vertex shader source was empty.");
			result = false;
		}

		if (fragment_str.empty())
		{
			PL_LOG(PL_ERROR, "Fragment shader source was empty.");
			result = false;
		}

		if (result)
		{
			// OpenGL vars.
			int success = 0;
			char info[1024];
			unsigned int v_id = 0;
			unsigned int f_id = 0;

			// Then we need to convert the stream to a c string because OpenGL requires a refernece to a c string. yeah.
			const char* v_src = vertex_str.c_str();
			const char* f_src = fragment_str.c_str();

			// Retrieve the ids from opengl when creating the shader, then compile shaders, while checking for errors.
			v_id = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(v_id, 1, &v_src, nullptr);
			glCompileShader(v_id);

			glGetShaderiv(v_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(v_id, 1024, nullptr, info);

				PL_LOG(PL_ERROR, "Failed to vertex compile shader. {0}.", info);
				result = false;
			}

			// Now do the same for the fragment shader.
			f_id = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(f_id, 1, &f_src, nullptr);
			glCompileShader(f_id);

			glGetShaderiv(f_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(f_id, 1024, nullptr, info);

				PL_LOG(PL_ERROR, "Failed to compile fragment shader. {0}.", info);
				result = false;
			}

			// Hopefully by this point nothing has gone wrong...
			if (result)
			{
				// Create and link program.
				m_id = glCreateProgram();
				glAttachShader(m_id, v_id);
				glAttachShader(m_id, f_id);
				glLinkProgram(m_id);

				glGetProgramiv(m_id, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(m_id, 1024, nullptr, info);

					PL_LOG(PL_ERROR, "Failed to attach shaders. {0}.", info);
					result = false;
				}
			}

			// Cleanup shaders.
			glDeleteShader(v_id);
			glDeleteShader(f_id);
		}

		return result;
	}

	void Shader::bind() noexcept
	{
		glUseProgram(m_id);
	}

	void Shader::unbind() noexcept
	{
		glUseProgram(0);
	}

	int Shader::get_uniform_location(std::string_view name) noexcept
	{
		auto str = static_cast<std::string>(name);

		// If uniform already exists return it from cache to avoid querying OpenGL, which is slow.
		if (m_cache.contains(str))
		{
			return m_cache[str];
		}
		else
		{
			// Otherwise, for the first time, retrieve location.
			auto location = glGetUniformLocation(m_id, str.c_str());
			if (location != -1)
			{
				// Then if not error, add to hash map.
				m_cache.emplace(name, location);
			}
			else
			{
				PL_LOG(PL_WARNING, "Failed to find uniform: {0}.", name);
			}

			return location;
		}
	}
} // namespace qs