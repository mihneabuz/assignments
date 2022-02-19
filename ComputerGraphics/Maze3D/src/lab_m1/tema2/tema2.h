#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/maze.h"

namespace m1
{
    const float CAMERA_DISTANCE     = 2.0f;
    const float PLAYER_HITBOX_SIZE  = 0.1f;
    const float PROJECTILE_SIZE     = 0.03f;
    const float PROJECTILE_COOLDOWN = 0.5f;
    const float PLAYER_MAX_HEALTH   = 100.0f;
    const float MAX_TIMER           = 100.0f;
    const glm::vec3 WALL_SIZE  = glm::vec3(3.0f, 5.0f, 3.0f);
    const glm::vec3 ENEMY_SIZE = glm::vec3(1.0f, 2.0f, 1.0f);
    const glm::vec3 EXIT_SIZE  = glm::vec3(1.0f, 200.0f, 1.0f);

    enum camera_mode {
        FIRST_PERSON,
        THIRD_PERSON
    };

    struct enemy {
        int x, y;
        glm::mat4 last_model;
        float decay;

        enemy(int x, int y, glm::mat4 model) {
            this->x = x;
            this->y = y;
            this->last_model = model;
            decay = 1.2f;
        }
    };

    struct projectile {
        glm::vec3 pos;
        glm::vec3 dir;
        bool hit;
        float timeout;

        projectile(glm::vec3 pos, glm::vec3 dir) {
            this->pos = pos;
            this->dir = dir;
            this->hit = false;
            this->timeout = 5.0f;
        }
    };

    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    class Tema2: public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void Render3D(Mesh *mesh, Shader *shader, const glm::mat4 &model_matrix);
        void Render2D(Mesh *mesh, Shader *shader, const glm::mat4 &model_matrix);
        void RenderAnimated(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, float decay);
        void SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear);

        void DrawScene(float deltaTimeSeconds);
        void DrawUI();

        bool WallCollision(const glm::vec3 pos, const float size);
        bool PlayerWallCollision();
        bool CameraWallCollision();
        bool PlayerAABBCollision(glm::vec3 a_pos, glm::vec3 a_size);
        bool SphereToAABBCollision(glm::vec3 sphere_pos, float radius, glm::vec3 aabb_pos, glm::vec3 aabb_size);


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        implemented::Camera *camera;
        glm::mat4 projection_matrix;
        float player_angle;
        float enemy_phase;
        float cooldown;
        float health;
        float timer;
        std::vector<enemy> enemies;
        std::vector<projectile> projectiles;
        camera_mode cam_mode;
        Maze maze;
    };
}
