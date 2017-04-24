// includes
#include <iostream>	// basics
#include <string>	// getline
#include <cstdlib>	// atoi
#include <iomanip>	// setw
using namespace std;

// vertex class
struct vertex {
	vertex(): v_num(0), next(0x0) {}

	int v_num;
	struct vertex* next;
};

// Algorithm
// Use the head of the struct vertex array and traverse for specific vertex
// Connect the last 'next' pointer to the passed in vertex node
// Each entry is a linked list
void insertion (struct vertex* v_ptr, int v_index, int vertices, struct vertex* v_node) {
	bool found = false;
	struct vertex* tmp = 0x0;

	for (int i=0; i<vertices; i++, v_ptr++) {
		if (v_ptr->v_num == v_index) {
			found = true;
			tmp = v_ptr;
			for (; tmp->next!=0x0; tmp = tmp->next) {}
			tmp->next = v_node;
		}

		if (found) { break; }
	}

	return;
}

// temporary vertex print
void vertex_print (struct vertex* v_ptr, int vertices) {
	struct vertex* tmp = 0x0;

	for (int i=0; i<vertices; i++,v_ptr++) {
		tmp = v_ptr;
		cout << tmp->v_num;
		if (tmp->next != 0x0) {
			tmp = tmp->next;
			for (; tmp->next!=0x0; tmp = tmp->next) { cout << " -> " << tmp->v_num; }
			cout << " -> " << tmp->v_num;
		}
		cout << "\n";
	}

	return;
}

// temporary queue print
// helper function
void queue_print(int* q_ptr, int reverse = 0) {
	if (!reverse) {
		while (*q_ptr != -1) {
			cout << *q_ptr << " ";
			q_ptr++;
		}
	}
	else {
		int cnt = 0;
		while (*q_ptr != -1) { cnt++, q_ptr++; }
		q_ptr--;
		for (int i=0; i<cnt; i++,q_ptr--) {
			cout << *q_ptr;
			if (i != (cnt - 1)) { cout << " -> "; }
		} 	
	}
	cout << "\n";

	return;
}	

// Algorithm
// Uses a vertex number 'v_num' as the key to return 
// the specific pointer to the struct vertex
// Needed to find all the connected edges for a vertex
struct vertex* get_vertex(struct vertex* v_ptr, int n) {
	bool found = false;
	struct vertex* tmp = v_ptr;
	struct vertex* ptr = 0x0;

	while (!found) {
		if (tmp->v_num == n) {
			found = true;
			ptr = tmp;
		}

		tmp++;
	}

	return ptr;			
}

// helper functions
bool all_visited(bool[], int);
int* push(int*, int, int);
int* pop(int*, int);
int front(int*);

// Algorithm
// Essentially my version of a breadth first search
// Created a makeshift queue using arrays and modified functions push, pop, and front
// Recursively mark all visited nodes and prints order of visited
void bfs (bool visited[], struct vertex* v_ptr, struct vertex* root, int vertices, int& comp_num, int wherefrom[], int flag) {
	int size = 1, value = 0, num = 0, component_num = comp_num + 1;
	int* queue = 0x0;
	struct vertex* tmp = 0x0;
	
	visited[v_ptr->v_num] = true;	
	queue = push(queue, size++, v_ptr->v_num); 
	
	while(*queue != -1) {
		value = front(queue);
		//cout << value << " ";	// Not sure if professor wants order of vertices visited (popped from queue)
		num++;
		queue = pop(queue, size--);

		tmp = get_vertex(root, value);
		tmp = tmp->next;		

		if (tmp != 0x0) {
			for(; tmp->next!=0x0; tmp = tmp->next) {
				if (visited[tmp->v_num] == false) {
					queue = push(queue, size++, tmp->v_num);
					visited[tmp->v_num] = true;
					wherefrom[tmp->v_num] = value;
				}
			}
			// sanity check
			// dont have a real termination for connected edges
			// therefore algorithm stops short (not efficient but effective)
			if (visited[tmp->v_num] == false) {
				queue = push(queue, size++, tmp->v_num);
				visited[tmp->v_num] = true;
				wherefrom[tmp->v_num] = value;
			}
		}
	}

	if (!flag) { cout << "Component " << component_num << ": " << num << "\n"; }
	//queue_print(queue);

	// Recursively marks all visited nodes
	// Unconnected graphs get their own breadth first search
	while (!all_visited(visited, vertices)) {
		for (int i=0; i<vertices; i++,v_ptr++) {
			if (visited[v_ptr->v_num] == false) { 
				bfs(visited, v_ptr, root, vertices, component_num, wherefrom, flag); 
			}
		}
	}

	return;
}

