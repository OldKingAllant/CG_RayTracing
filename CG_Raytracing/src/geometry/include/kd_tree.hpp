#pragma once

#include <hittable.hpp>
#include <ray.hpp>

#include <vector>
#include <memory>
#include <optional>
#include <functional>

namespace cg_raytracing::geometry {
	struct KDNode {
		KDNode();

		BoundingBox bbox;
		std::optional<size_t> obj_index;
		std::shared_ptr<KDNode> left;
		std::shared_ptr<KDNode> right;
		size_t level;
		size_t num_subnodes;
	};

	struct FlatKDNode {
		BoundingBox           bbox;
		std::optional<size_t> obj_index;
		std::optional<size_t> left, right;
	};

	class KDTree {
	public :
		KDTree(KDTree&& _prev) noexcept;
		KDTree& operator=(KDTree&& _other) noexcept;

		static KDTree CreateFromHittables(std::vector<std::shared_ptr<Hittable>> const& _hittables, float _world_size);

		/// <summary>
		/// Visit all nodes depth-search-first 
		/// </summary>
		/// <param name="_fun">Callback, a reference to the current node is passed</param>
		/// <param name="_leafs_only"></param>
		void VisitDSF(std::function<void(FlatKDNode const&)>&& _fun, bool _leafs_only = false) const;

		/// <summary>
		/// Visit all nodes breadth-search-first 
		/// </summary>
		/// <param name="_fun">Callback, a reference to the current node is passed</param>
		void VisitBSF(std::function<void(FlatKDNode const&)>&& _fun) const;

		/// <summary>
		/// Return list of leaf nodes that the ray intersects
		/// </summary>
		/// <param name="_ray">The ray</param>
		/// <returns>List of nodes</returns>
		std::vector<FlatKDNode const*> RayIntersectsObject(math::Ray const& _ray) const;

		/// <summary>
		/// Get total number of tracked objects in the BVH
		/// </summary>
		/// <returns>Number of objects</returns>
		size_t GetObjectCount() const;

	private :
		KDTree();
		KDTree(KDTree const& _prev) = delete;
		KDTree& operator=(KDTree const& _other) = delete;

	private :
		std::vector<FlatKDNode> m_flat_tree;
		float                   m_world_size;
	};
}