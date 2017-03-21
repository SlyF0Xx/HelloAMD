#include <vector>
#include <CL\cl.h>
#include <CL\cl2.hpp>
#include <iostream>
//#include <CLUtil.hpp>

#include <string>

#pragma comment(lib, "OpenCL.lib")

using namespace std;

void CL_CALLBACK pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
	fprintf(stderr, "OpenCL Error (via pfn_notify): %s\n", errinfo);
}

const char *program_source[] = {
	"__kernel void simple(__global write_only int *Dst, const  int Wide)\n",
	"{\n",
	"	int x = get_global_id(0);\n",
	"   int y = get_global_id(1);\n",
	"	Dst[y*Wide+x] = y*Wide+x;\n",
	"}\n"
};

const vector<string> source = {
	"__kernel void simple(__global write_only int *Dst, const  int Wide)\n",
	"{\n",
	"	int x = get_global_id(0);\n",
	"   int y = get_global_id(1);\n",
	"	Dst[y*Wide+x] = y*Wide+x;\n",
	"}\n"
};

void check(cl_int result)
{
	switch (result)
	{
	case CL_SUCCESS:
	{
		return;
	}

	default:
		cout << "Not add some excemptions";
		exit(0);
	}
}

struct Dev
{
	cl_device_id* devices;
	cl_uint NumDevices;
};

Dev CreateDevices(unsigned int size, cl_platform_id* platforms)
{
	cl_uint NumDevices = 0;
	for (cl_uint i(0); i < size; i++)
	{
		check(clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, NULL, NULL, &NumDevices));
		cout << "Count of devices on" << i + 1 << " platform succesfully find. There are " << NumDevices << " Devices" << endl;

		cl_device_id* devices = new cl_device_id[NumDevices];

		check(clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, NULL, NULL, &NumDevices));
		cout << "Count of devices on" << i + 1 << " platform succesfully find. There are " << NumDevices << " Devices" << endl;

		check(clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, NumDevices, devices, NULL));
		for (cl_uint j(0); j < NumDevices; j++)
		{
			cout << "Handler of " << j + 1 << " device succesfuly created" << endl;

			cl_device_type* type = new cl_device_type;
			check(clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, sizeof(type), type, NULL));
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
			cl_uint buffer;
			
			check(clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR_ID, sizeof(buffer), &buffer, NULL));
			{
				cout << "VendorID of " << j + 1 << " device is " << buffer << endl; 
			}

			check(clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buffer), &buffer, NULL));
			{
				cout << "Max compute units of " << j + 1 << " device are " << buffer << endl; 
			}

			check(clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(buffer), &buffer, NULL));
			{
				cout << "Max work item dimention of " << j + 1 << " device are " << buffer << endl;

				size_t* t = new size_t[buffer];

				check(clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*buffer, t, NULL));
				for (cl_uint f(0); f < buffer; f++)
				{
					cout << "Max work item of " << j + 1 << " device in " << f + 1 << " dimention are " << t[f] << endl;
				}
			}
					
			char name[256];

			check(clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(char[256]), &name, NULL));
			{
				cout << "Name of " << j + 1 << " device are " << name << endl;
			}

			size_t count;
			check(clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(count), &count, NULL));
			{
				cout << "Max count of work group of " << j + 1 << " device are " << count << endl;
			}

			//TODO: ������� � ������(����� � ��� ��������)

			cout << endl;
		}

		return Dev{ devices, NumDevices };
	}
}

void PlatformInfo(vector<cl::Platform> *Platforms)
{
	cout << "Platforms created perfectly. There are "<<Platforms->size()<<" platforms"<<endl;
	std::string str = std::string();
	for (auto i : (*Platforms))
	{
		check(i.getInfo(CL_PLATFORM_NAME, &str));
		cout << "Information about "<< str <<" platform" << endl;

		check(i.getInfo(CL_PLATFORM_EXTENSIONS, &str));
		cout << "These platform have these extensions: "<<str<<endl;
		
		check(i.getInfo(CL_PLATFORM_PROFILE, &str));
		cout << "These platform have these profile: " << str << endl;

		check(i.getInfo(CL_PLATFORM_VENDOR, &str));
		cout << "These platform have these vendor: " << str << endl;

		check(i.getInfo(CL_PLATFORM_VERSION, &str));
		cout << "These platform have these version: " << str << endl;
		cout << endl;
	}
}

void DeviceInfo(vector<vector<cl::Device>> *Devices)
{
	cout << "Devices created perfectly" << endl;
	/*for (int i(0); i < Devices->size(); i++)
	{
		cout << "There are " << (*Devices)[i].size() << " for this platorm" << endl;
		DeviceInfo(&((*Devices)[i]));
	}*/
	/*
	for (auto i : (*Devices))
	{
		cout << "There are "<< i.size()<<" for this platorm"<<endl;
		
		DeviceInfo(&i);
	}*/
}

