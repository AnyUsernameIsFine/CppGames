#pragma once

namespace Framework
{
	namespace Utility
	{
		template<void(*ctor)()>
		struct StaticConstructor
		{
			struct Constructor {
				Constructor() {
					ctor();
				} 
			};

			static Constructor c;
		};

		static vector<string> uniqueErrors;

		inline void outputError(const string& error, const string& file, int line, const string& expression = string())
		{
			string hash = file + std::to_string(line);
			if (std::find(uniqueErrors.begin(), uniqueErrors.end(), hash) == uniqueErrors.end()) {
				std::cerr << "\"" << error << "\" error in " << file.substr(file.find_last_of("\\/") + 1) << ":" << line << ".";

				if (!expression.empty()) {
					std::cerr << " (" << expression << ")";
				}

				std::cerr << std::endl;

				uniqueErrors.emplace_back(hash);
			}
		}

		template<typename T>
		inline T outputOpenGLError(T value, const string& file, int line, const string& expression)
		{
			GLenum e;
			while ((e = glGetError()) != GL_NO_ERROR) {
				outputError(OPEN_GL_ERRORS[e - GL_INVALID_ENUM], file, line, expression);
			}

			return value;
		}

		template<typename T>
		inline T outputSDLError(T value, const string& file, int line, const string& expression)
		{
			const char* e;
			while (std::strlen(e = SDL_GetError()) > 0) {
				outputError(e, file, line, expression);
				SDL_ClearError();
			}

			return value;
		}

		template<void(*ctor)()>
		typename StaticConstructor<ctor>::Constructor StaticConstructor<ctor>::c;

		static const string OPEN_GL_ERRORS[] = {
			"GL_INVALID_ENUM",
			"GL_INVALID_VALUE",
			"GL_INVALID_OPERATION",
			"GL_STACK_OVERFLOW",
			"GL_STACK_UNDERFLOW",
			"GL_OUT_OF_MEMORY",
			"GL_INVALID_FRAMEBUFFER_OPERATION",
		};
	}
}
