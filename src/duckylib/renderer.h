#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <SDL3/SDL.h>
#include "glad/glad.h"
#include "camera.h"

typedef struct DL_Renderer
{
    unsigned int shader_program;

    unsigned int vao;
    unsigned int vbo;

    unsigned int vertex_count;
} DL_Renderer, Renderer;


int dl_renderer_opengl_error(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char *error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM: 
                error = "INVALID_ENUM"; 
                break;
            case GL_INVALID_VALUE:                 
                error = "INVALID_VALUE"; 
                break;
            case GL_INVALID_OPERATION:             
                error = "INVALID_OPERATION"; 
                break;
            case GL_STACK_OVERFLOW:                
                error = "STACK_OVERFLOW"; 
                break;
            case GL_STACK_UNDERFLOW:               
                error = "STACK_UNDERFLOW"; 
                break;
            case GL_OUT_OF_MEMORY:                 
                error = "OUT_OF_MEMORY"; 
                    break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: 
                error = "INVALID_FRAMEBUFFER_OPERATION"; 
                break;
        }
        printf("%s | %s (%d)\n", error, file, line);
    }
    return errorCode;
}

int dl_renderer_enable_transparency(bool toggle) 
{
    if(toggle) 
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }

    return 0;
}

int dl_renderer_enable_depth_test(bool toggle)
{
    if(toggle) 
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    return 0;
}

int dl_renderer_enable_culling(bool toggle) 
{
    if(toggle) 
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);
    }

    return 0;
}

int dl_renderer_enable_antialiasing(bool toggle)
{
    if(toggle)
    {
        glEnable(GL_LINE_SMOOTH);
    }

    return 0;
}

int dl_renderer_set_background(Vec3 color)
{
    glClearColor(color.x, color.y, color.z, 1.0f);
    return 0;
}

int dl_renderer_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return 0;
}

int dl_renderer_compile_shaders(DL_Renderer *renderer, const char *v_shader_src, const char *f_shader_src)
{
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &v_shader_src, NULL);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &f_shader_src, NULL);
    glCompileShader(fragment_shader);

    renderer->shader_program = glCreateProgram();

    glAttachShader(renderer->shader_program, vertex_shader);
    glAttachShader(renderer->shader_program, fragment_shader);

    glLinkProgram(renderer->shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return 0;
}

int dl_renderer_unbind_vao(DL_Renderer *renderer)
{
    glBindVertexArray(0);

    return 0;
}

int dl_renderer_unbind_vbo(DL_Renderer *renderer)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}

int dl_renderer_unbind_all(DL_Renderer *renderer)
{
    dl_renderer_unbind_vao(renderer);
    dl_renderer_unbind_vbo(renderer);

    return 0;
}

int dl_renderer_gen_vao(DL_Renderer *renderer) 
{
    glGenVertexArrays(1, &renderer->vao);

    return 0;
}

int dl_renderer_gen_vbo(DL_Renderer *renderer, float *data, size_t size) 
{
    glBindVertexArray(renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindVertexArray(0);

    renderer->vertex_count = size / sizeof(float) * 5;
}

int dl_renderer_link_attrib(DL_Renderer *renderer, unsigned int layout, unsigned int amount, GLenum type, GLsizeiptr stride, void* offset) 
{
    glBindVertexArray(renderer->vao);
    glVertexAttribPointer(layout, amount, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);

    return 0;
}



int dl_renderer_use_program(DL_Renderer *renderer)
{
    glUseProgram(renderer->shader_program);

    glUniformMatrix4fv(glGetUniformLocation(renderer->shader_program, "camera_matrix"), 1, GL_FALSE, camera.camera_matrix);

    return 0;
}

int dl_renderer_bind_vao(DL_Renderer *renderer)
{
    glBindVertexArray(renderer->vao);

    return 0;
}

int dl_renderer_draw(DL_Renderer *renderer) // think i need to update the first arg which is 0 down there, to the size of the vertices (count)
{
    glDrawArrays(GL_TRIANGLES, 0, renderer->vertex_count);

    return 0;
}

int dl_renderer_delete(DL_Renderer *renderer)
{
    glDeleteVertexArrays(1, &renderer->vao);
    glDeleteBuffers(1, &renderer->vbo);

    glDeleteProgram(renderer->shader_program);

    return 0;
}

#endif