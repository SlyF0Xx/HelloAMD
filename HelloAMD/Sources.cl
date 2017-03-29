__kernel void simple(__global __read_only float *Src1, __global __read_only float *Src2, __global __write_only float *Dst, const int Wide, __local float *Temp)
	{
		__private int GroupX = get_group_id(0);
		__private int GroupY = get_group_id(1);

		for (__private int i = 0; i < 16; i++)
		{
			async_work_group_copy(Temp + (GroupY*16)*Wide + i*Wide, Src1+i*256, 256, 0);
		}
		
		const int Size = 256 * 16;

		for (__private int i = 0; i < 256; i++)
		{
			async_work_group_copy(Temp + (GroupX*16) + i*Wide, Src2 + Size + i * 16, 16, 0);
		}

		__private int x = get_global_id(0);
		__private int y = get_global_id(1);

		__private int LocalX = get_local_id(0);
		__private int LocalY = get_local_id(1);

		__private Sum = 0;

		barrier(CLK_LOCAL_MEM_FENCE);

		for(__private int i = 0; i<256; i++)
		{
			Sum += Temp[LocalY * 256 + i] * Temp[Size + LocalX + i * 16];
		}
		for (__private int i = 256; i < Wide; i++)
		{
			Sum += Src1[y*Wide + i] + Src2[x + i*Wide];
		}
		Dst[y*Wide + x] = Sum;
	}