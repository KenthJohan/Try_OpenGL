#pragma once
#include <GL/glew.h>
#include "debug.h"
#include "ANSIC.h"

#define GL_CHECK_ERROR gl_check_error (__COUNTER__, __RELATIVE_FILE__, __LINE__, __func__)


static char const * 
gl_getstr_source (GLenum source)
{
	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             return "Source: API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Source: Window System";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Source: Shader Compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Source: Third Party";
		case GL_DEBUG_SOURCE_APPLICATION:     return "Source: Application";
		case GL_DEBUG_SOURCE_OTHER:           return "Source: Other";
		default: return "Source: Unknown";
	}
}


static char const * 
gl_getstr_type (GLenum type)
{
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               return "Type: Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Type: Deprecated Behaviour";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Type: Undefined Behaviour"; 
		case GL_DEBUG_TYPE_PORTABILITY:         return "Type: Portability";
		case GL_DEBUG_TYPE_PERFORMANCE:         return "Type: Performance";
		case GL_DEBUG_TYPE_MARKER:              return "Type: Marker";
		case GL_DEBUG_TYPE_PUSH_GROUP:          return "Type: Push Group";
		case GL_DEBUG_TYPE_POP_GROUP:           return "Type: Pop Group";
		case GL_DEBUG_TYPE_OTHER:               return "Type: Other";
		default: return "Type: Unknown";
	}
}


static char const * 
gl_getstr_severity (GLenum severity)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         return "Severity: high";
		case GL_DEBUG_SEVERITY_MEDIUM:       return "Severity: medium";
		case GL_DEBUG_SEVERITY_LOW:          return "Severity: low";
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "Severity: notification";
		default: return "Severity: Unknown";
	}
}


static char const * 
gl_getstr_code (GLenum code)
{
	switch (code)
	{
		case GL_NO_ERROR:                      return "GL_NO_ERROR";
		case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
		default: return "Unknown error";
	}
}


__attribute__ ((__unused__)) static void gl_check_error 
(int id, char const * file, int line, char const * fn)
{
	GLenum code = glGetError();
	if (code == GL_NO_ERROR) {return;}
	while (1)
	{
		fprintf	
		(
			stderr,
			ANSIC (ANSIC_BOLD, ANSIC_RED, ANSIC_DEFAULT) "GL_CHECK_ERROR" ANSIC_RESET " "
			"%04i "
			ANSIC (ANSIC_BOLD, ANSIC_BLUE, ANSIC_DEFAULT) "%s" ANSIC_RESET ":"
			ANSIC (ANSIC_BOLD, ANSIC_BLUE, ANSIC_DEFAULT) "%04i" ANSIC_RESET " in "
			ANSIC (ANSIC_NORMAL, ANSIC_YELLOW , ANSIC_DEFAULT) "%s" ANSIC_RESET " () " 
			ANSIC (ANSIC_BOLD, ANSIC_RED , ANSIC_DEFAULT) "%s" ANSIC_RESET " (%04i)",
			id, 
			file, 
			line, 
			fn, 
			gl_getstr_code (code),
			code
		);
		fprintf (stderr, "\n");
		code = glGetError();
		if (code == GL_NO_ERROR) {break;}
	}
	abort ();
}


void GLAPIENTRY glDebugOutput
(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, void const * arg)
{
	//ignore non-significant error/warning codes
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) {return;}
	fprintf (stderr, "Debug message (%i) : %s\n", id, message);
	fprintf (stderr, "%s\n", gl_getstr_source (source));
	fprintf (stderr, "%s\n", gl_getstr_type (type));
	fprintf (stderr, "%s\n", gl_getstr_severity (severity));
}


