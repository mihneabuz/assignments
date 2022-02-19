#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include <string>

inline glm::mat3 Translate2D(float translateX, float translateY) {
    return glm::transpose(
        glm::mat3(
            1, 0, translateX,
            0, 1, translateY,
            0, 0, 1
        )
    );

}

inline glm::mat3 Scale2D(float scaleX, float scaleY) {
    return glm::transpose(
        glm::mat3(
            scaleX,  0,    0,
              0,   scaleY, 0,
              0,     0,    1
        )
    );

}

inline Mesh* RectangleMesh(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
