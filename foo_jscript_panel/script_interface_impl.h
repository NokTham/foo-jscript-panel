#pragma once

#include "script_interface.h"
#include "com_tools.h"
#include "panel_tooltip_param.h"
#include "helpers.h"

template <class T, class T2>
class GdiObj : public MyIDispatchImpl<T>
{
	BEGIN_COM_QI_IMPL()
		COM_QI_ENTRY_MULTI(IUnknown, IDispatch)
		COM_QI_ENTRY(T)
		COM_QI_ENTRY(IGdiObj)
		COM_QI_ENTRY(IDisposable)
		COM_QI_ENTRY(IDispatch)
	END_COM_QI_IMPL()

protected:
	T2* m_ptr;

	GdiObj<T, T2>(T2* p) : m_ptr(p)
	{
	}

	virtual ~GdiObj<T, T2>()
	{
	}

	virtual void FinalRelease()
	{
		if (m_ptr)
		{
			delete m_ptr;
			m_ptr = NULL;
		}
	}

public:
	// Default Dispose
	STDMETHODIMP Dispose()
	{
		FinalRelease();
		return S_OK;
	}

	STDMETHODIMP get__ptr(void** pp)
	{
		*pp = m_ptr;
		return S_OK;
	}
};

class GdiFont : public GdiObj<IGdiFont, Gdiplus::Font>
{
protected:
	HFONT m_hFont;
	bool m_managed;

	GdiFont(Gdiplus::Font* p, HFONT hFont, bool managed = true);

	virtual ~GdiFont()
	{
	}

	virtual void FinalRelease();

public:
	STDMETHODIMP get_HFont(UINT* p);
	STDMETHODIMP get_Height(UINT* p);
	STDMETHODIMP get_Name(LANGID langId, BSTR* outName);
	STDMETHODIMP get_Size(float* outSize);
	STDMETHODIMP get_Style(INT* outStyle);
};

class GdiBitmap : public GdiObj<IGdiBitmap, Gdiplus::Bitmap>
{
protected:
	GdiBitmap(Gdiplus::Bitmap* p) : GdiObj<IGdiBitmap, Gdiplus::Bitmap>(p)
	{
	}

public:
	STDMETHODIMP ApplyAlpha(BYTE alpha, IGdiBitmap** pp);
	STDMETHODIMP ApplyMask(IGdiBitmap* mask, VARIANT_BOOL* p);
	STDMETHODIMP BoxBlur(int radius, int iteration);
	STDMETHODIMP Clone(float x, float y, float w, float h, IGdiBitmap** pp);
	STDMETHODIMP CreateRawBitmap(IGdiRawBitmap** pp);
	STDMETHODIMP GetColorScheme(UINT count, VARIANT* outArray);
	STDMETHODIMP GetGraphics(IGdiGraphics** pp);
	STDMETHODIMP ReleaseGraphics(IGdiGraphics* p);
	STDMETHODIMP Resize(UINT w, UINT h, INT interpolationMode, IGdiBitmap** pp);
	STDMETHODIMP RotateFlip(UINT mode);
	STDMETHODIMP SaveAs(BSTR path, BSTR format, VARIANT_BOOL* p);
	STDMETHODIMP StackBlur(int radius);
	STDMETHODIMP get_Height(UINT* p);
	STDMETHODIMP get_Width(UINT* p);
};

class GdiGraphics : public GdiObj<IGdiGraphics, Gdiplus::Graphics>
{
protected:
	GdiGraphics() : GdiObj<IGdiGraphics, Gdiplus::Graphics>(NULL)
	{
	}

