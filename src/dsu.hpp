#pragma once

// Source: https://usaco.guide/gold/dsu?lang=cpp#implementation

#include <vector>
#include <unordered_set>

namespace ske {

struct dsu {
  private:
	mutable std::vector<unsigned int> parents;
	std::vector<unsigned int> sizes;
	mutable std::unordered_set<unsigned int> groups;
	mutable bool groupsChanged;
  public:
	dsu(unsigned int size) 
        : parents(size)
        , sizes(size, 1)
        , groupsChanged(true) 
    {
		for (unsigned int i = 0; i < size; i++) 
            parents[i] = i;
	}
	unsigned int find(unsigned int x) const {
		return parents[x] == x ? x : (parents[x] = find(parents[x]));
	}
	void unite(unsigned int x, unsigned int y) {
		unsigned int x_root = find(x);
		unsigned int y_root = find(y);
		if (x_root == y_root)
            return;
		groupsChanged = true;
		if (sizes[x_root] < sizes[y_root]) 
            std::swap(x_root, y_root);
		sizes[x_root] += sizes[y_root];
		parents[y_root] = x_root;
	}
	bool is_same_set(unsigned int x, unsigned int y) const { 
        return find(x) == find(y); 
    }
	const std::unordered_set<unsigned int>& leaders() const {
		if (groupsChanged) {
			groupsChanged = false;
			groups.clear();
			for (unsigned int i = 0; i < sizes.size(); ++i)
				groups.insert(find(i));
		}
		return groups;
	}
};

} // namespace ske