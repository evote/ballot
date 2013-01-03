#include "ballot.hpp"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE ( pyballot )
{
	using namespace boost::python;
	class_<Ballot,boost::noncopyable> ( "Ballot" )
		.def ( "process", &Ballot::process );
}