	static void GetRoundRectPath(Gdiplus::GraphicsPath& gp, Gdiplus::RectF& rect, float arc_width, float arc_height);

public:
	STDMETHODIMP Dispose()
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP CalcTextHeight(BSTR str, IGdiFont* font, UINT* p);
	STDMETHODIMP CalcTextWidth(BSTR str, IGdiFont* font, UINT* p);
	STDMETHODIMP DrawEllipse(float x, float y, float w, float h, float line_width, VARIANT color);
	STDMETHODIMP DrawImage(IGdiBitmap* image, float dstX, float dstY, float dstW, float dstH, float srcX, float srcY, float srcW, float srcH, float angle, BYTE alpha);
	STDMETHODIMP DrawLine(float x1, float y1, float x2, float y2, float line_width, VARIANT color);
	STDMETHODIMP DrawPolygon(VARIANT color, float line_width, VARIANT points);
	STDMETHODIMP DrawRect(float x, float y, float w, float h, float line_width, VARIANT color);
	STDMETHODIMP DrawRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, float line_width, VARIANT color);
	STDMETHODIMP DrawString(BSTR str, IGdiFont* font, VARIANT color, float x, float y, float w, float h, int flags);
	STDMETHODIMP EstimateLineWrap(BSTR str, IGdiFont* font, int max_width, VARIANT* p);
	STDMETHODIMP FillEllipse(float x, float y, float w, float h, VARIANT color);
	STDMETHODIMP FillGradRect(float x, float y, float w, float h, float angle, VARIANT color1, VARIANT color2, float focus);
	STDMETHODIMP FillPolygon(VARIANT color, INT fillmode, VARIANT points);
	STDMETHODIMP FillRoundRect(float x, float y, float w, float h, float arc_width, float arc_height, VARIANT color);
	STDMETHODIMP FillSolidRect(float x, float y, float w, float h, VARIANT color);
	STDMETHODIMP GdiAlphaBlend(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH, BYTE alpha);
	STDMETHODIMP GdiDrawBitmap(IGdiRawBitmap* bitmap, int dstX, int dstY, int dstW, int dstH, int srcX, int srcY, int srcW, int srcH);
	STDMETHODIMP GdiDrawText(BSTR str, IGdiFont* font, VARIANT color, int x, int y, int w, int h, int format, VARIANT* p);
	STDMETHODIMP MeasureString(BSTR str, IGdiFont* font, float x, float y, float w, float h, int flags, IMeasureStringInfo** pp);
	STDMETHODIMP SetInterpolationMode(INT mode);
	STDMETHODIMP SetSmoothingMode(INT mode);
	STDMETHODIMP SetTextRenderingHint(UINT mode);
	STDMETHODIMP put__ptr(void* p);
};

class GdiRawBitmap : public IDisposableImpl4<IGdiRawBitmap>
{
protected:
	UINT m_width, m_height;
	HDC m_hdc;
	HBITMAP m_hbmp, m_hbmpold;

	GdiRawBitmap(Gdiplus::Bitmap* p_bmp);

	virtual ~GdiRawBitmap()
	{
	}

	virtual void FinalRelease()
	{
		if (m_hdc)
		{
			SelectBitmap(m_hdc, m_hbmpold);
			DeleteDC(m_hdc);
			m_hdc = NULL;
		}

		if (m_hbmp)
		{
			DeleteBitmap(m_hbmp);
			m_hbmp = NULL;
		}
	}

public:
	STDMETHODIMP get_Height(UINT* p);
	STDMETHODIMP get_Width(UINT* p);
	STDMETHODIMP get__Handle(HDC* p);
};

class MeasureStringInfo : public IDispatchImpl3<IMeasureStringInfo>
{
protected:
	float m_x, m_y, m_w, m_h;
	int m_l, m_c;

	MeasureStringInfo(float x, float y, float w, float h, int l, int c) : m_x(x), m_y(y), m_w(w), m_h(h), m_l(l), m_c(c)
	{
	}

	virtual ~MeasureStringInfo()
	{
	}

public:
	STDMETHODIMP get_chars(int* p);
	STDMETHODIMP get_height(float* p);
	STDMETHODIMP get_lines(int* p);
	STDMETHODIMP get_width(float* p);
	STDMETHODIMP get_x(float* p);
	STDMETHODIMP get_y(float* p);
};

// NOTE: Do not use com_object_impl_t<> to initialize, use com_object_singleton_t<> instead.
class GdiUtils : public IDispatchImpl3<IGdiUtils>
{
protected:
	GdiUtils()
	{
	}

	virtual ~GdiUtils()
	{
	}

public:
	STDMETHODIMP CreateImage(int w, int h, IGdiBitmap** pp);
	STDMETHODIMP CreateStyleTextRender(VARIANT_BOOL pngmode, IStyleTextRender** pp);
	STDMETHODIMP Font(BSTR name, float pxSize, int style, IGdiFont** pp);
	STDMETHODIMP Image(BSTR path, IGdiBitmap** pp);
	STDMETHODIMP LoadImageAsync(UINT window_id, BSTR path, UINT* p);
};

class FbFileInfo : public IDisposableImpl4<IFbFileInfo>
{
protected:
	file_info_impl* m_info_ptr;

	FbFileInfo(file_info_impl* p_info_ptr) : m_info_ptr(p_info_ptr)
	{
	}

	virtual ~FbFileInfo()
	{
	}

	virtual void FinalRelease()
	{
		if (m_info_ptr)
		{
			delete m_info_ptr;
			m_info_ptr = NULL;
		}
	}

public:
	STDMETHODIMP InfoFind(BSTR name, UINT* p);
	STDMETHODIMP InfoName(UINT idx, BSTR* pp);
	STDMETHODIMP InfoValue(UINT idx, BSTR* pp);
	STDMETHODIMP MetaAdd(BSTR name, BSTR value, UINT* p);
	STDMETHODIMP MetaFind(BSTR name, UINT* p);
	STDMETHODIMP MetaInsertValue(UINT idx, UINT vidx, BSTR value);
	STDMETHODIMP MetaName(UINT idx, BSTR* pp);
	STDMETHODIMP MetaRemoveField(BSTR name);
	STDMETHODIMP MetaSet(BSTR name, BSTR value);
	STDMETHODIMP MetaValue(UINT idx, UINT vidx, BSTR* pp);
	STDMETHODIMP MetaValueCount(UINT idx, UINT* p);
	STDMETHODIMP get_InfoCount(UINT* p);
	STDMETHODIMP get_MetaCount(UINT* p);
	STDMETHODIMP get__ptr(void** pp);
};

