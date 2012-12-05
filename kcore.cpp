#include <graphlab.hpp>
using namespace graphlab;
#include <mpi.h>
#include <sstream>
#include <string>
using namespace std;
#include <unistd.h>

#define KCORE_DEGREE	2

struct vertex {
	bool in_core;
	int changed;

	vertex() : in_core(true), changed(0) { }
	
	void save(oarchive& oarc) const { }
	void load(iarchive& iarc) { }
};

typedef distributed_graph<vertex, empty> graph_type;

class kcore_program : public ivertex_program<graph_type, int>, public IS_POD_TYPE {
public:
	edge_dir_type gather_edges(icontext_type& context, const vertex_type& vertex) const {
		return IN_EDGES;
	}

	int gather(icontext_type& context, const vertex_type& vertex, edge_type& edge) const {
		return ( edge.source().data().in_core ? 1 : 0 );
	}

	void apply(icontext_type& context, vertex_type& vertex, const gather_type& total) {
		if( total <  KCORE_DEGREE ) {
			in_core = false;
			changed += 1;
		}
	}

	edge_dir_type scatter_edges(icontext_type& context, const vertex_type& vertex) const {
		if( changed == 1 ) {
		}
		else {
			return NO_EDGES;
		}
	}
};

class graph_writer
{
public:
	std::string save_vertex(graph_type::vertex_type v)
	{
		stringstream strm;
		//strm << "Process id: " << dc.procid() << " : " << mpi_tools::rank() << " / " << mpi_tools::size() << "\n";
		char name[81];
		gethostname(name, 80);
		name[80] = 0;
		return string(name) + "\n";
	}
	std::string save_edge(graph_type::edge_type e)
	{
		return "";
	}
};

int main(int argc, char* argv[])
{
	mpi_tools::init(argc, argv);

	distributed_control dc;
	graph_type graph(dc);
	graph.load_format("hdfs://ip-10-149-6-179/paul/graph.txt", "adj");

	dc.cout() << "Hello World from " << dc.procid() << "( " << mpi_tools::rank() << ") / " << mpi_tools::size() << "!\n";

	omni_engine<kcore_program> engine(dc, graph, "sync");
	engine.signal_all();
	engine.start();

	graph.save("hdfs://ip-10-149-6-179/paul/output.txt", graph_writer(), false, true, false);

	mpi_tools::finalize();

	return 0;
}
