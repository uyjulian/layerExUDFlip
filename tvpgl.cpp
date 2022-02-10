// Copied tvpgl functions, because we can't rely on the core to have them.

#include "tvpgl.h"
#include <string.h>

TVP_GL_FUNC_DECL(void, TVPSwapLine32_c, (tjs_uint32 *line1, tjs_uint32 *line2, tjs_int len))
{
	tjs_uint32 tmp;
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			tmp = line1[___index];;
			line1[___index] = line2[___index];;
			line2[___index] = tmp;;
		}
	}
}

TVP_GL_FUNC_DECL(void, TVPSwapLine8_c, (tjs_uint8 *line1, tjs_uint8 *line2, tjs_int len))
{
#define swap_tmp_buf_size 256u
	tjs_uint8 swap_tmp_buf[swap_tmp_buf_size];
	while(len > 0)
	{
		tjs_int le = len < swap_tmp_buf_size ? len : swap_tmp_buf_size;
		memcpy(swap_tmp_buf, line1, le);
		memcpy(line1, line2, le);
		memcpy(line2, swap_tmp_buf, le);
		line1 += le;
		line2 += le;
		len -= le;
	}
#undef swap_tmp_buf_size
}