class FbMetadbHandle : public IDisposableImpl4<IFbMetadbHandle>
{
protected:
	metadb_handle_ptr m_handle;

	FbMetadbHandle(const metadb_handle_ptr& src) : m_handle(src)
	{
	}

	FbMetadbHandle(metadb_handle* src) : m_handle(src)
	{
	}

	virtual ~FbMetadbHandle()
	{
	}

	virtual void FinalRelease()
	{
		m_handle.release();
	}

public:
	STDMETHODIMP Compare(IFbMetadbHandle* handle, VARIANT_BOOL* p);
	STDMETHODIMP GetFileInfo(IFbFileInfo** pp);
	STDMETHODIMP UpdateFileInfoSimple(SAFEARRAY* p);
	STDMETHODIMP get_FileSize(LONGLONG* p);
	STDMETHODIMP get_Length(double* p);
	STDMETHODIMP get_Path(BSTR* pp);
	STDMETHODIMP get_RawPath(BSTR* pp);
	STDMETHODIMP get_SubSong(UINT* p);
	STDMETHODIMP get__ptr(void** pp);
};

class FbMetadbHandleList : public IDisposableImpl4<IFbMetadbHandleList>
{
protected:
	metadb_handle_list m_handles;

	FbMetadbHandleList(metadb_handle_list_cref handles) : m_handles(handles)
	{
	}

	virtual ~FbMetadbHandleList()
	{
	}

	virtual void FinalRelease()
	{
		m_handles.remove_all();
	}

public:
	STDMETHODIMP Add(IFbMetadbHandle* handle, UINT* p);
	STDMETHODIMP AddRange(IFbMetadbHandleList* handles);
	STDMETHODIMP BSearch(IFbMetadbHandle* handle, UINT* p);
	STDMETHODIMP CalcTotalDuration(double* p);
	STDMETHODIMP CalcTotalSize(LONGLONG* p);
	STDMETHODIMP Clone(IFbMetadbHandleList** pp);
	STDMETHODIMP Find(IFbMetadbHandle* handle, UINT* p);
	STDMETHODIMP Insert(UINT index, IFbMetadbHandle* handle, UINT* outIndex);
	STDMETHODIMP InsertRange(UINT index, IFbMetadbHandleList* handles, UINT* outIndex);
	STDMETHODIMP MakeDifference(IFbMetadbHandleList* handles);
	STDMETHODIMP MakeIntersection(IFbMetadbHandleList* handles);
	STDMETHODIMP MakeUnion(IFbMetadbHandleList* handles);
	STDMETHODIMP OrderByFormat(__interface IFbTitleFormat* script, int direction);
	STDMETHODIMP OrderByPath();
	STDMETHODIMP OrderByRelativePath();
	STDMETHODIMP Remove(IFbMetadbHandle* handle);
	STDMETHODIMP RemoveAll();
	STDMETHODIMP RemoveById(UINT index);
	STDMETHODIMP RemoveRange(UINT from, UINT count);
	STDMETHODIMP Sort();
	STDMETHODIMP UpdateFileInfoSimple(SAFEARRAY* p);
	STDMETHODIMP get_Count(UINT* p);
	STDMETHODIMP get_Item(UINT index, IFbMetadbHandle** pp);
	STDMETHODIMP get__ptr(void** pp);
	STDMETHODIMP put_Item(UINT index, IFbMetadbHandle* handle);
};

class FbTitleFormat : public IDisposableImpl4<IFbTitleFormat>
{
protected:
	titleformat_object::ptr m_obj;

	FbTitleFormat(BSTR expr)
	{
		pfc::stringcvt::string_utf8_from_wide utf8 = expr;
		static_api_ptr_t<titleformat_compiler>()->compile_safe(m_obj, utf8);
	}

	virtual ~FbTitleFormat()
	{
	}

	virtual void FinalRelease()
	{
		m_obj.release();
	}

public:
	STDMETHODIMP Eval(VARIANT_BOOL force, BSTR* pp);
	STDMETHODIMP EvalWithMetadb(IFbMetadbHandle* handle, BSTR* pp);
	STDMETHODIMP get__ptr(void** pp);
};

