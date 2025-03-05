#include "euclediapch.h"
#include "Eucledia/Scene/SceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Eucledia/Scene/Entity.h"
#include "Eucledia/Scene/Components.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& vector)
		{
			Node node;
			node.push_back(vector.x);
			node.push_back(vector.y);
			node.push_back(vector.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& vector)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			vector.x = node[0].as<float>();
			vector.y = node[1].as<float>();
			vector.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& vector)
		{
			Node node;
			node.push_back(vector.x);
			node.push_back(vector.y);
			node.push_back(vector.z);
			node.push_back(vector.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& vector)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			vector.x = node[0].as<float>();
			vector.y = node[1].as<float>();
			vector.z = node[2].as<float>();
			vector.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Eucledia
{
	YAML::Emitter& operator<<(YAML::Emitter& data, const glm::vec3& vector)
	{
		data << YAML::Flow;
		data << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
		return data;
	}

	YAML::Emitter& operator<<(YAML::Emitter& data, const glm::vec4& vector)
	{
		data << YAML::Flow;
		data << YAML::BeginSeq << vector.x << vector.y << vector.z << vector.w << YAML::EndSeq;
		return data;
	}

	SceneSerializer::SceneSerializer(const ref<Scene>& scene) : _scene(scene)
	{
	}

	static void serializeEntity(YAML::Emitter& data, Entity entity)
	{
		data << YAML::BeginMap;
		data << YAML::Key << "Entity" << YAML::Value << "1";

		if (entity.hasComponent<TagComponent>())
		{
			auto& tag = entity.getComponent<TagComponent>().tag;

			data << YAML::Key << "TagComponent";
			data << YAML::BeginMap;
			data << YAML::Key << "Tag" << YAML::Value << tag;
			data << YAML::EndMap;
		}

		if (entity.hasComponent<TransformComponent>())
		{
			auto& transformComponent = entity.getComponent<TransformComponent>();

			data << YAML::Key << "TransformComponent";
			data << YAML::BeginMap;
			data << YAML::Key << "Translation" << YAML::Value << transformComponent.translation;
			data << YAML::Key << "Rotation" << YAML::Value << transformComponent.rotation;
			data << YAML::Key << "Scale" << YAML::Value << transformComponent.scale;
			data << YAML::EndMap;
		}

		if (entity.hasComponent<SpriteRendererComponent>())
		{
			auto& spriteComponent = entity.getComponent<SpriteRendererComponent>();

			data << YAML::Key << "SpriteRendererComponent";
			data << YAML::BeginMap;
			data << YAML::Key << "Color" << YAML::Value << spriteComponent.color;
			data << YAML::EndMap;
		}

		if (entity.hasComponent<CameraComponent>())
		{
			auto& cameraComponent = entity.getComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

			data << YAML::Key << "CameraComponent";
			data << YAML::BeginMap;
			data << YAML::Key << "Camera" << YAML::Value;
			data << YAML::BeginMap;
			data << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();

			data << YAML::Key << "perspectiveFOV" << YAML::Value << camera.getPerspectiveFOV();
			data << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.getPerspectiveNearClip();
			data << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.getPerspectiveFarClip();

			data << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
			data << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.getOrthographicNearClip();
			data << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.getOrthographicFarClip();
			data << YAML::EndMap;
			data << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
			data << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;
			data << YAML::EndMap;
		}

		data << YAML::EndMap;
	}

	void SceneSerializer::serialize(const std::string& filepath)
	{
		YAML::Emitter data;
		data << YAML::BeginMap;
		data << YAML::Key << "Scene" << YAML::Value << "Untitled";
		data << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		_scene->_registry.view<entt::entity>().each([&](auto entityID)
		{
			Entity entity { entityID, _scene.get() };
			
			if (!entity)
			{
				return;
			}

			serializeEntity(data, entity);
		});

		data << YAML::EndSeq;
		data << YAML::EndMap;

		std::ofstream file(filepath);
		file << data.c_str();
	}

	void SceneSerializer::serializeRuntime(const std::string& filepath)
	{
		EUCLEDIA_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();
		YAML::Node data = YAML::Load(strStream.str());

		if (!data["Scene"])
		{
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		EUCLEDIA_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				std::string name;
				auto tagComponent = entity["TagComponent"];

				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
				}

				EUCLEDIA_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);
				Entity deserializedEntity = _scene->createEntity(name);
				auto transformComponent = entity["TransformComponent"];

				if (transformComponent)
				{
					auto& component = deserializedEntity.getComponent<TransformComponent>();
					component.translation = transformComponent["Translation"].as<glm::vec3>();
					component.rotation = transformComponent["Rotation"].as<glm::vec3>();
					component.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto spriteComponent = entity["SpriteRendererComponent"];

				if (spriteComponent)
				{
					auto& component = deserializedEntity.addComponent<SpriteRendererComponent>();
					component.color = spriteComponent["Color"].as<glm::vec4>();
				}

				auto cameraComponent = entity["CameraComponent"];

				if (cameraComponent)
				{
					auto& component = deserializedEntity.addComponent<CameraComponent>();
					auto& camera = cameraComponent["Camera"];
					component.camera.setProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());

					component.camera.setPerspectiveFOV(camera["perspectiveFOV"].as<float>());
					component.camera.setPerspectiveNearClip(camera["PerspectiveNearClip"].as<float>());
					component.camera.setPerspectiveFarClip(camera["PerspectiveFarClip"].as<float>());

					component.camera.setOrthographicsSize(camera["OrthographicSize"].as<float>());
					component.camera.setOrthographicNearClip(camera["OrthographicNearClip"].as<float>());
					component.camera.setOrthographicFarClip(camera["OrthographicFarClip"].as<float>());

					component.primary = cameraComponent["Primary"].as<bool>();
					component.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::deserializeRuntime(const std::string& filepath)
	{
		EUCLEDIA_CORE_ASSERT(false, "Not implemented");
		return false;
	}
}