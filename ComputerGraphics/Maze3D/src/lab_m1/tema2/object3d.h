#include "core/gpu/mesh.h"

inline Mesh* CreateMesh(
	const std::string name,
	const std::vector<VertexFormat>& vertices,
	const std::vector<unsigned int>& indices
)
{
	unsigned int VAO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindVertexArray(0);

	Mesh* mesh = new Mesh(name);
	mesh->InitFromBuffer(VAO, static_cast<GLushort>(indices.size()));
	mesh->vertices = vertices;
	mesh->indices = indices;

	return mesh;
}

inline Mesh* CubeMesh(std::string name, glm::vec3 top_color, glm::vec3 bot_color) {
    std::vector<VertexFormat> vertices {
        VertexFormat(glm::vec3(-0.5f, -0.5f, 0.5f),   bot_color),
        VertexFormat(glm::vec3(0.5f, -0.5f, 0.5f),    bot_color),
        VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f),    top_color),
        VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f),     top_color),
        VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f),  bot_color),
        VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f),   bot_color),
        VertexFormat(glm::vec3(-0.5f, 0.5f, -0.5f),   top_color),
        VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f),    top_color)
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,		1, 3, 2,
        2, 3, 7,		2, 7, 6,
        1, 7, 3,		1, 5, 7,
        6, 7, 4,		7, 5, 4,
        0, 4, 1,		1, 4, 5,
        2, 6, 4,		0, 2, 4,
    };

    return CreateMesh("ground", vertices, indices);
}

inline Mesh* PlayerHeadMesh() {
    return CubeMesh("player_head", glm::vec3(0.9f, 0.8f, 0.8f), glm::vec3(0.9f, 0.8f, 0.8f));
}

inline Mesh* PlayerTorsoMesh() {
    return CubeMesh("player_torso", glm::vec3(0.8f, 0.6f, 0.3f), glm::vec3(0.9f, 0.7f, 0.5f));
}

inline Mesh* PlayerArmMesh() {
    return CubeMesh("player_arm", glm::vec3(0.9f, 0.8f, 0.8f), glm::vec3(0.9f, 0.8f, 0.8f));
}

inline Mesh* PlayerLegMesh() {
    return CubeMesh("player_leg", glm::vec3(0.9f, 0.7f, 0.5f), glm::vec3(0.5f, 0.3f, 0.2f));
}

inline Mesh* GroundMesh() {
    return CubeMesh("ground", glm::vec3(0.16f, 0.68f, 0.46f), glm::vec3(0.2f, 0.7f, 0.5f));
}

inline Mesh* EnemyMesh() {
    return CubeMesh("enemy", glm::vec3(0.9f, 0.3f, 0.38f), glm::vec3(0.86f, 0.42f, 0.36f));
}

inline Mesh* ExitMesh() {
    return CubeMesh("wall", glm::vec3(0.16f, 0.22f, 0.88f), glm::vec3(0.1f, 0.2f, 0.9f));
}

inline Mesh* WallMesh() {
    return CubeMesh("wall", glm::vec3(0.46f, 0.52f, 0.68f), glm::vec3(0.18f, 0.26f, 0.36f));
}
