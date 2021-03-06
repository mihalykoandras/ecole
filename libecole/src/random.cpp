#include <mutex>

#include "ecole/random.hpp"

namespace ecole {
namespace {

class RandomEngineManager {
public:
	static auto get() -> RandomEngineManager&;

	auto seed(Seed val) -> void;
	auto spawn() -> RandomEngine;

private:
	std::mutex m;
	Seed user_seed = 0;
	Seed spawn_seed = 0;

	RandomEngineManager();

	auto new_seed_seq() -> std::seed_seq;
};

}  // namespace

auto seed(Seed val) -> void {
	RandomEngineManager::get().seed(val);
}

auto spawn_random_engine() -> RandomEngine {
	return RandomEngineManager::get().spawn();
}

namespace {

auto RandomEngineManager::get() -> RandomEngineManager& {
	static auto random_engine = RandomEngineManager{};
	return random_engine;
}

auto RandomEngineManager::seed(Seed val) -> void {
	auto const lk = std::unique_lock{m};
	user_seed = val;
	spawn_seed = 0;
}

auto RandomEngineManager::spawn() -> RandomEngine {
	auto seeds = new_seed_seq();
	return RandomEngine{seeds};
}

RandomEngineManager::RandomEngineManager() : user_seed{std::random_device{}()} {}

auto RandomEngineManager::new_seed_seq() -> std::seed_seq {
	auto const lk = std::unique_lock{m};
	return {user_seed, ++spawn_seed};
}

}  // namespace
}  // namespace ecole
