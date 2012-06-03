/* A templated dijkstra search algorithm.
 * It assigns a distance value to all reachable
 * nodes in a graph given the node at index
 * source.
 *
 * The function assumes that the template type
 * T has the following 4 public data types:
 *
 * -unsigned int dij_distance
 * 	The algorithm will fill this value with the
 *  calculated solution, or INFINITY if no path
 *  is found.
 *
 * -unsigned int dij_num_children
 *  The number of pointers stored in dij_children
 *
 * -T*[] dij_children
 * 	An array of pointers to all neighboring nodes.
 *
 * -T* prev
 * 	The node previous to this one in the path
 *  found between the source and this node. Will
 *  equal NULL if no path was found.
 *
 */
#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <list>
#include <climits>

#ifndef INFINITY
#define INFINITY UINT_MAX
#endif
template <class T>
void dijkstra_search(T graph[], const size_t graph_size, const size_t source, int (*costFunc)(T*,T*)) {
	std::list<T*> unsearched;

	for(size_t i = 0; i < graph_size; i++) {
		graph[i].dij_distance = INFINITY;
		graph[i].dij_prev = NULL;
		unsearched.push_back(&graph[i]);
	}

	graph[source].dij_distance = 0;

	while(!unsearched.empty()) {
		typename std::list<T*>::iterator uitr = unsearched.begin();
		typename std::list<T*>::iterator litr = unsearched.begin();
		for( ; litr != unsearched.end(); litr++) {
			if((*litr)->dij_distance < (*uitr)->dij_distance) {
				uitr = litr;
			}
		}
		T* u = (*uitr);
		unsearched.erase(uitr);

		if(u->dij_distance == (unsigned int)(INFINITY)) {
			return;
		}


		for(unsigned int ci = 0; ci < u->dij_num_children; ci++) {
			T* v = u->dij_children[ci];
			unsigned int alt = u->dij_distance + costFunc(u, v);
			if(alt < v->dij_distance) {
				v->dij_distance = alt;
				v->dij_prev = u;
			}
		}
	}
}

#endif
