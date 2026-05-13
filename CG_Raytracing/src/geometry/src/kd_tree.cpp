#include <kd_tree.hpp>

#include <algorithm>
#include <cmath>
#include <functional>
#include <stack>
#include <stdexcept>
#include <unordered_set>
#include <deque>

namespace cg_raytracing::geometry {
	namespace detail {
		/// <summary>
		/// Compute 16 bit morton code (interleaved positional
		/// data, normalized based on the size of the
		/// overall bounding box)
		/// </summary>
		/// <param name="_x"></param>
		/// <param name="_y"></param>
		/// <param name="_z"></param>
		/// <param name="_world_size_x"></param>
		/// <param name="_world_size_y"></param>
		/// <param name="_world_size_z"></param>
		/// <returns></returns>
		static uint64_t ComputeMortonCode(float _x, float _y, float _z, float _world_size_x, float _world_size_y, 
			float _world_size_z) {
			auto normalize = [](float _coord, float _world_size) {
				return (_coord + _world_size / 2) / _world_size;
			};

			auto bit_expand = [](uint64_t _val) {
				// - First separate the bytes FFFF -> 00FF 00FF
				// - Separate nibbles 0FF0FF -> 0F0F0F0F
				// - Create pairs of two bits
				// - Isolate all bits
				// - Add another empty bit
				_val = (_val | (_val << 8)) & 0x00FF'00FFULL;
				_val = (_val | (_val << 4)) & 0x0F0F'0F0FULL;
				_val = (_val | (_val << 2)) & 0x3333'3333ULL;
				_val = (_val | (_val << 1)) & 0x5555'5555ULL;
				return _val;
			};

			auto bit_expand64 = [](uint64_t _val) {
				_val = (_val | (_val << 16)) & 0x0000'FFFF'0000'FFFFULL;
				_val = (_val | (_val << 8))  & 0x00FF'00FF'00FF'00FFULL;
				_val = (_val | (_val << 4))  & 0x0F0F'0F0F'0F0F'0F0FULL;
				_val = (_val | (_val << 2))  & 0x3333'3333'3333'3333ULL;
				_val = (_val | (_val << 1))  & 0x5555'5555'5555'5555ULL;
				return _val;
			};

			_x = normalize(_x, _world_size_x);
			_y = normalize(_y, _world_size_y);
			_z = normalize(_z, _world_size_z);

			_x = std::clamp(_x, .0f, 1.f);
			_y = std::clamp(_y, .0f, 1.f);
			_z = std::clamp(_z, .0f, 1.f);

			constexpr auto MAX_VALUE = (float)((1 << 16) - 1);

			_x = std::min(std::floor(_x * MAX_VALUE), MAX_VALUE);
			_y = std::min(std::floor(_y * MAX_VALUE), MAX_VALUE);
			_z = std::min(std::floor(_z * MAX_VALUE), MAX_VALUE);

			auto _morton_x = bit_expand64(bit_expand((uint64_t)_x));
			auto _morton_y = bit_expand64(bit_expand((uint64_t)_y));
			auto _morton_z = bit_expand64(bit_expand((uint64_t)_z));

			return _morton_x | (_morton_y << 1) | (_morton_z << 2);
		}

		static uint64_t ComputeMortonCode(float _x, float _y, float _z, float _world_size) {
			return ComputeMortonCode(_x, _y, _z, _world_size, _world_size, _world_size);
		}
	}

	KDTree::KDTree(KDTree&& _prev) noexcept :
		m_flat_tree{std::move(_prev.m_flat_tree)},
		m_world_size{_prev.m_world_size}
	{}

	KDTree& KDTree::operator=(KDTree && _other) noexcept {
		m_flat_tree = std::move(_other.m_flat_tree);
		m_world_size = _other.m_world_size;
		return *this;
	}

