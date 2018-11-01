#include "app.h"

GLuint setup_verts (struct GUI_Vertex const v [], size_t vn, unsigned int const e [], size_t en)
{
	size_t const v8 = vn * sizeof (struct GUI_Vertex);
	size_t const e8 = en * sizeof (unsigned int);
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	glGenVertexArrays (1, &vao);
	glGenBuffers (1, &vbo);
	glGenBuffers (1, &ebo);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, v8, v, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, e8, e, GL_STATIC_DRAW);
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (struct GUI_Vertex), (void*)0);
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, sizeof (struct GUI_Vertex), (void*) (2 * sizeof (float)));
	glEnableVertexAttribArray (1);
	return vao;
}


GLuint mesh_create_test1 ()
{
	struct GUI_Vertex v [] =
	{
		{{ 0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{ 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
		{{-0.5f,  0.5f}, {0.5f, 0.0f, 0.5f, 1.0f}}
	};
	
	unsigned int e [] = 
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	return setup_verts (v, 4, e, 6);
}



int main (int argc, char *argv[])
{
	struct Application app;
	app_init (&app, argc, argv);
	
	GLuint vao1 = mesh_create_test1 ();

	
	while (1)
	{
		if (app.flags & APP_QUIT) {break;}
		if (app.flags & APP_OPENGL_ERROR) {break;}
		app_frame_begin (&app);
		
		while (SDL_PollEvent (&app.event))
		{
			app_handle_input (&app);
			switch (app.event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				break;
			}
		}
		
		glBindVertexArray (vao1);
		glDrawElements (GL_LINES, 6, GL_UNSIGNED_INT, 0);
		app_frame_end (&app);
	}

	app_destroy (&app);
	return 0;
}











