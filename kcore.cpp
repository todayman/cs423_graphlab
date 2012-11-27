#include <graphlab.hpp>
using namespace graphlab;
#include <mpi.h>
#include <sstream>
#include <string>
using namespace std;

distributed_control dc;

typedef distributed_graph<empty, empty> graph_type;

class graph_writer
{
public:
	std::string save_vertex(graph_type::vertex_type v)
	{
		stringstream strm;
		strm << "Process id: " << dc.procid() << " : " << mpi_tools::rank() << " / " << mpi_tools::size() << "\n";
		return strm.str();
	}
	std::string save_edge(graph_type::edge_type e)
	{
		return "";
	}
};

int main(int argc, char* argv[])
{
	mpi_tools::init(argc, argv);

	graph_type graph(dc);
	graph.load_format("graph.txt", "adj");

#ifdef HAS_MPI
	dc.cout() << "Has mpi!\n";
#endif
	dc.cout() << "Hello World from " << dc.procid() << " / " << mpi_tools::size() << "!\n";

	graph.save("output.txt", graph_writer(), false, true, false);
	sleep(4000);

	mpi_tools::finalize();

	return 0;
}
