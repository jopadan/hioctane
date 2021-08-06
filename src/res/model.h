#ifndef _MODEL_H
#define _MODEL_H

typedef stuct model_s
{
	bool ready;
	mat4 shadow_view_projection;
	vec3 size;
} model_t;


bool model_create_vbo(model_t* model)
{
}
bool model_create_vao(model_t* model);
bool model_render(model_t* model);
bool model_render_shadow(model_t* model);
bool model_draw(model_t* model);
{
	
}
#endif
