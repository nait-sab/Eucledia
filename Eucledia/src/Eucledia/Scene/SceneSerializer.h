#pragma once

#include "Scene.h"

namespace Eucledia
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const ref<Scene>& scene);

		void serialize(const std::string& filepath);
		void serializeRuntime(const std::string& filepath);

		bool deserialize(const std::string& filepath);
		bool deserializeRuntime(const std::string& filepath);

	private:
		ref<Scene> _scene;
	};
}
