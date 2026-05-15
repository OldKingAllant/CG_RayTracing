#include <kd_tree.hpp>
#include <hittable.hpp>

#include <memory>
#include <vector>
#include <optional>

namespace cg_raytracing::scene {
	enum class WorldError {
		OUT_OF_BOUNDS
	};

	/// <summary>
	/// Class that contains all the objects and 
	/// the kd-tree that divides those objects
	/// in a hierarchy
	/// </summary>
	class World : public geometry::Hittable {
	public :
		World(World&& _prev) noexcept;
		World& operator=(World&& _other) noexcept;

		/// <summary>
		/// Create world with coordinates that
		/// go from -_world_size/2 to +_world_size/2
		/// </summary>
		/// <param name="_world_size">Size of each coordinate</param>
		/// <returns>Za warudo</returns>
		static World CreateEmpty(float _world_size);

		/// <summary>
		/// Get overarching bounding box
		/// </summary>
		/// <returns>The bbox</returns>
		geometry::BoundingBox GetBoundingBox() const override;

		/// <summary>
		/// Add single object to the hierarchy. Remember to call 
		/// UpdateTree before using any other method
		/// </summary>
		/// <param name="_obj">The object to add</param>
		/// <returns>Error if any</returns>
		std::optional<WorldError> AddObject(std::shared_ptr<geometry::Hittable> _obj);

		/// <summary>
		/// Call this after adding new objects and before
		/// calling other methods
		/// </summary>
		void UpdateTree();

	private :
		World();
		World& operator=(World const& _other) = delete;

	private :
		float                                            m_world_size;
		std::vector<std::shared_ptr<geometry::Hittable>> m_objects;
		std::unique_ptr<geometry::KDTree>                m_kd_tree;
		bool                                             m_update_tree;
	};
}