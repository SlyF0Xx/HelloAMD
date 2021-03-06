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
	case CL_INVALID_KERNEL:
	{
		cout << "Kernel Invalid";
		exit(0);
	}
	case CL_INVALID_ARG_INDEX:
	{
		cout << "Index Arg Invalid";
		exit(0);
	}
	case CL_INVALID_ARG_VALUE:
	{
		cout << "Value Arg Invalid";
		exit(0);
	}
	case CL_INVALID_MEM_OBJECT:
	{
		cout << "Memory Invalid";
		exit(0);
	}
	case CL_INVALID_SAMPLER:
	{
		cout << "Sempler Invalid";
		exit(0);
	}
	case CL_INVALID_DEVICE_QUEUE:
	{
		cout << "Device Queue Invalid";
		exit(0);
	}
	case CL_INVALID_ARG_SIZE:
	{
		cout << "Arg Invalid";
		exit(0);
	}
	default:
		cout << "Not add some excemptions";
		exit(0);
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

		cl::CommandQueue CommandQueue = cl::CommandQueue(Context, Avaliable[0], Prop, &result);

		check(result);

		if (_DEBUG)
		{
			//TODO �������� ��������
		}


		cl::Program::Sources sources = cl::Program::Sources(source);
		cl::Program prog = cl::Program(Context, sources, &result);
		prog.build(); //TODO �����!!

		if (_DEBUG)
		{
			//TODO �������� ���������
		}


		//INITIAL END

		cl::Kernel Kernel = cl::Kernel(prog, "simple", &result);

		check(result);

		if (_DEBUG)
		{
			//TODO �������� �������, ���������� � ����.
		}

		const int Wide = 16384;

		unsigned int size = Wide * Wide * sizeof(int);

		int *Mem = nullptr;
		cl::Buffer Memory = cl::Buffer(Context, CL_MEM_WRITE_ONLY, size, Mem, &result);

		check(result);

		//check(Kernel.setArg(0, &Memory));
		check(Kernel.setArg(0, sizeof(cl_mem), &Memory));
		check(Kernel.setArg(1, sizeof(int), &Wide));

		cl::NDRange offset(0);
		cl::NDRange global(Wide, Wide, 1);
		cl::NDRange local(16, 16, 1);

		check(CommandQueue.enqueueNDRangeKernel(Kernel, cl::NullRange, global, local, NULL, NULL));
																				//event

		check(CommandQueue.enqueueMarkerWithWaitList());
											//event

		cout << "Done!" << endl;

		//int *dat = nullptr;
		//check(CommandQueue.enqueueReadBuffer(Memory, true, 0, size, &dat, NULL, NULL));
		//																				//events

		int data = 0;
		{
			for (int i(0); i < Wide; i++)
			{
				for (int j(0); j < Wide; j++)
				{
					CommandQueue.enqueueReadBuffer(Memory, true, (i*Wide + j) * sizeof(int), sizeof(int), &data, NULL, NULL);
					//result = clEnqueueReadBuffer(CommandQueue, Memory, true, i*Wide + j * sizeof(int), sizeof(int), &data, NULL, NULL, NULL);
					cout << data << " ";
					//cout << dat[i*Wide + j];
					
				}
				cout << endl;
			}
		}
	}


	return 0;
}