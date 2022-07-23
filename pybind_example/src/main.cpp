#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
int add(int i, int j) {
    return i + j;
}

class SomeClass {
    float multiplier;

    public:
    SomeClass(float multiplier_) : multiplier(multiplier_){};
    float multiply(float input) {

        return multiplier* input;
    }
    std::vector<float> multiply_list(std::vector<float> items )
    {
        for(auto i =0; i < items.size(); i++)
        {
            items[i]  = multiply(items.at(i));
        }
        return items;
    };

    py::tuple multiply_two(float one , float two)
    {
        return py::make_tuple(multiply(one) , multiply(two));
    };

    std::vector<std::vector<uint8_t>> make_image()
    {
        auto out = std::vector<std::vector<uint8_t>>();
        for(auto i =0; i <128 ; i++)
        {
            out.push_back(std::vector<uint8_t>(64));
        }
        for(auto i =0; i <30; i++)
        {
          for(auto j =0; j <30; j++)
          {
            out[i][j]=255;
          }
   
        }
        return out ;

    }
    
};

namespace py = pybind11;

PYBIND11_MODULE(PriceAlgo, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: cmake_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

py::class_<SomeClass>
(
    m , "PySomeClass"

).def(py::init<float>())
.def("multiply" , &SomeClass::multiply)
.def("multiply_list" , &SomeClass::multiply_list)
// .def("make_image" , &SomeClass::make_image)
.def("make_image" , [](SomeClass &self){
                    py::array out = py::cast(self.make_image());
                    return out;
})
//.def("multiply_two" , &SomeClass::multiply_two)
.def("multiply_two" , [](SomeClass &self , float one , float two){
                    return py::make_tuple(self.multiply(one) , self.multiply(two));
})
;

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
