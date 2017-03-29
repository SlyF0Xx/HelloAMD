#include <vector>
#include <CL\cl.h>
#include <CL\cl2.hpp>
#include <iostream>
//#include <CLUtil.hpp>
#include <fstream>

#include <string>

#pragma comment(lib, "OpenCL.lib")

using namespace std;

void CL_CALLBACK pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
	fprintf(stderr, "OpenCL Error (via pfn_notify): %s\n", errinfo);
}

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
		system("pause");
	}
	case CL_INVALID_ARG_INDEX:
	{
		cout << "Index Arg Invalid";
		system("pause");
	}
	case CL_INVALID_ARG_VALUE:
	{
		cout << "Value Arg Invalid";
		system("pause");
	}
	case CL_INVALID_MEM_OBJECT:
	{
		cout << "Memory Invalid";
		system("pause");
	}
	case CL_INVALID_SAMPLER:
	{
		cout << "Sempler Invalid";
		system("pause");
	}
	case CL_INVALID_DEVICE_QUEUE:
	{
		cout << "Device Queue Invalid";
		system("pause");
	}
	case CL_INVALID_ARG_SIZE:
	{
		cout << "Arg Invalid";
		system("pause");
	}
	case CL_OUT_OF_HOST_MEMORY:
	{
		cout << "Out of host memory";
		system("pause");
	}
	case CL_INVALID_VALUE:
	{
		cout << "Invalid value";
		system("pause");
	}
	case CL_INVALID_CONTEXT:
	{
		cout << "Invalid context";
		system("pause");
	}
	case CL_INVALID_COMMAND_QUEUE:
	{
		cout << "Invalid command Queue";
		system("pause");
	}
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
	{
		cout << "Allocation memory failed";
		system("pause");
	}
	default:
		cout << "Not add some excemptions";
		system("pause");
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

		cl_device_local_mem_type mem;
		check(j.getInfo(CL_DEVICE_LOCAL_MEM_TYPE, &mem));
		{
			cout << "Local mem type of device is " << (mem==CL_LOCAL ? "local" : "global") << endl;
		}
		

		cl_ulong lon;
		check(j.getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &lon));
		{
			cout << "Max local memory of device are " << lon << endl;
		}

		//TODO: описать и другие(можно и все описания)

		cout << endl;

	}
}

typedef float type;
const int Wide = 8192;//16384

void Reading(vector<string>* source, type* Src1Ptr, type* Src2Ptr)
{
	ifstream KernelFile("Sources.cl");
	
	string temp = string();

	if (!KernelFile)
	{
		cout << "Файл не найден";
		system("pause");
	}

	while (getline(KernelFile, temp))
	{
		source->push_back(temp);
	}

	KernelFile.close();

	ifstream Src1File("in-1.txt", ios::binary);
	ifstream Src2File("in-2.txt", ios::binary);


	for (int i(0); i < Wide*Wide; i++)
	{
		Src1File.read((char *)(Src1Ptr + i), sizeof(type));
		Src2File.read((char *)(Src2Ptr + i), sizeof(type));
	}

	Src1File.close();
	Src2File.close();
}

int main()
{
	vector<string> source;

	type Src1Ptr[Wide*Wide];
	type Src2Ptr[Wide*Wide];

	Reading(&source, Src1Ptr, Src2Ptr);

	ofstream fout("out-1.txt", ios::binary);

	cl_int result(0);

	vector<cl::Platform> Platforms;

	//PREINITIAL BEGIN

	check(cl::Platform::get(&Platforms));

	#ifdef _DEBUG
		PlatformInfo(&Platforms);
	#endif // DEBUG

	vector<vector<cl::Device>> Devices;

	Devices.resize(Platforms.size());

	vector<cl::Device> Avaliable;

	for (int i(0); i < Platforms.size(); i++)
	{
		check(Platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &(Devices[i])));
		#ifdef _DEBUG
			DeviceInfo(&(Devices[i]));
		#endif // _DEBUG

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

		#ifdef _DEBUG
			//TODO: Описание контекста 
		#endif


		cl::QueueProperties Prop(cl::QueueProperties::None);

		#ifdef _DEBUG
			Prop = Prop | (cl::QueueProperties)CL_QUEUE_PROFILING_ENABLE;
			//CommandQueuePropetries |= CL_QUEUE_PROFILING_ENABLE;
		#endif // _DEBUG

		cl::CommandQueue CommandQueue = cl::CommandQueue(Context, Avaliable[0], Prop, &result);

		check(result);

		#ifdef _DEBUG
			//TODO описание очередей
		#endif

		cl::Program::Sources sources = cl::Program::Sources(source);
		cl::Program prog = cl::Program(Context, sources, &result);

		check(result);

		check(prog.build()); //TODO опции!!

		cl::string s;
		cout<< (prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(Avaliable[0])).c_str();

		#ifdef _DEBUG
			//TODO описание программы
		#endif // _DEBUG

		//INITIAL END

		cl::Kernel Kernel = cl::Kernel(prog, "simple", &result);
		
		check(result);

		#ifdef _DEBUG
			//TODO описание Кернела, аргументов и проч.
		#endif // _DEBUG

		unsigned int size = Wide * Wide * sizeof(type);

		int *Mem = nullptr;
		cl::Buffer Memory = cl::Buffer(Context, CL_MEM_WRITE_ONLY, size, Mem, &result);

		check(result);

		//check(Kernel.setArg(0, &Memory));

		cl::Buffer Src1 = cl::Buffer(Context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, size, Src1Ptr, &result);
		cl::Buffer Src2 = cl::Buffer(Context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, size, Src2Ptr, &result);

		check(Kernel.setArg(0, sizeof(cl_mem), &Src1));
		check(Kernel.setArg(1, sizeof(cl_mem), &Src2));
		check(Kernel.setArg(2, sizeof(cl_mem), &Memory));
		check(Kernel.setArg(3, sizeof(int), &Wide));
		check(Kernel.setArg(4, 32768, NULL));

		cl::NDRange offset(0);
		cl::NDRange global(Wide, Wide, 1);
		cl::NDRange local(16, 16, 1);

		check(CommandQueue.enqueueNDRangeKernel(Kernel, cl::NullRange, global, local, NULL, NULL));
																				//event

		check(CommandQueue.enqueueMarkerWithWaitList());
											//event

		cout << "Done!" << endl;

		
		const unsigned int BufferSize = 8192;//16384
		type Buffer[BufferSize];

		for (int i(0); i < Wide*Wide/BufferSize; i++)
		{
			//check(clEnqueueReadBuffer(CommandQueue(), Memory(), true, i*sizeof(Buffer), sizeof(Buffer), Buffer, NULL, NULL, NULL));
			
			CommandQueue.enqueueReadBuffer(Memory, true, i * sizeof(Buffer), sizeof(Buffer), &Buffer, NULL, NULL);

			fout.write((char *)&Buffer, sizeof(Buffer)/ sizeof(char));
			//fout << '\n';
			fout.flush();
		}

		check(CommandQueue.enqueueMarkerWithWaitList());
		//event

		delete Mem;
		fout.close();
	}


	return 0;
}