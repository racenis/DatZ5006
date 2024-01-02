#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>

// Šķaudne
struct Edge {
	int a;	// indekss vienai virsotnei
	int b;	// indekss otrai virsotnei
	
	int w;	// šķaudnes svars
};

// Virsotne
struct Vertex {
	std::vector<int> edges;
};

// Absolūtais ļaunums std::vector<bool>
typedef std::vector<bool> Solution;
typedef std::vector<bool> EdgeSet;
typedef std::vector<bool> VertexSet;

// Kopā grafs
struct Graph {
	std::vector<Vertex> vertices; 
	std::vector<Edge> edges; 
};

// Funkcija kas ielasa grafa datus no diska.
Graph MakeGraphFromFile(const char* filename) {
	std::ifstream file(filename, std::ifstream::in);
	
	if (!file.is_open()) {
		std::cout << "ERROR reading file " << filename << std::endl;
		abort();
	}
	
	int vcount;
	file >> vcount;
	
	Graph g;
	
	g.vertices = std::vector<Vertex>(vcount);
	
	std::cout << "Vertex count: " << vcount << std::endl;
	
	Edge e;
		
	file >> e.a;
	file >> e.b;
	file >> e.w;
	
	while (file) {
		e.a--;
		e.b--;
		
		int e_index = g.edges.size();
		
		g.vertices[e.a].edges.push_back(e_index);
		g.vertices[e.b].edges.push_back(e_index);
		
		g.edges.push_back(e);
		
		file >> e.a;
		file >> e.b;
		file >> e.w;
	}
	
	std::cout << "Edge count: " << g.edges.size() << std::endl;
	
	file.close();

	return g;
}

// Saglabā rezultātu uz diska.
void WriteSolutionToFile(Graph& g, Solution& s, int w, const char* filename) {
	std::ofstream file(filename, std::ofstream::out);
	
	if (!file.is_open()) {
		std::cout << "ERROR writing file " << filename << std::endl;
		abort();
	}
	
	int pots = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i]) pots++;
	}
	
	file << pots << " ";
	file << w << " ";
	
	for (int i = 0; i < s.size(); i++) {
		if (s[i]) {
			file << g.edges[i].a+1 << " ";
			file << g.edges[i].b+1 << " ";
		}
	}
	
	file.close();
	
	std::cout << "Saved to disk." << std::endl;
}

// Rekursīvi atrod visus ciklus, kas sākas no kāda punkta.
void FindAllCycles(
	Graph& graph, 
	EdgeSet visited_edges,
	VertexSet visited_vertices,
	int current_vertex,
	int end_vertex,
	std::set<EdgeSet>& found_cycles
) {
	// atzīmējam kārtējo verteksi kā apskatītu
	visited_vertices[current_vertex] = true;
	
	// ejam cauri visām šķaudnēm, kas sākas no šī vertekša
	for (int edge_index : graph.vertices[current_vertex].edges) {
		
		// pārbaudām vai kārtējā šķaudne nav jau apskatīta
		if (visited_edges[edge_index]) continue;
		
		const auto& edge = graph.edges[edge_index];
		
		// atrodam nākamā vertekša indeksu
		int next = edge.a == current_vertex ? edge.b : edge.a;
		
		// ja nākamais verteksis ir sākuma verteksis, tad sanāk ka cikls ir
		// noslēgts un var apgriezties
		if (next == end_vertex) {
			
			// atzīmē pēdējo šķaudni
			auto f_edges = visited_edges;
			f_edges[edge_index] = true;
			
			// ieliek atrastajos ciklos
			found_cycles.insert(f_edges);
			
			continue;
		}
		
		// ja nākamais verteksis ir jau apstaigāts, izlaižam
		if (visited_vertices[next]) continue;
		
		// pieliek kārtējo šķaudni
		auto next_edges = visited_edges;
		auto next_vertices = visited_vertices;
		next_edges[edge_index] = true;
		next_vertices[next] = true;
		
		// rekursē
		FindAllCycles(graph, next_edges, next_vertices, next, end_vertex, found_cycles);
	}
	
	return;
}

// Atrod visus ciklus, kas ir grafā.
std::vector<EdgeSet> FindAllCycles(Graph& graph) {
	std::set<EdgeSet> found_cycles;
	
	std::cout << "Beginning cycle search..." << std::endl;
	
	// no katra vertekša ejam un meklējam ciklus
	for (int i = 0; i < graph.vertices.size(); i++) {
		FindAllCycles(graph, EdgeSet(graph.edges.size(), false), VertexSet(graph.vertices.size(), false), i, i, found_cycles);
		
		std::cout << "Finished " << i+1 << "/" << graph.vertices.size() << std::endl;
	}
	
	// pārveidojam kopu uz masīvu, lai ātrāk iterējās
	std::vector<EdgeSet> cycle_vector;
	
	for (auto& cycle : found_cycles) {
		cycle_vector.push_back(cycle);
	}
	
	std::cout << "End! Total cycles found: " << cycle_vector.size() << std::endl;
	
	return cycle_vector;
}

// Pārbauda risinājumu.
int ValidateSolution(
	const Graph& graph, 
	const std::vector<std::vector<bool>>& cycles, 
	const Solution& solution
) {
	int fails = 0;
	
	for (const auto& cycle : cycles) {
		bool has_pot = false;
		for (int i = 0; i < solution.size(); i++) {
			if (solution[i] && cycle[i]) {
				has_pot = true;
				break;
			}
		}
		if (!has_pot) {
			fails++;
			break; // !!! neizskaita līdz beigām
		}
	}

	return fails;
}