class FbTooltip : public IDisposableImpl4<IFbTooltip>
{
protected:
	HWND m_wndtooltip;
	HWND m_wndparent;
	BSTR m_tip_buffer;
	TOOLINFO m_ti;
	panel_tooltip_param_ptr m_panel_tooltip_param_ptr;

	FbTooltip(HWND p_wndparent, const panel_tooltip_param_ptr& p_param_ptr);

	virtual ~FbTooltip()
	{
	}

	virtual void FinalRelease();

public:
	STDMETHODIMP Activate();
	STDMETHODIMP Deactivate();
	STDMETHODIMP GetDelayTime(int type, INT* p);
	STDMETHODIMP SetDelayTime(int type, int time);
	STDMETHODIMP SetMaxWidth(int width);
	STDMETHODIMP TrackPosition(int x, int y);
	STDMETHODIMP get_Text(BSTR* pp);
	STDMETHODIMP put_Text(BSTR text);
	STDMETHODIMP put_TrackActivate(VARIANT_BOOL activate);
};

class ContextMenuManager : public IDisposableImpl4<IContextMenuManager>
{
protected:
	contextmenu_manager::ptr m_cm;

	ContextMenuManager()
	{
	}

	virtual ~ContextMenuManager()
	{
	}

	virtual void FinalRelease()
	{
		m_cm.release();
	}

public:
	STDMETHODIMP BuildMenu(IMenuObj* p, int base_id, int max_id);
	STDMETHODIMP ExecuteByID(UINT id, VARIANT_BOOL* p);
	STDMETHODIMP InitContext(VARIANT handle);
	STDMETHODIMP InitNowPlaying();
};

class MainMenuManager : public IDisposableImpl4<IMainMenuManager>
{
protected:
	mainmenu_manager::ptr m_mm;

	MainMenuManager()
	{
	}

	virtual ~MainMenuManager()
	{
	}

	virtual void FinalRelease()
	{
		m_mm.release();
	}

public:
	STDMETHODIMP BuildMenu(IMenuObj* p, int base_id, int count);
	STDMETHODIMP ExecuteByID(UINT id, VARIANT_BOOL* p);
	STDMETHODIMP Init(BSTR root_name);
};

class FbProfiler : public IDispatchImpl3<IFbProfiler>
{
protected:
	pfc::string_simple m_name;
	helpers::mm_timer m_timer;

	FbProfiler(const char* p_name) : m_name(p_name) { m_timer.start(); }

	virtual ~FbProfiler()
	{
	}

public:
	STDMETHODIMP Print();
	STDMETHODIMP Reset();
	STDMETHODIMP get_Time(INT* p);
};

class FbUiSelectionHolder : public IDisposableImpl4<IFbUiSelectionHolder>
{
protected:
	ui_selection_holder::ptr m_holder;

	FbUiSelectionHolder(const ui_selection_holder::ptr& holder) : m_holder(holder)
	{
	}

	virtual ~FbUiSelectionHolder()
	{
	}

	virtual void FinalRelease()
	{
		m_holder.release();
	}

public:
	STDMETHODIMP SetPlaylistSelectionTracking();
	STDMETHODIMP SetPlaylistTracking();
	STDMETHODIMP SetSelection(IFbMetadbHandleList* handles);
};

// NOTE: Do not use com_object_impl_t<> to initialize, use com_object_singleton_t<> instead.
class FbUtils : public IDispatchImpl3<IFbUtils>
{
protected:
	FbUtils()
	{
	}

