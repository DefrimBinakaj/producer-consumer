#include "commands.h"




// end time; time calculation function
std::string end_time(std::chrono::system_clock::time_point start_time) {

	// https://stackoverflow.com/questions/40705817/c-chrono-get-seconds-with-a-precision-of-3-decimal-places
	std::chrono::duration<double> elapsed_time = std::chrono::system_clock::now() - start_time;
	double execution_time = elapsed_time.count();
	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << execution_time;
	std::string execution_time_as_string = stream.str();

    return execution_time_as_string;

    // call with:
    // end_time(start_time).c_str()

}
