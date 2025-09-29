#pragma once

/// std
#include <string>


/// //////////////////////////////////////////////////////
/// TextureConverter
/// //////////////////////////////////////////////////////
class TextureConverter {
public:
	/// ====================================================
	/// public : methods
	/// ====================================================

	void ConvertTextureWICToDDS(const std::string& _filepath);
	void LoadWICTextureFromFile(const std::string& _filepath);

private:
	/// ====================================================
	/// private : methods
	/// ====================================================

	static std::wstring ConvertMultiByteStringToWideString(const std::string& _mString);

};