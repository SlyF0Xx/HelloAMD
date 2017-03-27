__kernel void simple(__global __read_only float *Src1,__global __read_only float *Src2,__global __write_only float *Dst, const  int Wide)
	{
		int x = get_global_id(0);
	   	int y = get_global_id(1);
		Dst[y*Wide+x] = 0;
		for(__private int i = 0; i<Wide; i++)
		{
			Dst[y*Wide+x] += Src1[y*Wide+i] * Src2[i*Wide+x];
		}
		
	}