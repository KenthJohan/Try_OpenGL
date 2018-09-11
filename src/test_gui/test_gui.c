#include "app.h"



int main (int argc, char *argv[])
{
	struct Application app;
	app_init (&app, argc, argv);
	
	struct GUI gui;
	gui_init (&gui, 3);
	
	float y = -0.3f;
	gui.rectangles [0].sw = (struct v2f32_xy){-1.0f, y};
	gui.rectangles [0].ne = (struct v2f32_xy){0.0f, 1.0f};
	
	gui.rectangles [1].sw = (struct v2f32_xy){0.0f, y};
	gui.rectangles [1].ne = (struct v2f32_xy){1.0f, 1.0f};
	
	gui.rectangles [2].sw = (struct v2f32_xy){-1.0f, -1.0f};
	gui.rectangles [2].ne = (struct v2f32_xy){1.0f, -0.4f};
	
	gui_sync (&gui);

	GLint uniform_color = glGetUniformLocation (app.program, "objectColor");

	
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
				{
					printf ("SDL_MOUSEBUTTONDOWN\n");
					float m [2];
					ui_mouse (m, app.window, &app.event.button);
					gui_intersect_point (&gui, m, GUI_COLOR1, 0);
					//printf ("");
				}
				
				case SDL_MOUSEBUTTONUP:
				if (app.event.button.state == SDL_RELEASED)
				{
					printf ("SDL_MOUSEBUTTONUP\n");
					float m [2];
					ui_mouse (m, app.window, &app.event.button);
					gui_intersect_point (&gui, m, 0, GUI_COLOR1);
					//printf ("");
				}
				
				//gui_hit (app.window, &app.event.button, boxset1, 2);
				break;
			}
		}
	
		gui_draw (&gui, uniform_color);
		
		app_frame_end (&app);
	}

	app_destroy (&app);
	return 0;
}










