#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include "object2d.h"
#include "object3d.h"

using namespace std;
using namespace m1;

Tema2::Tema2() {}
Tema2::~Tema2() {}

void Tema2::Init() {
    camera = new implemented::Camera();
    camera->Set(glm::vec3(4.5f, 1.4f, 4.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), CAMERA_DISTANCE);
    camera->RotateThirdPerson_OY(RADIANS(180));
    player_angle = RADIANS(45);

    {   // MESHES
        meshes["wall"]   = WallMesh();
        meshes["ground"] = GroundMesh();
        meshes["enemy"]  = EnemyMesh();
        meshes["exit"]   = ExitMesh();

        meshes["player_head"]  = PlayerHeadMesh();
        meshes["player_torso"] = PlayerTorsoMesh();
        meshes["player_arm"]   = PlayerArmMesh();
        meshes["player_leg"]   = PlayerLegMesh();

        Mesh* health_bar = RectangleMesh("health_bar", glm::vec3(0), 4.0f, 2.0f, glm::vec3(0.9f, 0.2f, 0.3f), true);
        Mesh* timer_bar  = RectangleMesh("timer_bar",  glm::vec3(0), 4.0f, 2.0f, glm::vec3(0.2f, 0.4f, 0.8f), true);
        meshes["health_bar"] = health_bar;
        meshes["timer_bar"]  = timer_bar;

        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {   // SHADERS
        Shader *shader = new Shader("enemy_death");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projection_matrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 100.0f);
    cam_mode = THIRD_PERSON;

    enemies     = std::vector<enemy>();
    projectiles = vector<projectile>();

    cooldown    = 0.0f;
    enemy_phase = 0.0f;

    health = PLAYER_MAX_HEALTH;
    timer  = MAX_TIMER + 4;
}

void Tema2::FrameStart() {}

void Tema2::Update(float deltaTimeSeconds) {
    const glm::ivec2 resolution = window->GetResolution();

    if (health > 0.0f && timer > 0.0f) {
        auto viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
        SetViewportArea(viewSpace, glm::vec3(0.08f, 0.0f, 0.18f), true);
        DrawScene(deltaTimeSeconds);

        viewSpace = ViewportSpace(40, 40, 440, 200);
        SetViewportArea(viewSpace, glm::vec3(0.1f), true);
        DrawUI();

        std::vector<projectile> temp1;
        for (auto proj : projectiles)
            if (!proj.hit && proj.timeout > 0)
                temp1.push_back(proj);
        projectiles = temp1;

        std::vector<enemy> temp2;
        for (auto en : enemies)
            if (en.decay > 0.0f)
                temp2.push_back(en);
        enemies = temp2;

        enemy_phase += deltaTimeSeconds;
        enemy_phase = enemy_phase > 8.0f ? enemy_phase - 8.0f : enemy_phase;

        cooldown -= deltaTimeSeconds;
        timer    -= deltaTimeSeconds;

    } else {
        std::cout << "\n\n\n\n";
        std::cout << "###############\n";
        std::cout << "## YOU LOSE! ##\n";
        std::cout << "###############\n";
        exit(0);
    }
}

void Tema2::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear) {
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.6f, 400);
    GetSceneCamera()->Update();
}