	using SplitFunction = std::function<size_t(std::vector<std::pair<size_t, uint64_t>> const&, size_t, size_t)>;
	static std::pair<std::shared_ptr<KDNode>, size_t> CreateSubTree(
		std::vector<std::pair<size_t, uint64_t>> const& _list, 
		std::vector<std::shared_ptr<Hittable>> const& _hittables,
		size_t _begin, size_t _end, SplitFunction const& _split_function,
		size_t _level = 0) {
		if (_begin == _end) {
			auto leaf = std::make_shared<KDNode>();
			leaf->bbox = _hittables[_list[_begin].first]->GetBoundingBox();
			leaf->obj_index = _list[_begin].first;
			leaf->level = _level;
			leaf->num_subnodes = 0;
			return { leaf, 1 };
		}
		else {
			auto node = std::make_shared<KDNode>();
			auto split_pos = _split_function(_list, _begin, _end);
			auto [l, count_l] = CreateSubTree(_list, _hittables, _begin, split_pos, _split_function, _level + 1);
			auto [r, count_r] = CreateSubTree(_list, _hittables, split_pos + 1, _end, _split_function, _level + 1);

			node->num_subnodes = 2 + l->num_subnodes + r->num_subnodes;
			node->left  = l;
			node->right = r;
			node->bbox = node->left->bbox.Union(node->right->bbox);
			node->level = _level;

			return { node, 1 + count_l + count_r };
		}
	}

	/// <summary>
	/// Create flat kd-tree without constructing a 
	/// temporary pointer-based tree
	/// </summary>
	/// <param name="_list">List of (object index, morton code)</param>
	/// <param name="_hittables">List of objects to be tracked</param>
	/// <param name="_split_function">Function to split between left/right at each non-leaf node</param>
	/// <returns>The flat tree</returns>
	static std::vector<FlatKDNode> CreateTreeIterative(
		std::vector<std::pair<size_t, uint64_t>> const& _list,
		std::vector<std::shared_ptr<Hittable>> const& _hittables,
		SplitFunction const& _split_function) {
		FlatKDNode root{};
		std::vector<FlatKDNode> flat_tree{};

		auto num_codes = _list.size();
		auto num_levels = std::ceilf(std::log2f((float)num_codes)) + 1;
		// Preallocate 2^N nodes
		// If the number of objects is not a power of two,
		// the preallocation is bigger than needed
		auto alloc_size = (size_t)std::powf(2.f, num_levels);

		flat_tree.reserve(alloc_size);
		flat_tree.push_back(root);

		std::stack<std::pair<size_t, size_t>> ranges_to_visit_l{};
		std::stack<std::pair<size_t, size_t>> ranges_to_visit_r{};
		std::stack<size_t> parents{};
		std::stack<size_t> nodes_without_bbox{};

		auto split_point = _split_function(_list, 0, _list.size() - 1);

		ranges_to_visit_l.push({ 0, split_point });
		ranges_to_visit_r.push({ split_point + 1, _list.size() - 1 });

		parents.push(0);

		while (!ranges_to_visit_l.empty() || !ranges_to_visit_r.empty()) {
			std::pair<size_t, size_t> curr_range{};
			// Are we at the right of a node?
			bool is_r = false;
			// Pop a range to visit either from the left
			// or the right
			if (!ranges_to_visit_l.empty()) {
				curr_range = ranges_to_visit_l.top();
				ranges_to_visit_l.pop();
			}
			else {
				curr_range = ranges_to_visit_r.top();
				ranges_to_visit_r.pop();
				is_r = true;
			}

			FlatKDNode new_node{};
			if (curr_range.first == curr_range.second) {
				// We reached a leaf, assign bbox and index
				new_node.bbox = _hittables[_list[curr_range.first].first]->GetBoundingBox();
				new_node.obj_index = _list[curr_range.first].first;
			}

			auto curr_pos = flat_tree.size();
			flat_tree.push_back(new_node);

			// Get index of parent node
			auto parent_index = parents.top();
			if (is_r) {
				// Right node
				{
					auto& parent = flat_tree[parent_index];
					// Assign index of this node
					parent.right = curr_pos;
					if (curr_range.first == curr_range.second) {
						// If this node is a leaf, we can complete 
						// the parent node with the merged bbox
						parent.bbox = flat_tree[parent.left.value()].bbox.Union(flat_tree[parent.right.value()].bbox);
					}
					else {
						// Else we still need to traverse other nodes before 
						// completing the node
						nodes_without_bbox.push(parent_index);
					}
				}

				parents.pop();
			}
			else {
				// Left node
				flat_tree[parent_index].left = curr_pos;
			}

			if (curr_range.first != curr_range.second) {
				// This range is yet not complete
				// Split it again
				auto split_point = _split_function(_list, curr_range.first, curr_range.second);

				ranges_to_visit_l.push({ curr_range.first, split_point });
				ranges_to_visit_r.push({ split_point + 1, curr_range.second });

				parents.push(curr_pos);
			}
		}

		// Complete the flat tree by grouping the various boxes
		// from the bottom to top (considering only the nodes
		// that are still without the bbox)
		while (!nodes_without_bbox.empty()) {
			auto curr_node = nodes_without_bbox.top();

			if (0 != flat_tree[curr_node].bbox.Volume()) {
				// Already complete, probably this
				// node has been reinsterted in the stack
				nodes_without_bbox.pop();
				continue;
			}

			// Get the bboxes of the left and right nodes

			auto bbox_left = flat_tree[flat_tree[curr_node].left.value()].bbox;
			auto bbox_right = flat_tree[flat_tree[curr_node].right.value()].bbox;

			// Is any of the boxes missing?

			auto missing_left = 0 == bbox_left.Volume();
			auto missing_right = 0 == bbox_right.Volume();

			// If a box is missing:
			// Reinsert possibly duplicate indices before the current index

			if (missing_right) {
				nodes_without_bbox.push(flat_tree[curr_node].right.value());
			}

			if (missing_left) {
				nodes_without_bbox.push(flat_tree[curr_node].left.value());
			}

			// None of the nodes has missing bbox: merge the two

			if (!missing_left && !missing_right) {
				nodes_without_bbox.pop();
				flat_tree[curr_node].bbox = bbox_left.Union(bbox_right);
			}	
		}

		return flat_tree;
	}

