#include "CameraComponent.h"
#include "Framework/Actor.h"
#include "Framework/Engine.h"

namespace nc
{
	CLASS_DEFINITION(CameraComponent)

	bool CameraComponent::Initialize()
	{
		// get aspect from window if not set
		if (aspect == 0)
		{
			// set aspect with renderer width / renderer height (make sure it is a floating point division)
			// aspect = width / height;
			aspect = Renderer().GetWidth() / (float)Renderer().GetHeight();
		}

		return true;
	}

	void CameraComponent::Update(float dt)
	{
		//view matrix
		view = glm::lookAt(m_owner->transform.position, (m_owner->transform.position + m_owner->transform.Forward()), m_owner->transform.Up());
		
		//projection matrix
		if (projectionType == Perspective)
		{
			projection = glm::perspective(glm::radians(fov), aspect, near, far);
		}
		else
		{
			projection = glm::ortho(-size * aspect * 0.5f, size * aspect * 0.5f, -size * 0.5f, size * 0.5f, near, far);
		}
	}

	void CameraComponent::SetPerspective(float fov, float aspect, float near, float far)
	{
		// set this class fov, aspect, near and far values with parameters (this->fov = fov, ...)
		this->fov = fov;
		this->aspect = aspect;
		this->near = near;
		this->far = far;

		projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}

	void CameraComponent::SetLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
	{
		view = glm::lookAt(eye, center, up);
	}

	void CameraComponent::SetProgram(res_t<Program> program)
	{
		// set program uniform for "view" with view matrix
		// set program uniform for "projection" with projection matrix
		program->SetUniform("view", view);
		program->SetUniform("projection", projection);
		
	}

	void CameraComponent::ProcessGui()
	{
		const char* types[] = { "Perspective", "Orthographic" };
		ImGui::Combo("Projection", (int*)(&projectionType), types, 2);

		// use ImGui::DragFloat to set fov, aspect, near and far values (use speed of 0.1f)
		ImGui::DragFloat("FOV", &fov, 0.1f);
		ImGui::DragFloat("Aspect", &aspect, 0.1f);
		ImGui::DragFloat("Near", &near, 0.1f);
		ImGui::DragFloat("Far", &far, 0.1f);
		ImGui::DragFloat("Size", &size, 0.1f);

	}

	void CameraComponent::Read(const json_t& value)
	{
		// READ_DATA of fov, aspect, near and far values
		READ_DATA(value, fov);
		READ_DATA(value, aspect);
		READ_DATA(value, near);
		READ_DATA(value, far);

		std::string projectionTypeName;
		READ_NAME_DATA(value, "projectionType", projectionTypeName);
		if (IsEqualIgnoreCase("orthographic", projectionTypeName)) projectionType = Orthographic;
		READ_DATA(value, size);

		SetPerspective(fov, aspect, near, far);
	}
}