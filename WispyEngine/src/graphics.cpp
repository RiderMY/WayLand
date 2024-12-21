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

  if (SUCCEEDED(hr)) {
    render_target_->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED); // Turning Off Anti-Aliasing

    hr = render_target_->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &brush_);
  }

  if (SUCCEEDED(hr)) hr = img_factory_.CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_ALL);

  if (SUCCEEDED(hr)) hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown **>(&write_factory_));

  if (SUCCEEDED(hr)) hr = write_factory_->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.0f, L"en-us", &text_format_);

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
  render_target_->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), brush_, 3.0f);
}

void Graphics::FillRectangle(float x, float y, float width, float height, const D2D1_COLOR_F &color) {
  brush_->SetColor(color);
  render_target_->FillRectangle(D2D1::RectF(x, y, x + width, y + height), brush_);
}

void Graphics::DrawEllipse(float x, float y, float radius_x, float radius_y, const D2D1_COLOR_F &color) {
  brush_->SetColor(color);
  render_target_->DrawEllipse(D2D1::Ellipse(D2D1::Point2(x, y), radius_x, radius_y), brush_, 3.0f);
}


CComPtr<ID2D1Bitmap> Graphics::LoadBitmapFromFilename(LPCWSTR filename) {
  CComPtr<ID2D1Bitmap> bitmap;

  CComPtr<IWICBitmapDecoder> decoder;
  CComPtr<IWICBitmapFrameDecode> frame;
  CComPtr<IWICFormatConverter> format_converter;

  HRESULT hr = img_factory_->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);

  if (SUCCEEDED(hr)) hr = decoder->GetFrame(0, &frame);

  if (SUCCEEDED(hr)) hr = img_factory_->CreateFormatConverter(&format_converter);

  if (SUCCEEDED(hr)) hr = format_converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

  if (SUCCEEDED(hr)) hr = render_target_->CreateBitmapFromWicBitmap(format_converter, NULL, &bitmap);

  if (FAILED(hr)) MessageBoxEx(NULL, GetErrorMessage(hr), L"Bitmap Creation Failed!", MB_ICONEXCLAMATION | MB_OK, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));

  return bitmap;
}

void Graphics::DrawBitmap(const CComPtr<ID2D1Bitmap> &bitmap, float x, float y, float scale_x, float scale_y, unsigned int slices_x, unsigned int slices_y, unsigned int index_x, unsigned int index_y) {
  float src_width = bitmap->GetSize().width / (slices_x + 1u), src_height = bitmap->GetSize().height / (slices_y + 1u);
  float src_x = index_x * src_width, src_y = index_y * src_height;
  D2D1_RECT_F src_rect = D2D1::RectF(src_x, src_y, src_x + src_width, src_y + src_height);
  D2D1_RECT_F dest_rect = D2D1::RectF(x, y, x + src_width * scale_x, y + src_height * scale_y);
  render_target_->DrawBitmap(bitmap, dest_rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, src_rect);
}

void Graphics::DrawBasicText(LPCWSTR text, float x, float y, float width, float height) {
  brush_->SetColor(D2D1::ColorF(1.0f, 1.0f, 1.0f));
  render_target_->DrawTextW(text, static_cast<unsigned int>(wcslen(text)), text_format_, D2D1::RectF(x, y, x + width, y + height), brush_);
}

Graphics::CoInitializationRAII::CoInitializationRAII() {
  CoInitialize(NULL);
}

Graphics::CoInitializationRAII::~CoInitializationRAII() {
  CoUninitialize();
}

}