	static std::vector<FlatKDNode> CreateFlattenedTree(std::shared_ptr<KDNode> _tree, size_t _node_count) {
		std::vector<FlatKDNode> flat_tree{};
		flat_tree.reserve(_node_count);

		std::stack<std::shared_ptr<KDNode>> nodes_to_visit{};
		nodes_to_visit.push(_tree);

		while (!nodes_to_visit.empty()) {
			auto curr_node = nodes_to_visit.top();
			nodes_to_visit.pop();

			auto i = flat_tree.size();
			auto level = curr_node->level + 1;

			FlatKDNode new_node{};
			new_node.bbox = curr_node->bbox;
			new_node.obj_index = curr_node->obj_index;
			new_node.left = std::nullopt;
			new_node.right = std::nullopt;
			
			if (!curr_node->obj_index.has_value()) {
				//non-leaf
				if (curr_node->right) { nodes_to_visit.push(curr_node->right); }
				if (curr_node->left) { nodes_to_visit.push(curr_node->left);   }

				new_node.left = i + 1;
				auto right_offset = curr_node->left ? curr_node->left->num_subnodes : 0;
				// Sub node count does not include (obviously) the current node and the left child,
				// so we add 2
				new_node.right = i + right_offset + 2;
			}
			
			flat_tree.push_back(new_node);
		}

		return flat_tree;
	}

