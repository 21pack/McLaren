#include "serializable_world.h"

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>

namespace engine {

void to_json(const SerializableWorld &world, const std::string &filename) {
	std::ofstream os(filename);
	cereal::JSONOutputArchive archive(os);
	archive(CEREAL_NVP(world));
}

SerializableWorld of_json(const std::string &filename) {
	std::ifstream is(filename);
	cereal::JSONInputArchive archive(is);
	SerializableWorld world;
	archive(CEREAL_NVP(world));
	return world;
}

} // namespace engine
