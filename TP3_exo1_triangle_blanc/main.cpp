#include "p6/p6.h"
#include "glimac/default_shader.hpp"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Create an array of vertices to draw a triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f,// 1er sommet
        0.5f, -0.5f,//2eme sommet
        0.0f,  0.5f // 3eme sommet
    };

    // Send the vertices to the GPU
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // Unbind the VBO to avoid modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //Create vao
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // Bind VAO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);

    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // Unbind the VBO to avoid modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the VAO to avoid modification
    glBindVertexArray(0);


    // Declare your infinite update loop.
    ctx.update = [&]() {

        // Clear the color buffer to avoid accumulation (here we only need to clear the color buffer)
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the VAO
        glBindVertexArray(vao);

        // Use the default shader provided by glimac
        glimac::bind_default_shader();

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Unbind the VAO
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // Free the memory allocated for the VBO
    glDeleteBuffers(1, &vbo);

    // Free the memory allocated for the VAO
    glDeleteVertexArrays(1, &vao);
}