void Tema2::DrawScene(float deltaTimeSeconds) {
    if (cam_mode == THIRD_PERSON) {
        camera->SetCameraDistanceThirdPerson(CAMERA_DISTANCE);
        while (CameraWallCollision())
            camera->ChangeCameraDistanceThirdPerson(-0.02f);
    }

    auto ground_matrix = glm::mat4(1.0f);
    ground_matrix = glm::scale(ground_matrix, glm::vec3(1000.0f, 0.1f, 1000.0f));
    ground_matrix = glm::translate(ground_matrix, glm::vec3(0, -0.5f, 0));
    Render3D(meshes["ground"], shaders["VertexColor"], ground_matrix);

    for (int i = 0; i < maze.size; i++) {
        for (int j = 0; j < maze.size; j++) {
            if (maze.map[i][j] == WALL) {
                auto model_matrix = glm::mat4(1.0f);
                model_matrix = glm::scale(model_matrix, WALL_SIZE);
                model_matrix = glm::translate(model_matrix, glm::vec3(i, 0.5f, j));
                Render3D(meshes["wall"], shaders["VertexColor"], model_matrix);
            }

            if (maze.map[i][j] == EXIT) {
                glm::mat4 model_matrix = glm::mat4(1.0f);
                model_matrix = glm::scale(model_matrix, WALL_SIZE);
                model_matrix = glm::translate(model_matrix, glm::vec3(i, 0.8f, j));
                model_matrix = glm::scale(model_matrix, glm::vec3(1.0f) / WALL_SIZE * EXIT_SIZE);
                Render3D(meshes["exit"], shaders["VertexColor"], model_matrix);

                auto pos_vec = glm::vec3(model_matrix[3][0], model_matrix[3][1], model_matrix[3][2]);
                if (PlayerAABBCollision(pos_vec, EXIT_SIZE)) {
                    std::cout << "\n\n\n\n";
                    std::cout << "##############\n";
                    std::cout << "## YOU WIN! ##\n";
                    std::cout << "##############\n";
                    exit(0);
                }
            }

            if (maze.map[i][j] == ENEMY) {

                glm::mat4 model_matrix = glm::mat4(1.0f);
                model_matrix = glm::scale(model_matrix, WALL_SIZE);
                model_matrix = glm::translate(model_matrix, glm::vec3(i, 0.2f, j));
                model_matrix = glm::scale(model_matrix, glm::vec3(1.0f) / WALL_SIZE * ENEMY_SIZE);

                float phase = enemy_phase + ((float)i * 7 + j * 17);
                phase = fmod(phase, 8.0f);

                float dx, dy;
                if (phase > 6.0f) {
                    dx = 0.0f;
                    dy = 8 - phase;
                } else if (phase > 4.0f) {
                    dx = 6 - phase;
                    dy = 2.0f;
                } else if (phase > 2.0f) {
                    dx = 2.0f;
                    dy = phase - 2;
                } else {
                    dx = phase;
                    dy = 0.0f;
                }

                model_matrix = glm::translate(model_matrix, glm::vec3((dx - 1.0f) * 0.7f, 0, (dy - 1.0f) * 0.7f));
                auto pos_vec = glm::vec3(model_matrix[3][0], model_matrix[3][1], model_matrix[3][2]);

                if (PlayerAABBCollision(pos_vec, ENEMY_SIZE * 0.5f))
                    health -= deltaTimeSeconds * 50;

                for (auto& proj : projectiles) {
                    if (SphereToAABBCollision(proj.pos, PROJECTILE_SIZE, pos_vec, ENEMY_SIZE * 0.5f)) {
                        proj.hit = true;
                        maze.map[i][j] = EMPTY;
                        enemies.emplace_back(enemy{i, j, model_matrix});
                    } 
                }

                Render3D(meshes["enemy"], shaders["VertexColor"], model_matrix);
            }
        }
    }

    if (cam_mode == THIRD_PERSON) {
        auto player_matrix = glm::translate(glm::mat4(1.0f), camera->GetTargetPosition());
        player_matrix = glm::rotate(player_matrix, player_angle, glm::vec3(0, 1, 0));
        player_matrix = glm::translate(player_matrix, glm::vec3(0, -0.3f, 0));

        // body
        auto body_matrix = player_matrix;
        Render3D(meshes["player_torso"], shaders["VertexColor"], glm::scale(body_matrix, glm::vec3(0.4f, 0.6f, 0.3f)));

        // head
        auto head_matrix = glm::translate(player_matrix, glm::vec3(0, 0.4f, 0));
        Render3D(meshes["player_head"], shaders["VertexColor"], glm::scale(head_matrix, glm::vec3(0.22f)));

        // arms 
        auto arm_matrix = glm::translate(player_matrix, glm::vec3(0.28f, 0, 0));
        Render3D(meshes["player_arm"], shaders["VertexColor"], glm::scale(arm_matrix, glm::vec3(0.16f, 0.5f, 0.26f)));
        arm_matrix = glm::translate(player_matrix, glm::vec3(-0.28f, 0, 0));
        Render3D(meshes["player_arm"], shaders["VertexColor"], glm::scale(arm_matrix, glm::vec3(0.16f, 0.5f, 0.26f)));

        // legs
        auto leg_matrix = glm::translate(player_matrix, glm::vec3(0.1f, -0.5f, 0));
        Render3D(meshes["player_leg"], shaders["VertexColor"], glm::scale(leg_matrix, glm::vec3(0.18f, 0.4f, 0.28f)));
        leg_matrix = glm::translate(player_matrix, glm::vec3(-0.1f, -0.5f, 0));
        Render3D(meshes["player_leg"], shaders["VertexColor"], glm::scale(leg_matrix, glm::vec3(0.18f, 0.4f, 0.28f)));
    }

    for (auto& enemy : enemies) {
        enemy.decay -= deltaTimeSeconds;
        auto enemy_model = glm::translate(enemy.last_model, glm::vec3(0, -1.0f, 0));
        RenderAnimated(meshes["enemy"], shaders["enemy_death"], enemy_model, enemy.decay);
    }

    for (auto& proj : projectiles) {
        proj.pos += deltaTimeSeconds * proj.dir * 10.0f;
        proj.timeout -= deltaTimeSeconds;
        auto model_matrix = glm::translate(glm::mat4(1.0f), proj.pos);
        Render3D(meshes["sphere"], shaders["VertexColor"], glm::scale(model_matrix, glm::vec3(PROJECTILE_SIZE * 5)));

        if (WallCollision(proj.pos, PROJECTILE_SIZE) || proj.pos.y < PROJECTILE_SIZE)
            proj.hit = true;
    }
}

