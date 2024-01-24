#include <p6/p6.h>
#include "glm/glm.hpp"
#include <vector>

struct Vertex2DColor {
    glm::vec2 position;
    glm::vec3 color;
};

int main() {
    auto ctx = p6::Context{{720, 720, "TP OpenGL"}};
    ctx.focus_window();

    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Define parameters for the disk
    float radius = 0.5f;
    int numTriangles = 60;

    // => Tableau de sommets : un seul exemplaire de chaque sommet
    std::vector<Vertex2DColor> vertices;
    vertices.push_back({glm::vec2(0.0f, 0.0f), {1.f, 0.f, 0.f}}); // Sommet 0 (centre du disque)

    for (int i = 0; i < numTriangles; ++i) {
        float theta1 = (i * 2 * 3.141592) / numTriangles;
        glm::vec2 vertexPos(radius * glm::cos(theta1), radius * glm::sin(theta1));

        // Add vertex to the vector
        vertices.push_back({vertexPos, {1.f, 0.f, 0.f}});
    }

    // => Penser à bien changer le nombre de sommets (1 + N au lieu de N + 1)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // => Creation du IBO
    GLuint ibo;
    glGenBuffers(1, &ibo);

    // => On bind sur GL_ELEMENT_ARRAY_BUFFER, cible réservée pour les IBOs
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // => Tableau d'indices : ce sont les indices des sommets à dessiner
    // On en a 3 * N afin de former N triangles
    // Chaque indice correspond au sommet correspondant dans le VBO
    std::vector<uint32_t> indices;
    for (int i = 1; i <= numTriangles; ++i) {
        indices.push_back(0); // Centre du disque
        indices.push_back(i);
        indices.push_back((i % numTriangles) + 1);
    }

    // => On remplit l'IBO avec les indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // => Comme d'habitude, on debind avant de passer à autre chose
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    // => On bind l'IBO sur GL_ELEMENT_ARRAY_BUFFER; puisqu'un VAO est actuellement bindé,
    // cela a pour effet d'enregistrer l'IBO dans le VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    static constexpr GLuint VERTEX_ATTR_POSITION = 3;
    static constexpr GLuint VERTEX_ATTR_COLOR    = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, color));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Application loop :
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        shader.use();

        // => On utilise glDrawElements à la place de glDrawArrays
        // Cela indique à OpenGL qu'il doit utiliser l'IBO enregistré dans le VAO
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    };

    // Start the update loop
    ctx.start();

    // Cleanup once the window is closed
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ibo);
}
