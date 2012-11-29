#include <graphlab.hpp>
using namespace graphlab;
#include <mpi.h>
#include <sstream>
#include <string>
using namespace std;
#include <unistd.h>


typedef distributed_graph<empty, empty> graph_type;

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

	graph.save("hdfs://ip-10-149-6-179/paul/output.txt", graph_writer(), false, true, false);

	mpi_tools::finalize();

	return 0;
}
