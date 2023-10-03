#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt) {

        m_angle -= dt * ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? dt * 90 : 0;
        m_angle += dt * ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? dt * 90 : 0;

        m_position.x -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? dt * 5 : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt * 5 : 0;

        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt * 5 : 0;
        m_position.y -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? dt * 5 : 0;

        m_time += dt;

    }

    void World01::Draw(Renderer& renderer) {

        // pre-render
        renderer.BeginFrame();

        // render

        glPushMatrix();
        glTranslatef(m_position.x, m_position.y, 0);
        glRotatef(m_angle, 0, 0, 1);

        glBegin(GL_TRIANGLE_STRIP);

        glColor3f(1, 0, 0);
        glVertex2f(0, 0);

        glColor3f(0, 1, 0);
        glVertex2f(0, -0.5f);

        glColor3f(0, 0, 1);
        glVertex2f(0.5f, -0.5f);

        glColor3f(0, 0, 1);
        glVertex2f(0.5f, 0);

        glEnd();

        glPopMatrix();

        // post-render
        renderer.EndFrame();

    }
}