	virtual ~FbUtils()
	{
	}

public:
	STDMETHODIMP AcquireUiSelectionHolder(IFbUiSelectionHolder** outHolder);
	STDMETHODIMP AddDirectory();
	STDMETHODIMP AddFiles();
	STDMETHODIMP ClearPlaylist();
	STDMETHODIMP CreateContextMenuManager(IContextMenuManager** pp);
	STDMETHODIMP CreateMainMenuManager(IMainMenuManager** pp);
	STDMETHODIMP CreateProfiler(BSTR name, IFbProfiler** pp);
	STDMETHODIMP Exit();
	STDMETHODIMP GetFocusItem(VARIANT_BOOL force, IFbMetadbHandle** pp);
	STDMETHODIMP GetLibraryItems(IFbMetadbHandleList** outItems);
	STDMETHODIMP GetLibraryRelativePath(IFbMetadbHandle* handle, BSTR* p);
	STDMETHODIMP GetNowPlaying(IFbMetadbHandle** pp);
	STDMETHODIMP GetQueryItems(IFbMetadbHandleList* items, BSTR query, IFbMetadbHandleList** pp);
	STDMETHODIMP GetSelection(IFbMetadbHandle** pp);
	STDMETHODIMP GetSelectionType(UINT* p);
	STDMETHODIMP GetSelections(UINT flags, IFbMetadbHandleList** pp);
	STDMETHODIMP IsLibraryEnabled(VARIANT_BOOL* p);
	STDMETHODIMP IsMetadbInMediaLibrary(IFbMetadbHandle* handle, VARIANT_BOOL* p);
	STDMETHODIMP LoadPlaylist();
	STDMETHODIMP Next();
	STDMETHODIMP Pause();
	STDMETHODIMP Play();
	STDMETHODIMP PlayOrPause();
	STDMETHODIMP Prev();
	STDMETHODIMP Random();
	STDMETHODIMP RunContextCommand(BSTR command, UINT flags, VARIANT_BOOL* p);
	STDMETHODIMP RunContextCommandWithMetadb(BSTR command, VARIANT handle, UINT flags, VARIANT_BOOL* p);
	STDMETHODIMP RunMainMenuCommand(BSTR command, VARIANT_BOOL* p);
	STDMETHODIMP SavePlaylist();
	STDMETHODIMP ShowConsole();
	STDMETHODIMP ShowLibrarySearchUI(BSTR query);
	STDMETHODIMP ShowPopupMessage(BSTR msg, BSTR title, int iconid);
	STDMETHODIMP ShowPreferences();
	STDMETHODIMP Stop();
	STDMETHODIMP TitleFormat(BSTR expression, IFbTitleFormat** pp);
	STDMETHODIMP Trace(SAFEARRAY* p);
	STDMETHODIMP VolumeDown();
	STDMETHODIMP VolumeMute();
	STDMETHODIMP VolumeUp();
	STDMETHODIMP get_ComponentPath(BSTR* pp);
	STDMETHODIMP get_CursorFollowPlayback(VARIANT_BOOL* p);
	STDMETHODIMP get_FoobarPath(BSTR* pp);
	STDMETHODIMP get_IsPaused(VARIANT_BOOL* p);
	STDMETHODIMP get_IsPlaying(VARIANT_BOOL* p);
	STDMETHODIMP get_PlaybackFollowCursor(VARIANT_BOOL* p);
	STDMETHODIMP get_PlaybackLength(double* p);
	STDMETHODIMP get_PlaybackTime(double* p);
	STDMETHODIMP get_ProfilePath(BSTR* pp);
	STDMETHODIMP get_ReplaygainMode(UINT* p);
	STDMETHODIMP get_StopAfterCurrent(VARIANT_BOOL* p);
	STDMETHODIMP get_Volume(float* p);
	STDMETHODIMP put_CursorFollowPlayback(VARIANT_BOOL p);
	STDMETHODIMP put_PlaybackFollowCursor(VARIANT_BOOL p);
	STDMETHODIMP put_PlaybackTime(double time);
	STDMETHODIMP put_ReplaygainMode(UINT p);
	STDMETHODIMP put_StopAfterCurrent(VARIANT_BOOL p);
	STDMETHODIMP put_Volume(float value);
};

class MenuObj : public IDisposableImpl4<IMenuObj>
{
protected:
	HMENU m_hMenu;
	HWND m_wnd_parent;
	bool m_has_detached;

	MenuObj(HWND wnd_parent) : m_wnd_parent(wnd_parent), m_has_detached(false)
	{
		m_hMenu = ::CreatePopupMenu();
	}

	virtual ~MenuObj()
	{
	}

	virtual void FinalRelease()
	{
		if (!m_has_detached && m_hMenu && IsMenu(m_hMenu))
		{
			DestroyMenu(m_hMenu);
			m_hMenu = NULL;
		}
	}

public:
	STDMETHODIMP AppendMenuItem(UINT flags, UINT item_id, BSTR text);
	STDMETHODIMP AppendMenuSeparator();
	STDMETHODIMP AppendTo(IMenuObj* parent, UINT flags, BSTR text);
	STDMETHODIMP CheckMenuItem(UINT id_or_pos, VARIANT_BOOL check, VARIANT_BOOL bypos);
	STDMETHODIMP CheckMenuRadioItem(UINT first, UINT last, UINT check, VARIANT_BOOL bypos);
	STDMETHODIMP EnableMenuItem(UINT id_or_pos, UINT enable, VARIANT_BOOL bypos);
	STDMETHODIMP TrackPopupMenu(int x, int y, UINT flags, UINT* item_id);
	STDMETHODIMP get_ID(UINT* p);
};

// NOTE: Do not use com_object_impl_t<> to initialize, use com_object_singleton_t<> instead.
class JSUtils : public IDispatchImpl3<IJSUtils>
{
protected:
	JSUtils()
	{
	}