// Aprēķina risinājuma kopējo sarežģītību.
int ComputeDifficulty(const Graph& g, const Solution& s) {
	int sum = 0;
	for (size_t i = 0; i < g.edges.size(); i++) {
		if (s[i]) sum += g.edges[i].w;
	}
	return sum;
}

// Nedeterministiski pamaina risinājumu.
void ModifySolution(Solution& s) {
	int index = rand() % s.size();
	s[index] = rand() % 2;
}

int main(int argc, const char** argv) {
	
	if (argc != 4) {
		std::cout << "Usage: prog input_file output_file max_iterations" << std::endl;
		std::cout << "\t input_file is input file." << std::endl;
		std::cout << "\t output_file is output file." << std::endl;
		std::cout << "\t max_iterations is iterations until stopping optimization," << std::endl;
		std::cout << "\t\tuse negative values to select other algorithm." << std::endl;
		return 0;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];
	int iterations = atoi(argv[3]);
	bool switch_algorithm = iterations < 0;
	if (iterations < 0) iterations = -iterations;
	
	Graph g = MakeGraphFromFile(input);
	auto c = FindAllCycles(g);
	
	std::set<Solution> valid_solutions;		// visi labie risinājumi
	std::set<Solution> invalid_solutions;	// visi nelabie risinājumi
	std::set<Solution> generated_solutions;	// uzģenerētie risinājumi
	
	// iebāžam pirmo (iespējams) korekto risinājumu
	generated_solutions.insert(Solution(g.edges.size(), true));
	
	// pieņemam pirmo kā labāko
	Solution best = *generated_solutions.begin();
	int best_value = ComputeDifficulty(g, *generated_solutions.begin());
	int skipped = 0;
	
	std::cout << "Starting optimization..." << std::endl;
	
	
	// ļoti nekorektais, bet samērā ātrais algoritms priekš meklēšanas
	if (!switch_algorithm) while (generated_solutions.size()) {
		std::set<Solution> new_solutions;
		
		for (auto& solution : generated_solutions) {
			for (size_t i = 0; i < 5; i++) {
			
				// kaut kāds jauns risinājums
				auto new_solution = solution; ModifySolution(new_solution);
				
				if (valid_solutions.contains(new_solution) ||
					invalid_solutions.contains(new_solution) ||
					new_solutions.contains(new_solution)
				) {
					skipped++;
					continue;
				}
				
				bool is_valid = !ValidateSolution(g, c, new_solution);

				if (is_valid) {
					valid_solutions.insert(new_solution);
					new_solutions.insert(new_solution);
					
					int difficulty = ComputeDifficulty(g, new_solution);
					if (difficulty < best_value) {
						best_value = difficulty;
						best = new_solution;
						std::cout << "New best: " << best_value << std::endl;
					}
				} else {
					invalid_solutions.insert(new_solution);
				}
			}
		}
		
		std::cout << "Generated: " << generated_solutions.size();
		std::cout << "\tValid: " << valid_solutions.size(); 
		std::cout << "\tInvalid: " << invalid_solutions.size();
		std::cout << "\tSkipped: " << skipped << std::endl;
		
		if (valid_solutions.size() + invalid_solutions.size() + skipped > iterations) break;
		
		generated_solutions = new_solutions;
	}
	
	
	// vecais risinājumu ģenerēšanas kods, kas 100% atradīs visus risinājumus,
	// bet viņš ir lēns
	if (switch_algorithm) while (generated_solutions.size()) {
		
		// te liksim jaunos risinājumus
		std::set<Solution> new_solutions;
		
		// ejam cauri visiem apskatītajiem risinājumiem
		for (auto& solution : generated_solutions) {
			
			// ģenerējam jaunus risinājumus ejot cauri visiem podiem risinājumā
			for (size_t i = 0; i < solution.size(); i++) {
				
				// ja pods jau ir izmests, tad vienalga, turpinām
				if (!solution[i]) continue;
				
				// izmetam podu i-tajā pozīcijā
				auto new_solution = solution; new_solution[i] = false;
				
				// ja šitāds jau ir uzģenerēts, tad nemokāmies vēlreiz
				if (valid_solutions.contains(new_solution) ||
					invalid_solutions.contains(new_solution) ||
					new_solutions.contains(new_solution)
				) {
					skipped++;
					continue;
				}
				
				// pārbaudām vai derīgs risinājums
				bool is_valid = !ValidateSolution(g, c, new_solution);
				
				// iekrāmējam attiecīgajā grupā
				if (is_valid) {
					valid_solutions.insert(new_solution);
					new_solutions.insert(new_solution);
					
					int difficulty = ComputeDifficulty(g, new_solution);
					if (difficulty < best_value) {
						best_value = difficulty;
						best = new_solution;
						std::cout << "New best: " << best_value << std::endl;
					}
				} else {
					invalid_solutions.insert(new_solution);
				}
			}
		}
		
		std::cout << "Generated: " << generated_solutions.size();
		std::cout << "\tValid: " << valid_solutions.size(); 
		std::cout << "\tInvalid: " << invalid_solutions.size();
		std::cout << "\tSkipped: " << skipped << std::endl;
		
		if (valid_solutions.size() + invalid_solutions.size() + skipped > iterations) break;
		
		// visi iepriekš uzģenerētie risinājumi ir apskatīti, apskatām jaunos
		generated_solutions = new_solutions;
	}

	std::cout << "Done!" << std::endl;
	
	WriteSolutionToFile(g, best, best_value, output);
}