	/// <summary>
	/// Create kd-tree given the objects to track and the max coord size for all
	/// dimensions
	/// </summary>
	/// <param name="_hittables">List of objects</param>
	/// <param name="_world_size">Max coord size</param>
	/// <returns>The tree</returns>
	KDTree KDTree::CreateFromHittables(std::vector<std::shared_ptr<Hittable>> const& _hittables, float _world_size) {
		std::vector<std::pair<size_t, uint64_t>> code_list{};
		code_list.reserve(_hittables.size());

		for (size_t index = 0; auto const& object : _hittables) {
			auto box = object->GetBoundingBox();
			auto center = box.pos;
			auto morton_code = detail::ComputeMortonCode(center.x, center.y, center.z, _world_size);
			code_list.push_back({ index, morton_code });
			index++;
		}

		// Sort by spatial locality
		std::sort(code_list.begin(), code_list.end(), [](auto _l, auto _r) { return _r.second > _l.second; });

		// Simple middle split between lef/right
		auto split_middle = [](std::vector<std::pair<size_t, uint64_t>> const&, size_t _begin, size_t _end) {
			return (_begin + _end) / 2;
		};

		//auto [tree, node_count] = CreateSubTree(code_list, _hittables, 0, code_list.size() - 1, split_middle);
		//auto flattened = CreateFlattenedTree(tree, node_count);

		auto flattened = CreateTreeIterative(code_list, _hittables, split_middle);

		auto kd_tree = KDTree();
		kd_tree.m_flat_tree = std::move(flattened);
		kd_tree.m_world_size = _world_size;

		return kd_tree;
	}

	void KDTree::VisitDSF(std::function<void(FlatKDNode const&)>&& _fun, bool _leafs_only) const {
		std::stack<size_t> nodes_to_visit{};
		std::unordered_set<size_t> visited_nodes{};

		// Start with the root
		nodes_to_visit.push(0);

		while (!nodes_to_visit.empty()) {
			// Get index in flat tree
			auto curr_node_index = nodes_to_visit.top();
			if (visited_nodes.contains(curr_node_index)) {
				// Somehow the node was already visited
				nodes_to_visit.pop();
				continue;
			}

			auto const& node = m_flat_tree[curr_node_index];
			if (node.obj_index.has_value()) { // Leaf 
				// Just call the function, no more
				// links to follow in this subtree
				_fun(node);
				visited_nodes.insert(curr_node_index);
				nodes_to_visit.pop();
				continue;
			}

			auto l = node.left;
			auto r = node.right;

			// Check if either subtree is unexplored
			// (and that it is explorable)

			auto is_l_not_visited = l.has_value() && !visited_nodes.contains(l.value());
			auto is_r_not_visited = r.has_value() && !visited_nodes.contains(r.value());

			// First append right node, then left, so the next
			// iteration will pick the left one first

			if (is_r_not_visited) {
				nodes_to_visit.push(r.value());
			}

			if (is_l_not_visited) {
				nodes_to_visit.push(l.value());
			}

			if (!is_l_not_visited && !is_r_not_visited) {
				// All subtrees have been visited
				// Call the function on this node
				if (!_leafs_only) {
					_fun(node);
				}
				
				visited_nodes.insert(curr_node_index);
				nodes_to_visit.pop();
			}
		}
	}

	void KDTree::VisitBSF(std::function<void(FlatKDNode const&)>&& _fun) const {
		// Use fifo
		std::deque<size_t> nodes_to_visit{};
		nodes_to_visit.push_front(0);

		while (!nodes_to_visit.empty()) {
			auto curr_node_index = nodes_to_visit.back();

			auto const& node = m_flat_tree[curr_node_index];
			// Breadth-first, visit node immediately
			_fun(node);
			nodes_to_visit.pop_back();

			auto l = node.left;
			auto r = node.right;

			// Check if either subtree is unexplored
			// (and that it is explorable)

			auto is_l_not_visited = l.has_value();
			auto is_r_not_visited = r.has_value();

			if (is_l_not_visited) {
				nodes_to_visit.push_front(node.left.value());
			}

			if (is_r_not_visited) {
				nodes_to_visit.push_front(node.right.value());
			}
		}
	}

	std::optional<FlatKDNode const*> KDTree::RayIntersectsObject(math::Ray const& _ray) const {
		return std::nullopt;
	}

	KDTree::KDTree() :
		m_flat_tree{},
		m_world_size{} {}

	KDNode::KDNode() :
		bbox{},
		obj_index{},
		left{},
		right{},
		level{},
		num_subnodes{}
	{}
}