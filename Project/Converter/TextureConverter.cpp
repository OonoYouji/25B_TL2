#include "TextureConverter.h"

#include "../../External/DirectXTex/DirectXTex.h"
/// std
#define WIN32_LEAN_AND_MEAN   // ← これを先頭に入れると軽量化できる（推奨）
#include <Windows.h>



void TextureConverter::ConvertTextureWICToDDS(const std::string& _filepath) {
	LoadWICTextureFromFile(_filepath);

}

void TextureConverter::LoadWICTextureFromFile(const std::string& _filepath) {
	std::wstring filePathW = ConvertMultiByteStringToWideString(_filepath);


}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& _mString) {
	/// 引数が空の場合は空文字を返す
	if (_mString.empty()) {
		return std::wstring();
	}

	/// 変換後のサイズを取得
	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, _mString.data(), static_cast<int>(_mString.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}

	/// 変換
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, _mString.data(), static_cast<int>(_mString.size()), result.data(), sizeNeeded);
	return result;
}
