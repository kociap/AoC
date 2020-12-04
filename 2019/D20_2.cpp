#include <deque>
#include <iostream>
#include <string.h>
#include <vector>
#include <queue>

using i64 = long long;

bool is_letter(char c) {
	return c >= 65 && c <= 90;
}

class Vec2 {
public:
	i64 x = 0;
	i64 y = 0;
};

bool operator==(Vec2 v1, Vec2 v2) {
	return v1.x == v2.x && v1.y == v2.y;
}

bool operator!=(Vec2 v1, Vec2 v2) {
	return !(v1 == v2);
}

struct Portal_ID {
	char chars[2];
};

bool operator==(Portal_ID lhs, Portal_ID rhs) {
	return lhs.chars[0] == rhs.chars[0] && lhs.chars[1] == rhs.chars[1];
}

struct Portal {
	Portal_ID id;
	Vec2 pos1;
	Vec2 pos2;
};

void find_portals(std::vector<char> const& map, i64 const width, i64 const height, i64 const ring_width, std::vector<Portal>& portals) {
	auto find_portal = [&portals](Portal_ID id) -> Portal * {
		for (Portal& p : portals) {
			if (p.id == id) {
				return &p;
			}
		}
		return nullptr;
	};

	Vec2 const corners[] = {
		// outside
		{2, 2},         // top-left
		{width - 3, 2}, // top-right
		{2, height - 3}, // bottom-left
		{width - 3, height - 3}, // bottom-right
		// inside
		{2 + (ring_width - 1), 2 + (ring_width - 1)}, // top-left
		{width - 3 - (ring_width - 1), 2 + (ring_width - 1)}, // top-right
		{2 + (ring_width - 1), height - 3 - (ring_width - 1)}, // bottom-left
		{width - 3 - (ring_width - 1), height - 3 - (ring_width - 1)} // bottom-right
	};

	i64 const horizontal_scans[12] = {
		corners[0].x, corners[0].y, corners[1].x,
		corners[2].x, corners[2].y, corners[3].x,
		corners[4].x, corners[4].y, corners[5].x,
		corners[6].x, corners[6].y, corners[7].x
	};

	i64 const vertical_scans[12] = {
		corners[0].x, corners[0].y, corners[2].y,
		corners[1].x, corners[1].y, corners[3].y,
		corners[4].x, corners[4].y, corners[6].y,
		corners[5].x, corners[5].y, corners[7].y
	};

	for (i64 i = 0; i < 12; i += 3) {
		for (i64 x = horizontal_scans[i], y = horizontal_scans[i + 1]; x < horizontal_scans[i + 2]; ++x) {
			if (map[y * width + x] == '.') {
				Portal_ID new_id;
				if (is_letter(map[(y - 1) * width + x])) {
					new_id.chars[0] = map[(y - 2) * width + x];
					new_id.chars[1] = map[(y - 1) * width + x];
				} else {
					new_id.chars[0] = map[(y + 1) * width + x];
					new_id.chars[1] = map[(y + 2) * width + x];
				}

				Portal* portal = find_portal(new_id);
				if (portal) {
					portal->pos2 = Vec2{ x, y };
				} else {
					Portal new_portal;
					new_portal.id = new_id;
					new_portal.pos1 = Vec2{ x, y };
					portals.push_back(new_portal);
				}
			}
		}
	}

	for (i64 i = 0; i < 12; i += 3) {
		for (i64 x = vertical_scans[i], y = vertical_scans[i + 1]; y < vertical_scans[i + 2]; ++y) {
			if (map[y * width + x] == '.') {
				Portal_ID new_id;
				if (is_letter(map[y * width + x - 1])) {
					new_id.chars[0] = map[y * width + x - 2];
					new_id.chars[1] = map[y * width + x - 1];
				} else {
					new_id.chars[0] = map[y * width + x + 1];
					new_id.chars[1] = map[y * width + x + 2];
				}

				Portal* portal = find_portal(new_id);
				if (portal) {
					portal->pos2 = Vec2{ x, y };
				} else {
					Portal new_portal;
					new_portal.id = new_id;
					new_portal.pos1 = Vec2{ x, y };
					portals.push_back(new_portal);
				}
			}
		}
	}
}

struct Connection {
	Vec2 target_pos;
	i64 distance;
};

struct Node {
	Vec2 pos;
	bool outside;
	std::vector<Connection> connections;
};

Node& find_node(std::vector<Node>& nodes, Vec2 pos) {
	for (Node& n : nodes) {
		if (n.pos == pos) {
			return n;
		}
	}
	__builtin_unreachable();
}

i64 find_node_index(std::vector<Node>& nodes, Vec2 pos) {
	for (i64 i = 0; i < nodes.size(); ++i) {
		if (nodes[i].pos == pos) {
			return i;
		}
	}
	__builtin_unreachable();
}