	virtual ~JSUtils()
	{
	}

public:
	STDMETHODIMP CheckComponent(BSTR name, VARIANT_BOOL is_dll, VARIANT_BOOL* p);
	STDMETHODIMP CheckFont(BSTR name, VARIANT_BOOL* p);
	STDMETHODIMP FileTest(BSTR path, BSTR mode, VARIANT* p);
	STDMETHODIMP FormatDuration(double p, BSTR* pp);
	STDMETHODIMP FormatFileSize(LONGLONG p, BSTR* pp);
	STDMETHODIMP GetAlbumArtAsync(UINT window_id, IFbMetadbHandle* handle, int art_id, VARIANT_BOOL need_stub, VARIANT_BOOL only_embed, VARIANT_BOOL no_load, UINT* p);
	STDMETHODIMP GetAlbumArtEmbedded(BSTR rawpath, int art_id, IGdiBitmap** pp);
	STDMETHODIMP GetAlbumArtV2(IFbMetadbHandle* handle, int art_id, VARIANT_BOOL need_stub, IGdiBitmap** pp);
	STDMETHODIMP GetSysColor(UINT index, int* p);
	STDMETHODIMP GetSystemMetrics(UINT index, INT* p);
	STDMETHODIMP Glob(BSTR pattern, UINT exc_mask, UINT inc_mask, VARIANT* p);
	STDMETHODIMP IsKeyPressed(UINT vkey, VARIANT_BOOL* p);
	STDMETHODIMP MapString(BSTR str, UINT lcid, UINT flags, BSTR* pp);
	STDMETHODIMP PathWildcardMatch(BSTR pattern, BSTR str, VARIANT_BOOL* p);
	STDMETHODIMP ReadINI(BSTR filename, BSTR section, BSTR key, VARIANT defaultval, BSTR* pp);
	STDMETHODIMP ReadTextFile(BSTR filename, UINT codepage, BSTR* pp);
	STDMETHODIMP WriteINI(BSTR filename, BSTR section, BSTR key, VARIANT val, VARIANT_BOOL* p);
	STDMETHODIMP get_Version(UINT* v);
};

// forward declaration
namespace TextDesign
{
	class IOutlineText;
}

class StyleTextRender : public IDisposableImpl4<IStyleTextRender>
{
protected:
	TextDesign::IOutlineText* m_pOutLineText;
	bool m_pngmode;

	StyleTextRender(bool pngmode);

	virtual ~StyleTextRender()
	{
	}

	virtual void FinalRelease();

public:
	STDMETHODIMP DiffusedShadow(VARIANT color, int thickness, int offset_x, int offset_y);
	STDMETHODIMP DoubleOutLineText(int text_color, int outline_color1, int outline_color2, int outline_width1, int outline_width2);
	STDMETHODIMP EnableShadow(VARIANT_BOOL enable);
	STDMETHODIMP GlowText(int text_color, int glow_color, int glow_width);
	STDMETHODIMP OutLineText(int text_color, int outline_color, int outline_width);
	STDMETHODIMP RenderStringPoint(IGdiGraphics* g, BSTR str, IGdiFont* font, int x, int y, int flags, VARIANT_BOOL* p);
	STDMETHODIMP RenderStringRect(IGdiGraphics* g, BSTR str, IGdiFont* font, int x, int y, int w, int h, int flags, VARIANT_BOOL* p);
	STDMETHODIMP ResetShadow();
	STDMETHODIMP SetPngImage(IGdiBitmap* img);
	STDMETHODIMP SetShadowBackgroundColor(VARIANT color, int width, int height);
	STDMETHODIMP SetShadowBackgroundImage(IGdiBitmap* img);
	STDMETHODIMP Shadow(VARIANT color, int thickness, int offset_x, int offset_y);
};

class ThemeManager : public IDisposableImpl4<IThemeManager>
{
protected:
	HTHEME m_theme;
	int m_partid;
	int m_stateid;

	ThemeManager(HWND hwnd, BSTR classlist) : m_theme(NULL), m_partid(0), m_stateid(0)
	{
		m_theme = OpenThemeData(hwnd, classlist);

		if (!m_theme) throw pfc::exception_invalid_params();
	}

	virtual ~ThemeManager()
	{
	}

	virtual void FinalRelease()
	{
		if (m_theme)
		{
			CloseThemeData(m_theme);
			m_theme = NULL;
		}
	}

public:
	STDMETHODIMP DrawThemeBackground(IGdiGraphics* gr, int x, int y, int w, int h, int clip_x, int clip_y, int clip_w, int clip_h);
	STDMETHODIMP IsThemePartDefined(int partid, int stateid, VARIANT_BOOL* p);
	STDMETHODIMP SetPartAndStateID(int partid, int stateid);
};

class DropSourceAction : public IDisposableImpl4<IDropSourceAction>
{
public:
	enum t_action_mode
	{
		kActionModeNone = 0,
		kActionModePlaylist,
		kActionModeFilenames,
	};

protected:
	// -1 means active playlist
	int m_playlist_idx;
	t_action_mode m_action_mode;
	bool m_to_select;
	bool m_parsable;

	DropSourceAction() { Reset(); }

	virtual ~DropSourceAction()
	{
	}

