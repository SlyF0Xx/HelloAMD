#include <vector>
#include <CL\cl.h>
#include <iostream>
#include <CLUtil.hpp>

#pragma comment(lib, "OpenCL.lib")

using namespace std;

int main()
{
	cl_uint num = 0;

	if (clGetPlatformIDs(NULL, nullptr, &num) == CL_SUCCESS)
	{
		cout << "Count of platforms successfuly find. There are "<< num <<" platforms"<<endl;

		cl_platform_id* platform = new cl_platform_id[num];

		char buffer[10240];
		for (cl_uint i(1); i < num + 1; i++)
		{
			if (clGetPlatformIDs(i, platform, NULL) == CL_SUCCESS)
			{
				cout << "handler of " << i << " platform succesfuly created" << endl;

				if (clGetPlatformInfo(platform[i - 1], CL_PLATFORM_PROFILE, 10240, buffer, NULL) == CL_SUCCESS)
				{
					cout << "Profile of " << i << " platform is " << buffer << endl;
				};

				if (clGetPlatformInfo(platform[i - 1], CL_PLATFORM_VERSION, 10240, buffer, NULL) == CL_SUCCESS)
				{
					cout << "Version of " << i << " platform is " << buffer << endl;
				};

				if (clGetPlatformInfo(platform[i - 1], CL_PLATFORM_NAME, 10240, buffer, NULL) == CL_SUCCESS)
				{
					cout << "Name of " << i << " platform is " << buffer << endl;
				};

				if (clGetPlatformInfo(platform[i - 1], CL_PLATFORM_VENDOR, 10240, buffer, NULL) == CL_SUCCESS)
				{
					cout << "Vendor of " << i << " platform is " << buffer << endl;
				};

				if (clGetPlatformInfo(platform[i - 1], CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL) == CL_SUCCESS)
				{
					cout << "Extensions of " << i << " platform is " << buffer << endl;
				};

				cout << endl;

				cl_uint NumDevices = 0;
				if (clGetDeviceIDs(platform[i - 1], CL_DEVICE_TYPE_ALL, NULL, NULL, &NumDevices) == CL_SUCCESS)
				{
					cout << "Count of devices on" << i << " platform succesfully find. There are " << NumDevices << " Devices" << endl;

					cl_device_id* devices = new cl_device_id[NumDevices];
					for (cl_uint j(1); j < NumDevices+1; j++)
					{

						if (clGetDeviceIDs(platform[i - 1], CL_DEVICE_TYPE_ALL, j, devices, NULL) == CL_SUCCESS)
						{
							cout << "Handler of " << j << " device succesfuly created"<< endl;

							cl_device_type* type = new cl_device_type;
							if (clGetDeviceInfo(devices[j - 1], CL_DEVICE_TYPE, sizeof(type), type, NULL) == CL_SUCCESS)
							{
								if ((*type & CL_DEVICE_TYPE_CPU) != 0)
								{
									cout << "It's CPU!" << endl;
								}
								if ((*type & CL_DEVICE_TYPE_GPU) != 0)
								{
									cout << "It's GPU!" << endl;
								}
								if ((*type & CL_DEVICE_TYPE_ACCELERATOR) != 0)
								{
									cout << "It's Accelerator!" << endl;
								}
								if ((*type & CL_DEVICE_TYPE_CUSTOM) != 0)
								{
									cout << "It's Custom!" << endl;
								}
								if ((*type & CL_DEVICE_TYPE_DEFAULT) != 0)
								{
									cout << "It's default!" << endl;
								}
							};

							cl_uint buffer;
							if (clGetDeviceInfo(devices[j - 1], CL_DEVICE_VENDOR_ID, sizeof(buffer), &buffer, NULL) == CL_SUCCESS)
							{
								cout << "VendorID of " << i << " device is " << buffer << endl;
							}

							if (clGetDeviceInfo(devices[j - 1], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buffer), &buffer, NULL) == CL_SUCCESS)
							{
								cout << "Max compute units of " << i << " device are " << buffer << endl;
							}

							//TODO: описать и другие(можно и все описания)

							cout << endl;
						}
					}
				}
			};
		}

	};


	

	
	return 0;
}