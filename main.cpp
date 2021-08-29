/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2021-2021 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
//   See details of license at "LICENSE"   //
//                                         //
/////////////////////////////////////////////

#include "ncbind/ncbind.hpp"
#include "LayerBitmapIntf.h"
#include "LayerBitmapUtility.h"
#include "LayerBitmapUDFlip.h"
#include <string.h>
#include <stdio.h>

static void PreRegistCallback()
{
	iTJSDispatch2 *global = TVPGetScriptDispatch();
	if (global)
	{
		tTJSVariant layer_val;
		static ttstr Layer_name(TJS_W("Layer"));
		global->PropGet(0, Layer_name.c_str(), Layer_name.GetHint(), &layer_val, global);
		tTJSVariantClosure layer_valclosure = layer_val.AsObjectClosureNoAddRef();
		if (layer_valclosure.Object)
		{
			layer_valclosure.DeleteMember(TJS_IGNOREPROP, TJS_W("flipUD"), 0, NULL);
		}
	}
}

NCB_PRE_REGIST_CALLBACK(PreRegistCallback);

class LayerLayerExUDFlip
{
public:
	static tjs_error TJS_INTF_METHOD flipUD(
		tTJSVariant	*result,
		tjs_int numparams,
		tTJSVariant **param,
		iTJSDispatch2 *objthis)
	{
		if (numparams < 0)
		{
			return TJS_E_BADPARAMCOUNT;
		}

		if(objthis == NULL) TVPThrowExceptionMessage(TJS_W("Specify Layer or Bitmap class object"));
		tTJSVariant bmpobject = tTJSVariant(objthis, objthis);
		tTJSVariantClosure bmpobject_clo = bmpobject.AsObjectClosureNoAddRef();

		// Now get information (this will independ the bitmap)
		tjs_int bmpwidth = 0;
		tjs_int bmpheight = 0;
		tjs_int bmppitch = 0;
		tjs_uint8* bmpdata = NULL;
		GetBitmapInformationFromObject(bmpobject_clo, true, &bmpwidth, &bmpheight, &bmppitch, &bmpdata);
		tjs_int provincebmpwidth = 0;
		tjs_int provincebmpheight = 0;
		tjs_int provincebmppitch = 0;
		tjs_uint8* provincebmpdata = NULL;
		GetProvinceBitmapInformationFromObject(bmpobject_clo, true, &provincebmpwidth, &provincebmpheight, &provincebmppitch, &provincebmpdata);
		if(!bmpdata) TVPThrowExceptionMessage(TJS_W("Specify Layer or Bitmap class object"));
		tTVPBaseBitmap bmpinfo(bmpwidth, bmpheight, bmppitch, bmpdata);

		tTVPRect r(0, 0, bmpwidth, bmpheight);
		UDFlip(&bmpinfo, r);
		if (provincebmpdata)
		{
			tTVPBaseBitmap provincebmpinfo(provincebmpwidth, provincebmpheight, provincebmppitch, provincebmpdata, false);
			UDFlip(&provincebmpinfo, r);
		}

		UpdateWholeLayerWithLayerObject(bmpobject_clo);
		return TJS_S_OK;
	}
};

NCB_ATTACH_CLASS(LayerLayerExUDFlip, Layer)
{
	RawCallback("flipUD", &Class::flipUD, 0);
};
