#include "p6/p6.h"
#include "glimac/default_shader.hpp"
#include "glm/glm.hpp"

struct Vertex2DColor{
    glm::vec2 position;
    glm::vec3 color;
    };


int main()
{
    auto ctx = p6::Context{{720, 720, "TP3 EX5"}};
    ctx.focus_window();


    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );


    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


   // Define parameters for the disk
    float radius = 0.5f;
    int numTriangles = 60; // You can adjust this value

    // Generate vertices for the disk
    std::vector<Vertex2DColor> vertices;
    for (int i = 0; i < numTriangles; ++i) {
        float theta1 = (i * 2 * 3.141592) / numTriangles;
        float theta2 = ((i + 1) * 2 * 3.141592) / numTriangles;

        // Triangle center
        glm::vec2 center(0.0f, 0.0f);

        // Vertices
        glm::vec2 v1(radius * glm::cos(theta1), radius * glm::sin(theta1));
        glm::vec2 v2(radius * glm::cos(theta2), radius * glm::sin(theta2));

        // Add vertices to the vector
        vertices.push_back({center, {1.f, 0.f, 0.f}});
        vertices.push_back({v1, {1.f, 0.f, 0.f}});
        vertices.push_back({v2, {1.f, 0.f, 0.f}});
    }

    // Send the vertices to the GPU
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);

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
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // Unbind the VAO
        glBindVertexArray(0);
    };

    ctx.start();


    glDeleteBuffers(1, &vbo);


    glDeleteVertexArrays(1, &vao);
}