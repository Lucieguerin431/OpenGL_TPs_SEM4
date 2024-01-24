#include "p6/p6.h"
#include "glimac/default_shader.hpp"
#include "glm/glm.hpp"
#include <vector>
struct Vertex2DColor{
    glm::vec2 position;
    glm::vec3 color;
    };


int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();


    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );


    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    Vertex2DColor vertices[] = { 
        Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}}, // Premier sommet
        Vertex2DColor{{ 0.5f, -0.5f}, {0.f, 1.f, 0.f}}, // Deuxième sommet
        Vertex2DColor{{ -0.5f,  0.5f}, {0.f, 0.f, 1.f}},  // Troisième sommet
        Vertex2DColor{{0.5f, 0.5f}, {1.f, 1.f, 0.f}}, // Premier sommet
        Vertex2DColor{{ 0.5f, -0.5f}, {0.f, 1.f, 0.f}}, // Deuxième sommet
        Vertex2DColor{{ -0.5f,  0.5f}, {0.f, 0.f, 1.f}}
    };

    // Send the vertices to the GPU
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);

    // Unbind the VBO to avoid modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    GLuint vao;
    glGenVertexArrays(1, &vao);

    // Bind VAO, 
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    static constexpr GLuint vertex_attr_position = 3;
    glEnableVertexAttribArray(vertex_attr_position);

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*) offsetof(Vertex2DColor,position)); 

    static constexpr GLuint vertex_attr_color =8;
    glEnableVertexAttribArray(vertex_attr_color);

    glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*) (offsetof(Vertex2DColor,color)));

    //Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Unbind the VAO to avoid modification
    glBindVertexArray(0);



    ctx.update = [&]() {

        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the VAO
        glBindVertexArray(vao);

     
        glimac::bind_default_shader();

        // Call shaders
        shader.use();

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Unbind the VAO
        glBindVertexArray(0);
    };

    ctx.start();


    glDeleteBuffers(1, &vbo);


    glDeleteVertexArrays(1, &vao);
}