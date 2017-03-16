#include <vector>
#include <CL\cl.h>
#include <CLUtil.hpp>

#pragma comment(lib, "OpenCL.lib")

using namespace std;

int main()
{
	cl_platform_id* platform = nullptr;
	cl_uint uints = 0;
	clGetPlatformIDs(NULL , nullptr, &uints);

	return 0;
}