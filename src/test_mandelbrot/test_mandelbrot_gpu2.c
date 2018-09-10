#include "app.h"



struct Vertex
{
	float p [2];
	float c [4];
};


struct Mandelbrot_Controller
{
	GLint program;
	GLint u_iterations;
	GLint u_translate;
	GLint u_scale;
	float t [2];
	float s;
	float d;
};


void mbc_init (struct Mandelbrot_Controller * mbc)
{
	struct Shader shaders [2] = 
	{
		{0, GL_VERTEX_SHADER, "src/test_mandelbrot/mandelbrot.glvs"},
		{0, GL_FRAGMENT_SHADER, "src/test_mandelbrot/mandelbrot.glfs"}
	};
	mbc->program = program_create ("SimpleShaderProgram1", shaders, COUNTOF (shaders));
	glUseProgram (mbc->program);
	mbc->u_iterations = glGetUniformLocation (mbc->program, "u_iterations");
	mbc->u_translate = glGetUniformLocation (mbc->program, "u_translate");
	mbc->u_scale = glGetUniformLocation (mbc->program, "u_scale");
	ASSERT (mbc->u_iterations >= 0);
	ASSERT (mbc->u_translate >= 0);
	ASSERT (mbc->u_scale >= 0);
	
	mbc->t [0] = 0.0f;
	mbc->t [1] = 0.0f;
	mbc->s = 1.0f;
	mbc->d = 0.1f;
	glProgramUniform1f (mbc->program, mbc->u_iterations, 2000);
}


void mbc_update (struct Mandelbrot_Controller * mbc, SDL_KeyboardEvent * key)
{
	float s = 0.0;
	float t [2] = {0.0f, 0.0f};
	
	switch (key->keysym.sym)
	{
		case SDLK_KP_6:
		t [0] = 1.0f;
		break;
		case SDLK_KP_4:
		t [0] = -1.0f;
		break;
		case SDLK_KP_8:
		t [1] = 1.0f;
		break;
		case SDLK_KP_2:
		t [1] = -1.0f;
		break;
		
		case SDLK_KP_PLUS:
		s = 1.0f;
		break;
		case SDLK_KP_MINUS:
		s = -1.0f;
		break;
	}
	s *= 0.1f;
	mbc->s += s;
	float power_scale = SDL_pow (10, mbc->s);
	mbc->t [0] += t [0] * (power_scale) * 0.1f;
	mbc->t [1] += t [1] * (power_scale) * 0.1f;
	
	
	
	//float t [2];
	//vf32_mul_scalar (t, mbc->t, s, 2);
	printf ("ps: %f\n", power_scale);
	vf32_print (stdout, mbc->t, 2, "%f ");
	glProgramUniform2f (mbc->program, mbc->u_translate, mbc->t [0], mbc->t [1]);
	glProgramUniform1f (mbc->program, mbc->u_scale, power_scale);
}


GLuint setup_verts (struct Vertex v [], size_t vn, unsigned int e [], size_t en)
{
	size_t const v8 = vn * sizeof (struct Vertex);
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
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void*)0);
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (void*) (2 * sizeof (float)));
	glEnableVertexAttribArray (1);
	return vao;
}





GLuint create_mesh_quad ()
{
	struct Vertex v [] =
	{
		{{ 1.0f,  1.0f}}, // top right
		{{ 1.0f, -1.0f}}, // bottom right
		{{-1.0f, -1.0f}}, // bottom left
		{{-1.0f,  1.0f}}, // top left 
	};
	for (size_t i = 0; i < 4; ++ i)
	{
		//vf32_mul_scalar (v [i].p, v [i].p, 1.0f/2.0f, 2);
	}
	unsigned int e [] = 
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	GLuint vbo = setup_verts (v, 4, e, 6);
	return vbo;
}






int main (int argc, char *argv[])
{
	struct Application app;
	app_init (&app, argc, argv);
	
	GLuint vao = create_mesh_quad ();
	
	struct Mandelbrot_Controller mbc;
	mbc_init (&mbc);

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
				case SDL_KEYDOWN:
				mbc_update (&mbc, &app.event.key);
				break;
			}
		}
		
        glBindVertexArray (vao);
        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		app_frame_end (&app);
	}

	app_destroy (&app);
	return 0;
}