// Gut check - checks if all vertices are marked as visited
// Better implementation than having the vertices have a bool variable in the struct
bool all_visited (bool* array, int vertices) {
	for (int i=0; i<vertices; i++) {
		if (array[i] == false) { return false; }
	}

	return true;
}

// Algorithm
// Resize the array and push the value at the end of the queue
// Important - termination value is -1
int* push (int* q, int size, int value) {
	int* copy = q;
	int* qu = new int[size+1];
	int* tmp = qu;

	for (int i=0; i<size; i++,tmp++,copy++) {
		if (i == (size - 1)) { *tmp = value; }
		else {
			*tmp = *copy;
		}
	}

	*tmp = -1;
	return qu;
}

// Algorithm
// Resize the array and move pointer to point at the next element
// The top of the queue is supposed to "disappear"
// Important - termination value is -1
int* pop (int* q, int size) {
	int* copy = q;
	int* qu = new int[size+1];
	int* tmp = qu;
	copy++;

	for (int i=0; i<size; i++,tmp++,copy++) {
		*tmp = *copy;
	}

	*tmp = -1;
	return qu;
}

// ONLY FUNCTION - returns the value sitting at the immediate pointer location
int front (int* q) {
	return *q;
}

int max_edges (struct vertex*, int[], int);
int get_digits(int);
void degree_distribution_graph (struct vertex* v_ptr, int vertices) {
	int x_incr = (int)((double)vertices * 0.05);
	int edges[vertices];
	for (int i=0; i<vertices; i++) { edges[i] = 0; }
	int current_max = max_edges(v_ptr, edges, vertices);
	int y_incr = (int)((double)current_max * 0.05);
	int setw_value = get_digits(current_max);
	//int setw_value2 = get_digits(vertices);
	
	//for (int i=0; i<vertices; i++) { cout << "Edge " << i << ": " << edges[i] << endl; }	

	char histogram[20][20];
	cout << "\n";

	if (vertices < 20) { 
		return;
	}
	else {
		for (int i=current_max,j=0; i>=0 && j<20; i-=y_incr,j++) {
			int r1 = i, r2 = i - y_incr;
			r2 = (r2 <= 0 || j == 19) ? 0 : r2;
			cout << right << setw(setw_value) << r2 << "-" << left << setw(setw_value) << r1 << " | " << endl;
		}
		cout << right << setw((setw_value * 2) + 3) << "|" << setw(120) << setfill('-') << "" << endl;
		cout << setw((setw_value * 2) + 3) << setfill(' ') << "|" << " ";
		for (int i=0,j=0; i>vertices,j<20; i+=x_incr,j++) {
			int r1 = i;
			r1 = (j == 19) ? vertices : r1;
			cout << "<" << setw(get_digits(r1)) << r1 << " ";
		}
		cout << "\n";
	}

	return;
}

int max_edges (struct vertex* v_ptr, int edges[], int vertices) {
	int cnt = 0, max = 0, node = 0;
	struct vertex* tmp = 0x0;

	for (int i=0; i<vertices; i++,v_ptr++) {
		cnt = 0;
		tmp = v_ptr;
		if (tmp->next != 0x0) {
			tmp = tmp->next;
			for (; tmp->next!=0x0; tmp = tmp->next) { cnt++; }
			cnt++;
		}
		edges[i] = cnt;

		node = (cnt > max) ? i : node;
		max = (cnt > max) ? cnt : max;
	}

	//cout << "Node: " << node << endl;
	return max;
}

int get_digits (int n) {
	int digits = 0;

	if (n<0) { return -1; }
	for (; n>0; n/=10) { ++digits; }

	return digits;
}

void degree_distribution (struct vertex* v_ptr, int vertices) {
	int cnt = 0;
	int edges[vertices];
	for (int i=0; i<vertices; i++) { edges[i] = 0; }

	int max = max_edges(v_ptr, edges, vertices);

	for (int i=0; i<=max; i++) {
		cnt = 0;
		for (int j=0; j<vertices; j++) {
			if (edges[j] == i) { cnt++; }
		}
		
		if (cnt>0) { cout << i << "\t" << cnt << endl; }
	}

	cout << "\n";	
	return;
}

