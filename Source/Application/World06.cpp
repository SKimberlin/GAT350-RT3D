#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(1024, 1024);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material)
        {
            material->albedoTexture = GET_RESOURCE(Texture, "fb_texture");
        }

        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt) 
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        // Set postprocess gui
        ImGui::Begin("Post-Process");

        ImGui::SliderFloat("Blend", &m_blend, 0, 1);

        bool effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect))
        {
            (effect) ? m_params |= INVERT_MASK : m_params &= ~INVERT_MASK;
        }

        effect = m_params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect))
        {
            (effect) ? m_params |= GRAYSCALE_MASK : m_params &= ~GRAYSCALE_MASK;
        }

        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("Colortint", &effect))
        {
            (effect) ? m_params |= COLORTINT_MASK : m_params &= ~COLORTINT_MASK;
        }
        ImGui::ColorEdit3("Tint", &m_tint[0]);

        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect))
        {
            (effect) ? m_params |= GRAIN_MASK : m_params &= ~GRAIN_MASK;
        }

        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect))
        {
            (effect) ? m_params |= SCANLINE_MASK : m_params &= ~SCANLINE_MASK;
        }

        effect = m_params & PIXELATE_MASK;
        if (ImGui::Checkbox("Pixelate", &effect))
        {
            (effect) ? m_params |= PIXELATE_MASK : m_params &= ~PIXELATE_MASK;
        }
        ImGui::DragInt("Pixel Count", &m_pixels, 8, 8, 512);

        ImGui::End();

        // Set postprocess shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", m_params);
            program->SetUniform("tint", m_tint);
            program->SetUniform("pixels", m_pixels);
        }
        
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer) 
    {
        // *** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({0,0,0});
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("postprocess")->active = true;
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);


        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();

    }
}
