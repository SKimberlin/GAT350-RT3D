#include "ModelComponent.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"

namespace nc
{
	CLASS_DEFINITION(ModelComponent)

	bool ModelComponent::Initialize()
	{
		if (!modelName.empty())
		{
			model = GET_RESOURCE(Model, modelName);

		}
		if (model && !materialName.empty())
		{
			material = GET_RESOURCE(Material, materialName);
		}

		return true;
	}

	void ModelComponent::Update(float dt)
	{
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullface);
		
		model->Draw();
	}

	void ModelComponent::ProcessGui()
	{
		ImGui::Checkbox("Enable Depth", &enableDepth);
		ImGui::Checkbox("Cast Shadow", &castShadow);
	}

	void ModelComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);

		READ_DATA(value, enableDepth);
		READ_DATA(value, castShadow);

		std::string cullfaceName;
		if (READ_NAME_DATA(value, "cullface", cullfaceName))
		{
			if (IsEqualIgnoreCase(cullfaceName, "front")) cullface = GL_FRONT;
			if (IsEqualIgnoreCase(cullfaceName, "none")) cullface = GL_NONE;
		}
		
	}
}