void Tema2::DrawUI() {
    auto health_scale = Scale2D(max(min(health, PLAYER_MAX_HEALTH) / 4.5f, 0.0f), 18.0f);
    Render2D(meshes["health_bar"], shaders["VertexColor"], Translate2D(5.0f, 10.0f) * health_scale);

    auto timer_scale = Scale2D(max(min(timer, MAX_TIMER) / 4.5f, 0.0f), 18.0f);
    Render2D(meshes["timer_bar"], shaders["VertexColor"], Translate2D(5.0f, 54.0f) * timer_scale);
}

bool Tema2::SphereToAABBCollision(glm::vec3 sphere_pos, float radius, glm::vec3 aabb_pos, glm::vec3 aabb_size) {

  float x = max(aabb_pos.x - aabb_size.x, min(sphere_pos.x, aabb_pos.x + aabb_size.x));
  float y = max(aabb_pos.y - aabb_size.y, min(sphere_pos.y, aabb_pos.y + aabb_size.y));
  float z = max(aabb_pos.z - aabb_size.z, min(sphere_pos.z, aabb_pos.z + aabb_size.z));

  float distance = sqrt((x - sphere_pos.x) * (x - sphere_pos.x) +
                        (y - sphere_pos.y) * (y - sphere_pos.y) +
                        (z - sphere_pos.z) * (z - sphere_pos.z));

  return distance < radius;
}

bool Tema2::PlayerAABBCollision(glm::vec3 a_pos, glm::vec3 a_size) {
    auto player_pos = camera->GetTargetPosition();

    return (player_pos.x - PLAYER_HITBOX_SIZE <= a_pos.x + a_size.x && 
            player_pos.x + PLAYER_HITBOX_SIZE >= a_pos.x - a_size.x) &&
           (player_pos.y - PLAYER_HITBOX_SIZE <= a_pos.y + a_size.y && 
            player_pos.y + PLAYER_HITBOX_SIZE >= a_pos.y - a_size.y) &&
           (player_pos.z - PLAYER_HITBOX_SIZE <= a_pos.z + a_size.z && 
            player_pos.z + PLAYER_HITBOX_SIZE >= a_pos.z - a_size.z);
}

bool Tema2::CameraWallCollision() {
    return WallCollision(camera->position, 0.01f) || camera->position.y < 0.0f;
}

bool Tema2::PlayerWallCollision() {
    return WallCollision(camera->GetTargetPosition(), PLAYER_HITBOX_SIZE);
}

