#pragma once

/// std
#include <string>

#include "../../External/DirectXTex/DirectXTex.h"

/// //////////////////////////////////////////////////////
/// TextureConverter
/// //////////////////////////////////////////////////////
class TextureConverter {
public:
	/// ====================================================
	/// public : methods
	/// ====================================================

	/// @brief 使用方法を出力する
	static void OutputUsage();

	void ConvertTextureWICToDDS(const std::string& _filepath, int _numOptions = 0, char* _options[] = nullptr);
	void LoadWICTextureFromFile(const std::string& _filepath);
	void SeparateFilePath(const std::wstring& _filepath);

private:
	/// ====================================================
	/// private : methods
	/// ====================================================

	static std::wstring ConvertMultiByteStringToWideString(const std::string& _mString);

	void SaveDDSTextureToFile(int _numOptions, char* _options[]);

private:
	/// ====================================================
	/// private : objects
	/// ====================================================

	/// ----- texture data ----- ///
	DirectX::TexMetadata metadata_;
	DirectX::ScratchImage scratchImage_;

	/// ----- file path data ----- ///
	std::wstring directoryPath_;
	std::wstring fileName_;
	std::wstring fileExtension_;
};