#pragma once

#include <Windows.h>
#include <atlbase.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "WindowsCodecs.lib")

namespace wispy {

class Graphics {
public:
  Graphics() = default;
  ~Graphics() = default;

  HRESULT CreateGraphicsResources(HWND hwnd);

  void BeginDraw();
  void EndDraw();

  void Clear(const D2D1_COLOR_F &color);

  void DrawRectangle(float x, float y, float width, float height, const D2D1_COLOR_F &color);
  void FillRectangle(float x, float y, float width, float height, const D2D1_COLOR_F &color);

  void DrawEllipse(float x, float y, float radius_x, float radius_y, const D2D1_COLOR_F &color);

  CComPtr<ID2D1Bitmap> LoadBitmapFromFilename(LPCWSTR filename);
  void DrawBitmap(const CComPtr<ID2D1Bitmap> &bitmap, float x, float y, float scale_x, float scale_y);

  void DrawBasicText(LPCWSTR text, float x, float y, float width, float height);

private:
  inline LPCWSTR GetErrorMessage(HRESULT hr) {
    LPCWSTR msg_buffer = nullptr;
    DWORD buffer_size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&msg_buffer), 0, NULL);
    if (buffer_size == 0) {
      return L"Unidentified Error Code.";
    }
    return msg_buffer;
  }

  class CoInitializationRAII {
  public:
    CoInitializationRAII();
    ~CoInitializationRAII();
  } therapist_;

  CComPtr<ID2D1Factory> factory_;
  CComPtr<ID2D1HwndRenderTarget> render_target_;
  CComPtr<ID2D1SolidColorBrush> brush_;

  CComPtr<IWICImagingFactory> img_factory_;
  CComPtr<IWICBitmapDecoder> decoder_;
  CComPtr<IWICBitmapFrameDecode> frame_;
  CComPtr<IWICFormatConverter> format_converter_;

  CComPtr<IDWriteFactory> write_factory_;
  CComPtr<IDWriteTextFormat> text_format_;
};

}