bool Tema2::WallCollision(const glm::vec3 pos, const float size) {
    auto maze_x = pos.x / 3;
    auto maze_y = pos.z / 3;
    for (auto dx : {size, -size})
        for (auto dy : {size, -size})
            if (maze.map[round(maze_x + dx)][round(maze_y + dy)] == WALL)
                return true;

    return false;
}


void Tema2::FrameEnd() {}

void Tema2::Render3D(Mesh* mesh, Shader* shader, const glm::mat4& model_matrix) {
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
    mesh->Render();
}

void Tema2::Render2D(Mesh* mesh, Shader* shader, const glm::mat4& model_matrix) {
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    auto camera2D = GetSceneCamera();
    camera2D->SetOrthographic(0, 100, 0, 100, 0.01f, 400);
    camera2D->SetPosition(glm::vec3(0, 0, 50));
    camera2D->SetRotation(glm::vec3(0, 0, 0));
    camera2D->Update();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera2D->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera2D->GetProjectionMatrix()));

    glm::mat3 mm = model_matrix;
    glm::mat4 model = glm::mat4(
        mm[0][0], mm[0][1], mm[0][2], 0.f,
        mm[1][0], mm[1][1], mm[1][2], 0.f,
        0.f, 0.f, mm[2][2], 0.f,
        mm[2][0], mm[2][1], 0.f, 1.f);

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
    mesh->Render();
}

void Tema2::RenderAnimated(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, float seed)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();

    GLint loc_model_matrix    = glGetUniformLocation(shader->program, "model_matrix");
    int loc_view_matrix       = glGetUniformLocation(shader->program, "view_matrix");
    int loc_projection_matrix = glGetUniformLocation(shader->program, "projection_matrix");
    int time                  = glGetUniformLocation(shader->program, "time");

    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform1f(time, seed);

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    float player_speed = 3.0f, sprint_modifier = 1.8f;

    float forward = 0.0f, right = 0.0f;
    if (window->KeyHold(GLFW_KEY_W)) {
        forward = player_speed * deltaTime;
    } else if (window->KeyHold(GLFW_KEY_S)) {
        forward = -player_speed* deltaTime;
    } 

    if (window->KeyHold(GLFW_KEY_A)) {
        right = -player_speed * deltaTime;
    } else if (window->KeyHold(GLFW_KEY_D)) {
        right = player_speed * deltaTime;
    } 

    if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
        forward *= sprint_modifier;
        right   *= sprint_modifier;
    }

    if (right != 0.0f && forward != 0.0f) {
        forward /= 1.42f;
        right   /= 1.42f;
    }

    camera->MoveForward(forward);
    if (PlayerWallCollision())
        camera->MoveForward(-forward);

    camera->TranslateRight(right);
    if (PlayerWallCollision())
        camera->TranslateRight(-right);
}


void Tema2::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_T || key == GLFW_KEY_LEFT_CONTROL) {
        if (cam_mode == THIRD_PERSON) {
            cam_mode = FIRST_PERSON;
            projection_matrix = glm::perspective(RADIANS(90), window->props.aspectRatio, 0.01f, 100.0f);
            camera->SetCameraDistanceThirdPerson(0.0f);
        } else  {
            cam_mode = THIRD_PERSON;
            projection_matrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 100.0f);
        }
    }

    if (cam_mode == FIRST_PERSON && key == GLFW_KEY_SPACE && cooldown < 0.0f) {
        projectiles.push_back(projectile(camera->GetTargetPosition(), camera->forward));
        cooldown = PROJECTILE_COOLDOWN;
    }

}

void Tema2::OnKeyRelease(int key, int mods) {}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    const float sensivity = -0.006f;

    if (! window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (cam_mode == FIRST_PERSON) {
            camera->RotateFirstPerson_OX(sensivity * deltaY);
            camera->RotateFirstPerson_OY(sensivity * deltaX);
            player_angle += sensivity * deltaX;
        }

        if (cam_mode == THIRD_PERSON) {
            camera->RotateThirdPerson_OX(sensivity * deltaY);
            camera->RotateThirdPerson_OY(sensivity * deltaX);
            player_angle += sensivity * deltaX;
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Tema2::OnWindowResize(int width, int height) {}