void DeviceInfo(vector<cl::Device> *Devices)
{
	cout << "Devices created perfectly" << endl;
	
	for (auto j : *Devices)
	{
		char name[256];

		check(j.getInfo(CL_DEVICE_NAME, &name));
		{
			cout << "Name of device are " << name << endl;
		}

		cl_device_type* type = new cl_device_type;
		check(j.getInfo(CL_DEVICE_TYPE, type));
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
		cl_uint buffer;

		check(j.getInfo(CL_DEVICE_VENDOR_ID, &buffer));
		{
			cout << "VendorID of device is " << buffer << endl;
		}

		check(j.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &buffer));
		{
			cout << "Max compute units of device are " << buffer << endl;
		}

		check(j.getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &buffer));
		{
			cout << "Max work item dimention of device are " << buffer << endl;
			vector<size_t> t;

			check(j.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &t));
			for (cl_uint f(0); f < buffer; f++)
			{
				cout << "Max work item of device in " << f + 1 << " dimention are " << t[f] << endl;
			}

		}


		size_t count;
		check(j.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &count));
		{
			cout << "Max count of work group of device are " << count << endl;
		}

		//TODO: ������� � ������(����� � ��� ��������)

		cout << endl;

	}
}


int main()
{
	cl_int result(0);

	vector<cl::Platform> Platforms;

	//PREINITIAL BEGIN

	check(cl::Platform::get(&Platforms));

	if (_DEBUG)
	{
		PlatformInfo(&Platforms);
	}

	vector<vector<cl::Device>> Devices;

	Devices.resize(Platforms.size());

	vector<cl::Device> Avaliable;

	for (int i(0); i < Platforms.size(); i++)
	{
		check(Platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &(Devices[i])));
		if (_DEBUG)
		{
			DeviceInfo(&(Devices[i]));
		}
		Avaliable.insert(Avaliable.end(), Devices[i].begin(), Devices[i].end());
	}

	//PREINITIAL END
	
	{
		//INITIAL BEGIN

		vector<cl_context_properties> ContextPropetries;
		ContextPropetries.clear();
		if (Platforms.size() == 1)
		{
			ContextPropetries.push_back(CL_CONTEXT_PLATFORM);
			ContextPropetries.push_back((cl_context_properties)Platforms[0]());
		}

		ContextPropetries.push_back(0);

		cl::Context Context = cl::Context(Avaliable, ContextPropetries.data(), pfn_notify, &result);
		check(result);

		if (_DEBUG)
		{
			//TODO: �������� ��������� 
		}


		cl::QueueProperties Prop(cl::QueueProperties::None);

		if (_DEBUG)
		{
			Prop = Prop | (cl::QueueProperties)CL_QUEUE_PROFILING_ENABLE;
			//CommandQueuePropetries |= CL_QUEUE_PROFILING_ENABLE;
		}

		cl::CommandQueue CommandQueues = cl::CommandQueue(Context, Avaliable[0], Prop, &result);

		check(result);

		if (_DEBUG)
		{
			//TODO �������� ��������
		}


		cl::Program::Sources sources = cl::Program::Sources(source);
		cl::Program progs = cl::Program(Context, sources, &result);
		progs.build(); //TODO �����!!

		if (_DEBUG)
		{
			//TODO �������� ���������
		}


		//INITIAL END

		cl::Kernel Kernels = cl::Kernel(progs, "simple", &result);

		check(result);

		if (_DEBUG)
		{
			//TODO �������� �������, ���������� � ����.
		}

		const int Wide = 16384;

		unsigned int size = Wide * Wide * sizeof(int);
		cl::Memory Memories = cl::Buffer(Context, CL_MEM_WRITE_ONLY, size, &result);


	}











	cl_uint num = 0;

	if (clGetPlatformIDs(NULL, nullptr, &num) == CL_SUCCESS)
	{
		cout << "Count of platforms successfuly find. There are "<< num <<" platforms"<<endl;

		cl_platform_id* platform = new cl_platform_id[num];

		vector<cl_device_id> AvaliableDevices;

		char buffer[10240];
		for (cl_uint i(1); i < num + 1; i++)
		{
			check(clGetPlatformIDs(i, platform, NULL));
			cout << "handler of " << i << " platform succesfuly created" << endl;

			check(clGetPlatformInfo(platform[i - 1], CL_PLATFORM_PROFILE, 10240, buffer, NULL));
			{
				cout << "Profile of " << i << " platform is " << buffer << endl;
			};

			check(clGetPlatformInfo(platform[i - 1], CL_PLATFORM_VERSION, 10240, buffer, NULL));
			{
				cout << "Version of " << i << " platform is " << buffer << endl;
			};

			check(clGetPlatformInfo(platform[i - 1], CL_PLATFORM_NAME, 10240, buffer, NULL));
			{
				cout << "Name of " << i << " platform is " << buffer << endl;
			};

			check(clGetPlatformInfo(platform[i - 1], CL_PLATFORM_VENDOR, 10240, buffer, NULL)	);
			{
				cout << "Vendor of " << i << " platform is " << buffer << endl;
			};

			check(clGetPlatformInfo(platform[i - 1], CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL));
			{
				cout << "Extensions of " << i << " platform is " << buffer << endl;
			};

			cout << endl;

			Dev devices = CreateDevices(num, platform);
		}

		vector<cl_context_properties> ContextPropetries;

		if (num == 1)
		{
			ContextPropetries.push_back(CL_CONTEXT_PLATFORM);
			ContextPropetries.push_back((cl_context_properties)platform[0]);
		}

		ContextPropetries.push_back(0);
		//cl_context Context = clCreateContextFromType(ContextPropetries.data(), CL_DEVICE_TYPE_GPU, &pfn_notify, NULL, &result);
		cl_context Context = clCreateContext(ContextPropetries.data(), AvaliableDevices.size(), AvaliableDevices.data(), &pfn_notify, NULL, &result);

		if (result == CL_SUCCESS &&clRetainContext(Context) == CL_SUCCESS)
		{
			//TODO: �������� ��������� 

			vector<cl_queue_properties> QueuePropetries;
			cl_command_queue_properties CommandQueuePropetries = 0;

			if (_DEBUG)
			{
				//CommandQueuePropetries |= CL_QUEUE_PROFILING_ENABLE;
			}

			if (CommandQueuePropetries)
			{
				QueuePropetries.push_back(CommandQueuePropetries);
			}

			QueuePropetries.push_back(0);
			cl_command_queue CommandQueue = clCreateCommandQueueWithProperties(Context, AvaliableDevices.at(0), QueuePropetries.data(), &result);

			if (result == CL_SUCCESS && clRetainCommandQueue(CommandQueue) == CL_SUCCESS)
			{
				//TODO: �������� ������� �������

				cl_program Programm = clCreateProgramWithSource(Context, sizeof(program_source) / sizeof(*program_source), program_source, NULL, &result); //�������� ��������� � ��������

				if (result == CL_SUCCESS && clRetainProgram(Programm) == CL_SUCCESS)
				{
					if (clBuildProgram(Programm, NULL, NULL, "", NULL, NULL) == CL_SUCCESS) //�������� � ���������� ��������� 
					{									  //��������!!
						cl_kernel Kernel = clCreateKernel(Programm, "simple", &result); //������� ������(�������)

						//TODO: �������� �������
						size_t counts;
						if (clGetKernelWorkGroupInfo(Kernel, AvaliableDevices.at(0), CL_KERNEL_WORK_GROUP_SIZE, sizeof(counts), &counts, NULL) == CL_SUCCESS)
						{
							cout << counts << endl;
						};

						if (result == CL_SUCCESS && clRetainKernel(Kernel) == CL_SUCCESS)
						{
							int *MemPtr = nullptr;

							const int Wide = 16384;
							
							unsigned int size = Wide * Wide * sizeof(int);
							cl_mem Memory = clCreateBuffer(Context, CL_MEM_WRITE_ONLY, size, MemPtr, &result);
															    //��������!!!
							if (result == CL_SUCCESS)
							{
								if(clSetKernelArg(Kernel, 1, sizeof(int), &Wide)==CL_SUCCESS)
								{
									cout << "test1";
								};

								if (clSetKernelArg(Kernel, 0, sizeof(Memory), &Memory) == CL_SUCCESS)
								{
									cout << "���� 2";
								};

								const size_t global[3] = { Wide, Wide, 1 };
								const size_t local[3] = { 16 , 16, 1};

								if (clEnqueueNDRangeKernel(CommandQueue, Kernel, 2, NULL, global, local, NULL, NULL, NULL) == CL_SUCCESS)
								{
									if (clEnqueueMarkerWithWaitList(CommandQueue, NULL,NULL,NULL) == CL_SUCCESS)
									{
										cout << "Done!" << endl;

										int data = 0;
										

										//CL_INVALID_VALUE

										//if(clEnqueueReadBuffer(CommandQueue, Memory, true, 0, size, data, NULL, NULL, NULL)== CL_SUCCESS)

										//int * data = (int *)clEnqueueMapBuffer(CommandQueue, Memory, true, NULL, 0, sizeof(Memory), NULL, NULL, NULL, &result);
										//																//��������!
										//if (result == CL_SUCCESS)
										{
											for (int i(0); i < Wide; i++)
											{
												for (int j(0); j < Wide; j++)
												{
													result = clEnqueueReadBuffer(CommandQueue, Memory, true, i*Wide+j* sizeof(int), sizeof(int), &data, NULL, NULL, NULL);
													cout << data;
													//cout << data[i*Wide + j]<< " ";
												}
												cout << endl;
											}
										}
									}
								}
							}
							clReleaseKernel(Kernel);
						}
					};
					clReleaseProgram(Programm);
				}
				clReleaseCommandQueue(CommandQueue);
			}
			clReleaseContext(Context);

		}
		delete[] platform;
	};


	

	
	return 0;
}