int main() {
	// variables
	struct vertex* v = 0x0;
	struct vertex* tmp = 0x0;
	int cnt = 0, cmd_cnt = 0;
	int vertices = 0, edges = 0;
	int int1 = 0, int2 = 0;
	int comp_num = 0;
	string line = "";

	while (!cin.eof()) {
		if (cin.eof()) { break; } // sanity check

		cnt++;
		if (cnt == 1) {
			if (cin.eof()) { break; } // sanity check
			cin >> vertices;

			// intialize struct vertex array
			// use 'v' as the head (dont modify 'v')
			// use 'tmp' as iterator
			v = new struct vertex[vertices] ();
			tmp = v;

			// update key
			// similar to hashtable but not 2D array
			// our implementation is an array of struct vertex types
			for (int i=0; i<vertices; i++, tmp++) {
				tmp->v_num = i;
			}
		}
		
		if (cnt == 2) {
			// currently used for file traversal
			// potentially will be used in max_edges = (vertices*(vertices-1))/2
			// maximum amount of edges a vertex can have - 
			// worst case scenario is we use max_edges to build a 2D array 
			if (cin.eof()) { break; } // sanity check
			cin >> edges;
		}
		
		if (cnt > 2 && cnt <= (edges + 2)) {
			if (cin.eof()) { break; } //sanity check
			cin >> int1 >> int2;

			// no self loops
			if (int1 != int2) {
				struct vertex* node = new struct vertex;
				// in case constructor is never called on pointer reference
				node->v_num = int2;
				node->next = 0x0;

				insertion (v, int1, vertices, node);
			}
		}

		// command (cmd) parsers and functions
		if (cnt > (edges + 2) && cmd_cnt == 0) {
			cin >> cmd_cnt;
			cout << endl;
		} else if (cnt > (edges + 2) && cmd_cnt != 0) {
			//getline(cin, line);
			for (int i=0; i<cmd_cnt; i++) {
				cin >> line;
				if (line == "degree-distribution") {
					cout << setfill('-') << setw(20) << "" << setfill('-') << setw(50) << left << " Degree Distribution Analysis " << endl;
					//degree_distribution_graph(v, vertices);
					//cout << "\n";
					degree_distribution(v, vertices);
				}
				if (line == "components" || line == "shortest-path") {
					bool visited[vertices];
					int flag = 0, s = 0, t = 0;
					int wherefrom[vertices];
					struct vertex* tmp = v;
					for (int i=0; i<vertices; i++) { visited[i] = false; }
					for (int i=0; i<vertices; i++) { wherefrom[i] = -1; }					
 
					if (line == "components") {
						cout << setfill('-') << setw(20) << "" << setfill('-') << setw(50) << left << " Components Analysis " << endl;
					}
					else {
						flag = 1;
						cin >> s >> t;
						cout << setfill('-') << setw(20) << "" << setfill('-') << setw(50) << left << " Shortest Path Analysis" << endl;
						cout << "source (s): vertex " << s << "\tdestination(t): vertex " << t << "\n\n";
					
						for (int i=0; i<s; i++) { tmp++; }
					}
					
					bfs(visited, tmp, v, vertices, comp_num, wherefrom, flag);

					if (line == "shortest-path") {
						//for (int i=0; i<vertices; i++) { cout << "wherefrom[" << i << "]: " << wherefrom[i] << endl; }
						int* queue = 0x0;
						int size = 1, i = t;
						bool s_check = false, t_check = false;
						queue = push(queue, size++, t);
						while (wherefrom[i] != -1) {
							if (i == s) { s_check = true; }
							if (i == t) { t_check = true; }
							queue = push(queue, size++, wherefrom[i]);
							i = wherefrom[i];
						}
						if (i == s) { s_check = true; }
						if (i == t) { t_check = true; }
						//queue_print(queue);
						if (s_check && t_check) { queue_print(queue, 1); }
						else { cout << "vertex " << s << " and vertex " << t << " are not connected" << endl; }
					}
					cout << "\n";
				}
			}
			break;
		}
		 
	}
	
	//vertex_print (v, vertices);
	cout << "\n";

	return 0;

}
