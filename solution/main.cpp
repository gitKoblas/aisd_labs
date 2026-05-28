#include "hash_table.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <unordered_map>

using namespace std;

template<typename Vertex, typename Distance = double>
class Graph {
private:
    hash_table<shared_ptr<hash_table<Distance>>> adj_map;
    hash_table<Vertex> id_to_vertex;
public:
    Graph(int size) : adj_map(size), id_to_vertex(size) {}
    struct Edge {
        Vertex from;
        Vertex to;
        Distance weight;
        Edge(Vertex f, Vertex t, Distance w) : from(f), to(t), weight(w) {}
    };

    bool has_vertex(const Vertex& v) const {
        return id_to_vertex.search(v) != nullptr;
    }

    bool add_vertex(const Vertex& v) {
        if (id_to_vertex.search(v))
            return false;
        if (!id_to_vertex.insert(v, v))
            return false;
        adj_map.insert(v, nullptr);
        return true;
    }
	bool remove_vertex(const Vertex& v) {
        if (!id_to_vertex.search(v)) {
            return false;
        }
        if (!id_to_vertex.erase(v)) {
            return false;
        }
        adj_map.erase(v);
        auto adj_pairs = adj_map.items();
        for (const auto& pair : adj_pairs) {
            auto adj_ptr = pair.second; 
            if (adj_ptr) {
                adj_ptr->erase(v);
            }
        }
        return true;
    }
	std::vector<Vertex> vertices() const {
        vector<Vertex> verts;
        auto items = id_to_vertex.items();
        for (const auto& p : items) {
            verts.push_back(p.second);
        }
        return verts;
    }
	bool has_edge(const Vertex& from, const Vertex& to) const{
		auto adj_ptr = adj_map.search(from);
    	if (!adj_ptr || !(*adj_ptr)) {
        	return false;
    	}
    	return (*adj_ptr)->search(to) != nullptr;
	}
	bool has_edge(const Edge& e) const {
		auto adj_ptr = adj_map.search(e.from);
		if (!adj_ptr || !(*adj_ptr)) {
			return false;
		}
		auto end_ptr = (*adj_ptr)->search(e.to);
		return end_ptr && (*end_ptr == e.weight);
	}
	void add_edge(const Vertex& from, const Vertex& to, const Distance& d){
		if(!id_to_vertex.search(from) || !id_to_vertex.search(to)){
			return;
		}
		auto adj_ptr = adj_map.search(from);
		if(!(*adj_ptr)){
			*adj_ptr = make_shared<hash_table<Distance>>(adj_map.get_size());
		}
		(*adj_ptr)->insert_or_assign(to, d);
    	return;
	}
	bool remove_edge(const Vertex& from, const Vertex& to){
		if(!has_edge(from, to)){
			return false;
		}
		auto adj_ptr = adj_map.search(from);
		return (*adj_ptr)->erase(to);
	}
	bool remove_edge(const Edge& e){
		if(!has_edge(e)){
			return false;
		}
		auto adj_ptr = adj_map.search(e.from);
		return (*adj_ptr)->erase(e.to);
	}
	std::vector<Edge> edges(const Vertex& vertex) const{
		std::vector<Edge> edge;
		auto adj_ptr = adj_map.search(vertex);
		if (!adj_ptr || !(*adj_ptr)) return edge;

		auto items = (*adj_ptr)->items();
		for (const auto& p : items) {
			int to = p.first;
			Distance w = p.second;
			edge.push_back(Edge(vertex, to, w));
		}
		return edge;
	}
	size_t order() const{
    	return id_to_vertex.items().size();
	}
	size_t degree(const Vertex& v) const{
		if (!has_vertex(v)) {
        	return 0; 
    	}
		auto adj_ptr = adj_map.search(v);
		if (!adj_ptr || !(*adj_ptr)) {
			return 0;
		}
		return (*adj_ptr)->items().size();
	}
	std::vector<Vertex> walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action) const {
		std::vector<Vertex> visited;
		if (!has_vertex(start_vertex)) {
			return visited;
		}
		std::vector<Vertex> queue;
		queue.push_back(start_vertex);
		visited.push_back(start_vertex);
		int counter = 0;
		while (counter < queue.size()) {
			Vertex curr = queue[counter];
			if (action) {
				action(curr);
			}
			std::vector<Edge> out_edges = edges(curr);
			for (const auto& e : out_edges) {
				if (find(visited.begin(), visited.end(), e.to) == visited.end()) {
					visited.push_back(e.to);
					queue.push_back(e.to);
				}
			}
			++counter;
		}
		return visited;
	}
	bool is_connected() const{
		size_t ord = order();
		if (ord <= 1) {
			return true;
		}
		std::vector<Vertex> verts = vertices();
		Vertex start = verts[0];
		std::vector<Vertex> visited = walk(start, nullptr);
		if (visited.size() != ord) {
			return false;
		}
		std::vector<Vertex> queue;
        std::vector<Vertex> visited_backward; 
        queue.push_back(start);
        visited_backward.push_back(start);
        size_t head = 0;
        
        while (head < queue.size()) {
            Vertex curr = queue[head++];
            for (const Vertex& v : verts) {
                if (std::find(visited_backward.begin(), visited_backward.end(), v) == visited_backward.end()) {
                    if (has_edge(v, curr)) {
                        visited_backward.push_back(v);
                        queue.push_back(v); 
                    }
                }
            }
        }
        return visited_backward.size() == ord; 
    }
	std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
		std::vector<Edge> path;
		if (!has_vertex(from) || !has_vertex(to)) {
			return path; 
		}
		std::vector<Vertex> verts = vertices();
		int n = verts.size();
		std::unordered_map<Vertex, Distance> dist;
		std::unordered_map<Vertex, bool> used;
		std::unordered_map<Vertex, Vertex> parent;
		const Distance INF_VAL = static_cast<Distance>(1e18);
		for (const Vertex& v : verts) {
			dist[v] = INF_VAL;
			used[v] = false;
		}
		dist[from] = 0;
		for (int i = 0; i < n; i++) {
			Vertex v;
			bool found = false;
			for (const Vertex& j : verts) {
				if (!used[j] && (!found || dist[j] < dist[v])) {
					v = j;
					found = true;
				}
			}
			if (!found || dist[v] == INF_VAL)
				break;
			if (v == to)
				break;
			used[v] = true;
			for (const Edge& edge : edges(v)) {
				Vertex neighbor = edge.to;
				Distance w = edge.weight;
				if (dist[v] + w < dist[neighbor]) {
					dist[neighbor] = dist[v] + w;
					parent[neighbor] = v;
				}
			}
		}
		if (dist[to] == INF_VAL) {
			return path;
		}

		Vertex curr = to;
		while (curr != from) {
			Vertex prev = parent[curr];
			Distance edge_weight = 0;
			for (const Edge& e : edges(prev)) {
				if (e.to == curr) {
					edge_weight = e.weight;
					break;
				}
			}
			path.push_back(Edge(prev, curr, edge_weight));
			curr = prev;
		}
		std::reverse(path.begin(), path.end());
		return path;
	}
};

