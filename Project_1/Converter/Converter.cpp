
///////////////////////////////////////////////////////////////////////////////////
// converter.cpp - Change <> characters, Insert into html file                //
// ver 1.0																		 //
// Author: Chinmay Ujjwal Vanjare
//CSE 687-Object Oriented Design-Spring 2019
///////////////////////////////////////////////////////////////////////////////////

#include <string>
#include "Converter.h"

#ifdef  TEST_CONVERTER

int main() {
	std::string source = "Converter.cpp";
	std::string destination = "../convertedPages/";

	Convert con;

	con.ConvertHtml(source,destination);


}
#endif //  TEST_CONVERTER
