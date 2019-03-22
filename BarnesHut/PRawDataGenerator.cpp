/*
#include "RawDataGenerator.h"
#include "boost/python.hpp"
#include "boost/python/numpy.hpp"

namespace bp = boost::python;
namespace bn = boost::

bn::ndarray pPlotCoords(int numOfIons, void* coords)
{
	namespace p = boost::python;
	namespace np = boost::python::numpy;

	Py_Initialize();
	np::initialize();

	MY_TYPE* co_ = (MY_TYPE*)(coords);
	p::object own;
	np::ndarray ndArray = np::from_data(co_, np::dtype::get_builtin<MY_TYPE>(), p::make_tuple(numOfIons), p::make_tuple(sizeof(MY_TYPE)), own);
	return ndArray;
}
/*
 BOOST_PYTHON_MODULE(example) {
	 bn::initialize();
	 bp::def("PlotCoords", pPlotCoords);
 }
 */
