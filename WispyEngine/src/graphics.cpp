#include "graphics.h"

namespace wispy {

HRESULT Graphics::CreateGraphicsResources(HWND hwnd) {
  HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_);

  if (SUCCEEDED(hr)) {
    RECT client_rect = { };
    GetClientRect(hwnd, &client_rect);

    D2D1_SIZE_U size = D2D1::SizeU(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);

    hr = factory_->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hwnd, size), &render_target_);
  }

  if (SUCCEEDED(hr)) hr = render_target_->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &brush_);

  if (SUCCEEDED(hr)) hr = img_factory_.CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_ALL);

  if (FAILED(hr)) MessageBoxEx(NULL, GetErrorMessage(hr), L"D2D1 Initialization Failed!", MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));

  return hr;
}

void Graphics::BeginDraw() {
  render_target_->BeginDraw();
}

void Graphics::EndDraw() {
  render_target_->EndDraw();
}

void Graphics::Clear(const D2D1_COLOR_F &color) {
  render_target_->Clear(color);
}

void Graphics::DrawRectangle(float x, float y, float width, float height, const D2D1_COLOR_F &color) {
  brush_->SetColor(color);
  render_target_->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), brush_);
}

void Graphics::FillRectangle(float x, float y, float width, float height, const D2D1_COLOR_F &color) {
  brush_->SetColor(color);
  render_target_->FillRectangle(D2D1::RectF(x, y, x + width, y + height), brush_);
}

CComPtr<ID2D1Bitmap> Graphics::LoadBitmapFromFilename(LPCWSTR filename) {
  CComPtr<ID2D1Bitmap> bitmap;

  HRESULT hr = img_factory_->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder_);

  if (SUCCEEDED(hr)) hr = decoder_->GetFrame(0, &frame_);

  if (SUCCEEDED(hr)) hr = img_factory_->CreateFormatConverter(&format_converter_);

  if (SUCCEEDED(hr)) hr = format_converter_->Initialize(frame_, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

  if (SUCCEEDED(hr)) hr = render_target_->CreateBitmapFromWicBitmap(format_converter_, NULL, &bitmap);

  if (FAILED(hr)) MessageBoxEx(NULL, GetErrorMessage(hr), L"Bitmap Creation Failed!", MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));

  return bitmap;
}

void Graphics::DrawBitmap(const CComPtr<ID2D1Bitmap> &bitmap, float x, float y, float scale_x, float scale_y) {
  D2D1_RECT_F src_rect = D2D1::RectF(0, 0, bitmap->GetSize().width, bitmap->GetSize().height);
  D2D1_RECT_F dest_rect = D2D1::RectF(x, y, x + bitmap->GetSize().width * scale_x, y + bitmap->GetSize().height * scale_y);
  render_target_->DrawBitmap(bitmap, dest_rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, src_rect);
}

Graphics::CoInitializationRAII::CoInitializationRAII() {
  CoInitialize(NULL);
}

Graphics::CoInitializationRAII::~CoInitializationRAII() {
  CoUninitialize();
}

}
