#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented
{
    class Camera
    {
     public:
        Camera() {
            position = glm::vec3(0, 0, 0);
            forward  = glm::vec3(0, 0, 1);
            up       = glm::vec3(0, 1, 0);
            right    = glm::vec3(1, 0, 0);
            camera_dist = 2;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up, const float dist) {
            Set(position, center, up, dist);
        }

        ~Camera()
        { }

        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up, const float dist) {
            this->position = position;
            this->forward  = glm::normalize(center - position);
            this->right    = glm::cross(forward, up);
            this->up       = glm::cross(right, forward);
            this->camera_dist = dist;
        }

        void MoveForward(float distance) {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance) {
            position += glm::normalize(forward) * distance;
        }

        void TranslateRight(float distance) {
            position += glm::normalize(right) * distance;
        }

        void RotateFirstPerson_OX(float angle) {
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OY(float angle) {
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1)));
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OZ(float angle) {
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateThirdPerson_OX(float angle) {
            TranslateForward(camera_dist);
            RotateFirstPerson_OX(angle);
            TranslateForward(-camera_dist);
        }

        void RotateThirdPerson_OY(float angle) {
            TranslateForward(camera_dist);
            RotateFirstPerson_OY(angle);
            TranslateForward(-camera_dist);
        }

        void RotateThirdPerson_OZ(float angle) {
            TranslateForward(camera_dist);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-camera_dist);
        }

        void ChangeCameraDistanceThirdPerson(float diff) {
            if (camera_dist + diff > 0) {
                TranslateForward(camera_dist);
                camera_dist += diff;
                TranslateForward(-camera_dist);
            }
        }

        void SetCameraDistanceThirdPerson(float dist) {
            TranslateForward(camera_dist);
            camera_dist = dist;
            TranslateForward(-camera_dist);
        }

        glm::mat4 GetViewMatrix() {
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition() {
            return position + forward * camera_dist;
        }


     public:
        float camera_dist;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}