template<typename Vertex, typename Distance>
std::ostream& operator<<(std::ostream& os, const Graph<Vertex, Distance>& gr) {
    auto verts = gr.vertices();
    if (verts.empty()) {
        os << " " << std::endl;
        return os;
    }
    for (const auto& v : verts) {
        os << "Vertex " << v << " -> ";
        auto v_edges = gr.edges(v);
        if (v_edges.empty()) {
            os << " ";
        } 
		else {
            for (size_t i = 0; i < v_edges.size(); ++i) {
                os << " " << v_edges[i].to << ", weight: " << v_edges[i].weight << "; ";
            }
        }
        os << endl;
    }
    return os;
}

template<typename Vertex, typename Distance = double>
Vertex find_storage(const Graph<Vertex, Distance>& gr) {
    std::vector<Vertex> verts = gr.vertices();
    if (verts.empty()) {
        return Vertex();
    }
    Vertex storage = verts[0];
    std::unordered_map<Vertex, Distance> dist;
    Distance min_sum; 
    bool is_first = true;
    for (const Vertex& from : verts) {
        dist[from] = 0;
        for (const Vertex& to : verts) {
            Distance weight = 0; 
            std::vector<typename Graph<Vertex, Distance>::Edge> path = gr.shortest_path(from, to);
            for (const auto& ed : path) {
                weight += ed.weight;
            }
            dist[from] += weight;
        }
        if (is_first || dist[from] < min_sum) {
            min_sum = dist[from];
            storage = from;
            is_first = false;
        }
    }
    return storage;
}

int main(){
	Graph<int> gr(10);
	for (int i = 1; i <= 10; ++i) {
        gr.add_vertex(i);
    }
	gr.add_edge(1, 2, 4.0);
    gr.add_edge(2, 1, 4.0);
    gr.add_edge(2, 3, 3.0);
    gr.add_edge(3, 2, 3.0);
    gr.add_edge(3, 4, 5.0);
    gr.add_edge(4, 3, 5.0);
    gr.add_edge(4, 5, 1.5);
    gr.add_edge(5, 4, 1.5);
    gr.add_edge(5, 6, 2.0);
    gr.add_edge(6, 5, 2.0);
    gr.add_edge(6, 7, 4.0);
    gr.add_edge(7, 6, 4.0);
    gr.add_edge(7, 8, 3.0);
    gr.add_edge(8, 7, 3.0);
    gr.add_edge(8, 9, 6.0);
    gr.add_edge(9, 8, 6.0);
    gr.add_edge(9, 10, 2.0);
    gr.add_edge(10, 9, 2.0);
    gr.add_edge(10, 1, 5.0);
    gr.add_edge(1, 10, 5.0);
    gr.add_edge(5, 1, 2.0);
    gr.add_edge(1, 5, 2.0);
    gr.add_edge(5, 8, 2.5);
    gr.add_edge(8, 5, 2.5);
	cout << "Storage addres: " << find_storage(gr) << endl;
	cout << "degree: " << gr.degree(1) << endl;
	cout << "has edge(1,2): " << gr.has_edge(1, 2) << endl;
	cout << "has vertex(1): "<< gr.has_vertex(1) << endl;
	cout << "order: " << gr.order() << endl;
	cout << "is connected: " << gr.is_connected() << endl;
	cout << gr << endl;
	gr.remove_vertex(10);
	cout << gr << endl;
	gr.remove_edge(8, 9);
	cout << gr << endl;
	auto path = gr.shortest_path(1, 8);
    int weight = 0;
    for (const auto& edge : path) {
        std::cout << " " << edge.from << " -> " << edge.to << "; ";
        weight += edge.weight;
    }
    cout << endl << "weight: " << weight << std::endl;
	return 0;
}