void trace_path(std::vector<char> const& map, i64 const width, i64 const height,
	i64 const x, i64 const y, i64 const orientation, i64 const distance, std::vector<Connection>& connections) {
	char current = map[y * width + x];
	if (current == '#' || (is_letter(current) && distance == 1)) {
		return;
	}

	if (is_letter(current)) {
		switch (orientation) {
		case 0:
			connections.push_back({ {x, y + 1}, distance - 1 });
			break;
		case 1:
			connections.push_back({ {x - 1, y}, distance - 1 });
			break;
		case 2:
			connections.push_back({ {x, y - 1}, distance - 1 });
			break;
		case 3:
			connections.push_back({ {x + 1, y}, distance - 1 });
			break;
		}
		return;
	}

	switch (orientation) {
	case 0:
		trace_path(map, width, height, x, y - 1, 0, distance + 1, connections);
		trace_path(map, width, height, x + 1, y, 1, distance + 1, connections);
		trace_path(map, width, height, x - 1, y, 3, distance + 1, connections);
		break;
	case 1:
		trace_path(map, width, height, x, y - 1, 0, distance + 1, connections);
		trace_path(map, width, height, x + 1, y, 1, distance + 1, connections);
		trace_path(map, width, height, x, y + 1, 2, distance + 1, connections);
		break;
	case 2:
		trace_path(map, width, height, x + 1, y, 1, distance + 1, connections);
		trace_path(map, width, height, x, y + 1, 2, distance + 1, connections);
		trace_path(map, width, height, x - 1, y, 3, distance + 1, connections);
		break;
	case 3:
		trace_path(map, width, height, x, y - 1, 0, distance + 1, connections);
		trace_path(map, width, height, x, y + 1, 2, distance + 1, connections);
		trace_path(map, width, height, x - 1, y, 3, distance + 1, connections);
		break;
	}
}

struct Distance_Graph {
	Distance_Graph(i64 size) : _size(size) {}

	i64& at(i64 level, i64 index) {
		if (graph.size() <= level) {
			graph.resize(level + 1);
			for (std::vector<i64>& v : graph) {
				v.resize(_size, ~0ULL >> 1);
			}
		}

		return graph[level][index];
	}

private:
	std::vector<std::vector<i64>> graph;
	i64 _size;
};

void shortest_path(std::vector<Node>& nodes, Distance_Graph& graph, i64 const aa_index, i64 const zz_index) {
	auto is_blocked = [&nodes, aa_index, zz_index](Node const& n, i64 level) {
		bool blocked_top_level = level == 0 && n.outside && n.pos != nodes[zz_index].pos;
		bool blocked_lower = level != 0 && (n.pos == nodes[aa_index].pos || n.pos == nodes[zz_index].pos);
		return blocked_top_level || blocked_lower;
	};

	struct Search {
		i64 index;
		i64 level;
	};

	std::queue<Search> queue;
	queue.push({ aa_index, 0 });
	while (!queue.empty()) {
		i64 const level = queue.front().level;
		i64 const current = queue.front().index;
		queue.pop();

		for (Connection& c : nodes[current].connections) {
			i64 const next = find_node_index(nodes, c.target_pos);
			i64 next_level;
			if (c.distance == 1) {
				next_level = level - nodes[current].outside + !nodes[current].outside;
			} else {
				next_level = level;
			}

			if (!is_blocked(nodes[next], next_level)) {
				i64 new_dist = c.distance + graph.at(level, current);
				if (new_dist < graph.at(0, zz_index) && new_dist < graph.at(next_level, next)) {
					graph.at(next_level, next) = new_dist;
					queue.push({ next, next_level });
				}
			}
		}
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::vector<char> map;
	i64 width = 0;
	i64 height = 0;
	for (char c = std::cin.get(); !std::cin.eof();) {
		if (c == '\n') {
			height += 1;
		}
		else {
			map.push_back(c);
		}
		c = std::cin.get();
	}
	width = map.size() / height;

	i64 ring_width = 0;
	for (i64 x = 2; map[height / 2 * width + x] == '#' || map[height / 2 * width + x] == '.'; ++ring_width, ++x);

	std::vector<Portal> portals;
	find_portals(map, width, height, ring_width, portals);

	Vec2 aa_pos;
	Vec2 zz_pos;

	auto is_outside = [ring_width, width, height](Vec2 p) {
		return p.x == 2 || p.x == (width - 3) || p.y == 2 || p.y == (height - 3);
	};

	std::vector<Node> nodes;
	for (Portal const p : portals) {
		if (p.id == Portal_ID{ {'A', 'A'} }) {
			aa_pos = p.pos1;
		} else if (p.id == Portal_ID{ {'Z', 'Z'} }) {
			zz_pos = p.pos1;
		}

		if (p.pos2 != Vec2{ 0, 0 }) {
			Node n1{ p.pos1, is_outside(p.pos1) };
			n1.connections.push_back({ p.pos2, 1 });
			nodes.push_back(std::move(n1));

			Node n2{ p.pos2, is_outside(p.pos2) };
			n2.connections.push_back({ p.pos1, 1 });
			nodes.push_back(std::move(n2));
		}
		else {
			nodes.push_back({ p.pos1, is_outside(p.pos1) });
		}
	}

	for (Node& n : nodes) {
		if (map[(n.pos.y - 1) * width + n.pos.x] == '.') {
			trace_path(map, width, height, n.pos.x, n.pos.y, 0, 0, n.connections);
		} else if (map[(n.pos.y + 1) * width + n.pos.x] == '.') {
			trace_path(map, width, height, n.pos.x, n.pos.y, 2, 0, n.connections);
		} else if (map[n.pos.y * width + n.pos.x + 1] == '.') {
			trace_path(map, width, height, n.pos.x, n.pos.y, 1, 0, n.connections);
		} else {
			trace_path(map, width, height, n.pos.x, n.pos.y, 3, 0, n.connections);
		}
	}

	i64 const aa_node = find_node_index(nodes, aa_pos);
	i64 const zz_node = find_node_index(nodes, zz_pos);
	Distance_Graph graph(nodes.size());
	graph.at(0, aa_node) = 0;
	graph.at(0, zz_node) = (~0ULL >> 1);
	shortest_path(nodes, graph, aa_node, zz_node);

	std::cout << graph.at(0, zz_node) << '\n';

	return 0;
}