	virtual void FinalRelease()
	{
	}

public:
	void Reset()
	{
		m_playlist_idx = -1;
		m_to_select = true;
		m_action_mode = kActionModeNone;
		m_parsable = false;
	}

	t_action_mode& Mode() { return m_action_mode; }
	bool& Parsable() { return m_parsable; }
	int& Playlist() { return m_playlist_idx; }
	bool& ToSelect() { return m_to_select; }

	STDMETHODIMP ToPlaylist();
	STDMETHODIMP get_Mode(int* mode);
	STDMETHODIMP get_Parsable(VARIANT_BOOL* parsable);
	STDMETHODIMP get_Playlist(int* id);
	STDMETHODIMP get_ToSelect(VARIANT_BOOL* to_select);
	STDMETHODIMP put_Parsable(VARIANT_BOOL parsable);
	STDMETHODIMP put_Playlist(int id);
	STDMETHODIMP put_ToSelect(VARIANT_BOOL to_select);
};

// NOTE: Do not use com_object_impl_t<> to initialize, use com_object_singleton_t<> instead.
class FbPlaylistManager : public IDispatchImpl3<IFbPlaylistManager>
{
protected:
	FbPlaylistManager() : m_fbPlaylistRecyclerManager(NULL)
	{
	}

public:
	STDMETHODIMP AddItemToPlaybackQueue(IFbMetadbHandle* handle);
	STDMETHODIMP AddLocations(UINT playlistIndex, VARIANT locations, VARIANT_BOOL select);
	STDMETHODIMP AddPlaylistItemToPlaybackQueue(UINT playlistIndex, UINT playlistItemIndex);
	STDMETHODIMP ClearPlaylist(UINT playlistIndex);
	STDMETHODIMP ClearPlaylistSelection(UINT playlistIndex);
	STDMETHODIMP CreateAutoPlaylist(UINT idx, BSTR name, BSTR query, BSTR sort, UINT flags, UINT* p);
	STDMETHODIMP CreatePlaybackQueueItem(IFbPlaybackQueueItem** outPlaybackQueueItem);
	STDMETHODIMP CreatePlaylist(UINT playlistIndex, BSTR name, UINT* outPlaylistIndex);
	STDMETHODIMP DuplicatePlaylist(UINT from, BSTR name, UINT* outPlaylistIndex);
	STDMETHODIMP EnsurePlaylistItemVisible(UINT playlistIndex, UINT itemIndex);
	STDMETHODIMP ExecutePlaylistDefaultAction(UINT playlistIndex, UINT playlistItemIndex, VARIANT_BOOL* outSuccess);
	STDMETHODIMP FindPlaybackQueueItemIndex(IFbMetadbHandle* handle, UINT playlistIndex, UINT playlistItemIndex, INT* outIndex);
	STDMETHODIMP FlushPlaybackQueue();
	STDMETHODIMP GetPlaybackQueueContents(VARIANT* outContents);
	STDMETHODIMP GetPlaybackQueueCount(UINT* outCount);
	STDMETHODIMP GetPlayingItemLocation(IFbPlayingItemLocation** outPlayingLocation);
	STDMETHODIMP GetPlaylistFocusItemIndex(UINT playlistIndex, INT* outPlaylistItemIndex);
	STDMETHODIMP GetPlaylistItems(UINT playlistIndex, IFbMetadbHandleList** outItems);
	STDMETHODIMP GetPlaylistName(UINT playlistIndex, BSTR* outName);
	STDMETHODIMP GetPlaylistSelectedItems(UINT playlistIndex, IFbMetadbHandleList** outItems);
	STDMETHODIMP InsertPlaylistItems(UINT playlistIndex, UINT base, IFbMetadbHandleList* handles, VARIANT_BOOL select, UINT* outSize);
	STDMETHODIMP InsertPlaylistItemsFilter(UINT playlistIndex, UINT base, IFbMetadbHandleList* handles, VARIANT_BOOL select, UINT* outSize);
	STDMETHODIMP IsAutoPlaylist(UINT idx, VARIANT_BOOL* p);
	STDMETHODIMP IsPlaybackQueueActive(VARIANT_BOOL* outIsActive);
	STDMETHODIMP IsPlaylistItemSelected(UINT playlistIndex, UINT playlistItemIndex, UINT* outSelected);
	STDMETHODIMP IsPlaylistLocked(UINT playlistIndex, VARIANT_BOOL* p);
	STDMETHODIMP MovePlaylist(UINT from, UINT to, VARIANT_BOOL* outSuccess);
	STDMETHODIMP MovePlaylistSelection(UINT playlistIndex, int delta);
	STDMETHODIMP RemoveItemFromPlaybackQueue(UINT index);
	STDMETHODIMP RemoveItemsFromPlaybackQueue(VARIANT affectedItems);
	STDMETHODIMP RemovePlaylist(UINT playlistIndex, VARIANT_BOOL* outSuccess);
	STDMETHODIMP RemovePlaylistSelection(UINT playlistIndex, VARIANT_BOOL crop);
	STDMETHODIMP RenamePlaylist(UINT playlistIndex, BSTR name, VARIANT_BOOL* outSuccess);
	STDMETHODIMP SetActivePlaylistContext();
	STDMETHODIMP SetPlaylistFocusItem(UINT playlistIndex, UINT itemIndex);
	STDMETHODIMP SetPlaylistFocusItemByHandle(UINT playlistIndex, IFbMetadbHandle* item);
	STDMETHODIMP SetPlaylistSelection(UINT playlistIndex, VARIANT affectedItems, VARIANT_BOOL state);
	STDMETHODIMP SetPlaylistSelectionSingle(UINT playlistIndex, UINT itemIndex, VARIANT_BOOL state);
	STDMETHODIMP ShowAutoPlaylistUI(UINT idx, VARIANT_BOOL* p);
	STDMETHODIMP SortByFormat(UINT playlistIndex, BSTR pattern, VARIANT_BOOL selOnly, VARIANT_BOOL* outSuccess);
	STDMETHODIMP SortByFormatV2(UINT playlistIndex, BSTR pattern, INT direction, VARIANT_BOOL* outSuccess);
	STDMETHODIMP UndoBackup(UINT playlistIndex);
	STDMETHODIMP UndoRestore(UINT playlistIndex);
	STDMETHODIMP get_ActivePlaylist(UINT* outPlaylistIndex);
	STDMETHODIMP get_PlaybackOrder(UINT* outOrder);
	STDMETHODIMP get_PlayingPlaylist(UINT* outPlaylistIndex);
	STDMETHODIMP get_PlaylistCount(UINT* outCount);
	STDMETHODIMP get_PlaylistItemCount(UINT playlistIndex, UINT* outCount);
	STDMETHODIMP get_PlaylistRecyclerManager(__interface IFbPlaylistRecyclerManager** outRecyclerManager);
	STDMETHODIMP put_ActivePlaylist(UINT playlistIndex);
	STDMETHODIMP put_PlaybackOrder(UINT order);
	STDMETHODIMP put_PlayingPlaylist(UINT playlistIndex);

private:
	IFbPlaylistRecyclerManagerPtr m_fbPlaylistRecyclerManager;
};

