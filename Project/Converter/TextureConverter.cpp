#include "TextureConverter.h"

/// std
#define WIN32_LEAN_AND_MEAN   // ← これを先頭に入れると軽量化できる（推奨）
#include <Windows.h>



void TextureConverter::ConvertTextureWICToDDS(const std::string& _filepath) {
	LoadWICTextureFromFile(_filepath);
	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string& _filepath) {
	std::wstring filePathW = ConvertMultiByteStringToWideString(_filepath);
	HRESULT hr = DirectX::LoadFromWICFile(
		filePathW.c_str(),
		DirectX::WIC_FLAGS_NONE,
		&metadata_,
		scratchImage_
	);
	assert(SUCCEEDED(hr));

	/// フォルダパスとファイル名を分離
	SeparateFilePath(filePathW);
}

void TextureConverter::SeparateFilePath(const std::wstring& _filepath) {
	size_t pos1;
	std::wstring exceptExt;

	/// 拡張子を分離
	pos1 = _filepath.rfind('.');
	if (pos1 != std::wstring::npos) {
		fileExtension_ = _filepath.substr(pos1 + 1, _filepath.size() - pos1 - 1);
		exceptExt = _filepath.substr(0, pos1);
	} else {
		fileExtension_ = L"";
		exceptExt = _filepath;
	}

	/// ディレクトリパスとファイル名を分離 ('\\'で区切る)
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	/// ディレクトリパスとファイル名を分離 ('/'で区切る)
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}


	/// 区切り文字がない場合
	directoryPath_ = L"";
	fileName_ = exceptExt;
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

void TextureConverter::SaveDDSTextureToFile() {
	/// SRGBとして扱う
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	/// 出力ファイルを設定、DDSファイルに書き出す
	std::wstring filepath = directoryPath_ + fileName_ + L".dds";
	HRESULT hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		metadata_,
		DirectX::DDS_FLAGS_NONE,
		filepath.c_str()
	);
	assert(SUCCEEDED(hr));
}
