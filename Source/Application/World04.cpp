#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World04::Initialize()
    {

        auto material = GET_RESOURCE(Material, "Materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("Models/squirrel.obj");

        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt) 
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::DragFloat2("UV Offset", &uvOffset[0]);
        ImGui::DragFloat2("UV Tiling", &uvTiling[0]);
        ImGui::End();

        
        m_transform.position.x -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? dt * m_speed : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt * m_speed : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? dt * m_speed : 0;
        m_transform.position.z -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt * m_speed : 0;

        m_transform.position.y -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LSHIFT) ? dt * m_speed : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_SPACE) ? dt * m_speed : 0;

        m_time += dt;

        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();

        //model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());


        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        material->GetProgram()->SetUniform("view", view);
        
        //projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer) {

        // pre-render
        renderer.BeginFrame();
        // render
        m_model->Draw();
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();

    }
}