class FbPlaybackQueueItem : public IDisposableImpl4<IFbPlaybackQueueItem>
{
protected:
	t_playback_queue_item m_playback_queue_item;

	FbPlaybackQueueItem()
	{
	}

	FbPlaybackQueueItem(const t_playback_queue_item& playbackQueueItem);
	virtual ~FbPlaybackQueueItem();
	virtual void FinalRelease();

public:
	STDMETHODIMP Equals(IFbPlaybackQueueItem* item, VARIANT_BOOL* outEquals);
	STDMETHODIMP get_Handle(IFbMetadbHandle** outHandle);
	STDMETHODIMP get_PlaylistIndex(UINT* outPlaylistIndex);
	STDMETHODIMP get_PlaylistItemIndex(UINT* outItemIndex);
	STDMETHODIMP get__ptr(void** pp);
	STDMETHODIMP put_Handle(IFbMetadbHandle* handle);
	STDMETHODIMP put_PlaylistIndex(UINT playlistIndex);
	STDMETHODIMP put_PlaylistItemIndex(UINT itemIndex);
};

class FbPlayingItemLocation : public IDispatchImpl3<IFbPlayingItemLocation>
{
protected:
	bool m_isValid;
	t_size m_playlistIndex;
	t_size m_itemIndex;

	FbPlayingItemLocation(bool isValid, t_size playlistIndex, t_size itemInex)
		: m_isValid(isValid), m_playlistIndex(playlistIndex), m_itemIndex(itemInex)
	{
	}

public:
	STDMETHODIMP get_IsValid(VARIANT_BOOL* outIsValid);
	STDMETHODIMP get_PlaylistIndex(UINT* outPlaylistIndex);
	STDMETHODIMP get_PlaylistItemIndex(UINT* outPlaylistItemIndex);
};

class FbPlaylistRecyclerManager : public IDispatchImpl3<IFbPlaylistRecyclerManager>
{
public:
	STDMETHODIMP FindById(UINT id, UINT* outId);
	STDMETHODIMP Purge(VARIANT affectedItems);
	STDMETHODIMP Restore(UINT index);
	STDMETHODIMP RestoreById(UINT id);
	STDMETHODIMP get_Content(UINT index, IFbMetadbHandleList** outContent);
	STDMETHODIMP get_Count(UINT* outCount);
	STDMETHODIMP get_Id(UINT index, UINT* outId);
	STDMETHODIMP get_Name(UINT index, BSTR* outName);
};
