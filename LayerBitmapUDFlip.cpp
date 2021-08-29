
#include "tjsCommHead.h"
#include "LayerBitmapIntf.h"
#include "LayerBitmapUDFlip.h"

void UDFlip(tTVPBaseBitmap *destbmp, const tTVPRect &rect)
{
	// up-down flip for given rectangle

	if(rect.left < 0 || rect.top < 0 || rect.right > (tjs_int)destbmp->GetWidth() ||
		rect.bottom > (tjs_int)destbmp->GetHeight())
				TVPThrowExceptionMessage(TJS_W("Source rectangle out of bitmap"));

	tjs_int h = (rect.bottom - rect.top) /2;
	tjs_int w = rect.right - rect.left;
	tjs_int pitch = destbmp->GetPitchBytes();
	tjs_uint8 * l1 = (tjs_uint8*)destbmp->GetScanLineForWrite(rect.top);
	tjs_uint8 * l2 = (tjs_uint8*)destbmp->GetScanLineForWrite(rect.bottom - 1);


	if(destbmp->Is32BPP())
	{
		// 32bpp
		l1 += rect.left * sizeof(tjs_uint32);
		l2 += rect.left * sizeof(tjs_uint32);
		while(h--)
		{
			TVPSwapLine32((tjs_uint32*)l1, (tjs_uint32*)l2, w);
			l1 += pitch;
			l2 -= pitch;
		}
	}
	else
	{
		// 8bpp
		l1 += rect.left;
		l2 += rect.left;
		while(h--)
		{
			TVPSwapLine8(l1, l2, w);
			l1 += pitch;
			l2 -= pitch;
		}
	}
}
