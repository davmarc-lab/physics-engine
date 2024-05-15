#pragma once

#include "../Entity/Entity.hpp"

#include "../include/imgui-file-dialog/ImGuiFileDialog.h"
#include "../include/imgui/imgui.h"

#include "IGMenu.hpp"

class IGEntity : public IGMenu {

private:
    Entity *observer;

    enum AXIS {
        X, Y, Z, NONE
    };

    inline vec3 updateOtherValues(vec3 vec, AXIS axis) {
        float val = 0.f;

        switch (axis) {
            case AXIS::X: {
                val = vec.x;
                break;
            }
            case AXIS::Y: {
                val = vec.y;
                break;
            }
            case AXIS::Z: {
                val = vec.z;
                break;
            }
            default: {
                return vec3(vec);
            }
        }

        return vec = vec3(val);
    }

    inline AXIS findAxis(vector<AXIS> scaleAxis) {
        for (const auto elem: scaleAxis) {
            if (elem != AXIS::NONE)
                return elem;
        }
        return AXIS::NONE;
    }

public:
    IGEntity(Entity *entity) : observer(entity) {}

    inline virtual void render() override {
        ImGui::Begin("Entity");
        auto pos = this->observer->getPosition();
        auto scale = this->observer->getScale();
        auto rot = this->observer->getRotation();

        static bool lockFlag = false;
        vector<AXIS> scaleAxis;

        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::Text("Position:");
            if (ImGui::SliderFloat("x", &pos.x, -10, 10)) {
                this->observer->setMotionTime(0);
            }
            if (ImGui::SliderFloat("y", &pos.y, -10, 10)) {
                this->observer->setMotionTime(0);
            }
            if (ImGui::SliderFloat("z", &pos.z, -10, 10)) {
                this->observer->setMotionTime(0);
            }
            if (ImGui::Button("Reset")) {
                pos = vec3(0);
            }
            this->observer->setPosition(vec3(pos));

            ImGui::Text("Scale:");
            AXIS click;

            click = ImGui::SliderFloat("x##2", &scale.x, 0, 10) ? AXIS::X : AXIS::NONE;
            scaleAxis.push_back(click);
            click = ImGui::SliderFloat("y##2", &scale.y, 0, 10) ? AXIS::Y : AXIS::NONE;
            scaleAxis.push_back(click);
            click = ImGui::SliderFloat("z##2", &scale.z, 0, 10) ? AXIS::Z : AXIS::NONE;
            scaleAxis.push_back(click);

            if (ImGui::Button("Reset##2")) {
                scale = vec3(1);
            }

            ImGui::Checkbox("Lock All", &lockFlag);
            if (lockFlag) {
                scale = updateOtherValues(scale, findAxis(scaleAxis));
            }

            this->observer->setScale(vec3(scale));

            ImGui::Text("Rotation:");
            ImGui::SliderFloat("r.x", &rot.x, -3.14, 3.14);
            ImGui::SliderFloat("r.y", &rot.y, -3.14, 3.14);
            ImGui::SliderFloat("r.z", &rot.z, -3.14, 3.14);
            if (ImGui::Button("Reset Rotation")) {
                rot = vec3(0);
            }
            this->observer->setRotation(vec3(rot));
        }

        if (ImGui::CollapsingHeader("Texture")) {
            ImGui::Text("Texture File:");
            ImGui::Text("%s", this->observer->getTexture().getPath());
            if (ImGui::Button("Choose Texture")) {
                IGFD::FileDialogConfig config;
                config.path = "./resources/textures/";
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", config);
            }
            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    // action
                    this->observer->changeTexture(filePathName);
                }
                // close
                ImGuiFileDialog::Instance()->Close();
            }
        }

        auto vel = this->observer->getVelocity();
        auto force = this->observer->getForce();
        auto mass = this->observer->getMass();
    
        if (!ImGui::CollapsingHeader("Physics")) {
            ImGui::Text("Mass:");
            if (ImGui::SliderFloat("mass", &mass, 0, 200)) {
                this->observer->setMass(mass);
            }

            ImGui::Text("Velocity:");
            ImGui::SliderFloat("vel.x", &vel.x, -10, 10);
            ImGui::SliderFloat("vel.y", &vel.y, -10, 10);
            ImGui::SliderFloat("vel.z", &vel.z, -10, 10);
            this->observer->setVelocity(vel);

            ImGui::Text("Force:");
            ImGui::SliderFloat("for.x", &force.x, -10, 10);
            ImGui::SliderFloat("for.y", &force.y, -10, 10);
            ImGui::SliderFloat("for.z", &force.z, -10, 10);
        }


        ImGui::End();
    }

    ~IGEntity() = default;
};
