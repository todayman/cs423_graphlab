#include <graphlab.hpp>
using namespace graphlab;

int main(int argc, char* argv[])
{
	mpi_tools::init(argc, argv);
	distributed_control dc;

	dc.cout() << "Hello World!\n";

	mpi_tools::finalize();

	return 0;
}
