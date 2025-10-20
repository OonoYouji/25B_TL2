#include "TextureConverter.h"

/// std
#define WIN32_LEAN_AND_MEAN   // ← これを先頭に入れると軽量化できる（推奨）
#include <Windows.h>



void TextureConverter::OutputUsage() {
	printf("画像ファイルをWIC形式からDDS形式に変換します。\n");
	printf("\n");
	printf("TextureConverter.exe [ドライブ:][パス][ファイル名]\n");
	printf("\n");
	printf(" [ドライブ:][パス][ファイル名]: 変換したいWIC形式の画像ファイルを指定します。 \n");

	/// -ml オプションの説明
	printf(" -ml [mipレベル数]: 生成するMipMapのレベル数を指定します。\n");

}

void TextureConverter::ConvertTextureWICToDDS(const std::string& _filepath, int _numOptions, char* _options[]) {
	LoadWICTextureFromFile(_filepath);
	SaveDDSTextureToFile(_numOptions, _options);
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

void TextureConverter::SaveDDSTextureToFile(int _numOptions, char* _options[]) {

	size_t mipLevel = 0;
	for (int i = 0; i < _numOptions; i++) {
		if(std::string(_options[i]) == "-ml") {
			/// mip levelを指定
			mipLevel = std::stoi(_options[i + 1]);
			break;
		}
	}

	/// MipMapを生成
	DirectX::ScratchImage mipChain;
	HRESULT hr = DirectX::GenerateMipMaps(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT,
		mipLevel, mipChain
	);

	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	/// 圧縮形式に変換
	DirectX::ScratchImage converted;
	hr = DirectX::Compress(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB,
		DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT | DirectX::TEX_COMPRESS_PARALLEL,
		1.0f, converted
	);

	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}


	/// SRGBとして扱う
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	/// 出力ファイルを設定、DDSファイルに書き出す
	std::wstring filepath = directoryPath_ + fileName_ + L".dds";
	hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(),
		scratchImage_.GetImageCount(),
		metadata_,
		DirectX::DDS_FLAGS_NONE,
		filepath.c_str()
	);
	assert(SUCCEEDED(hr));
}
