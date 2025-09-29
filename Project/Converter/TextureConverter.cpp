#include "TextureConverter.h"

#include "../../External/DirectXTex/DirectXTex.h"
/// std
#define WIN32_LEAN_AND_MEAN   // �� �����擪�ɓ����ƌy�ʉ��ł���i�����j
#include <Windows.h>



void TextureConverter::ConvertTextureWICToDDS(const std::string& _filepath) {
	LoadWICTextureFromFile(_filepath);

}

void TextureConverter::LoadWICTextureFromFile(const std::string& _filepath) {
	std::wstring filePathW = ConvertMultiByteStringToWideString(_filepath);


}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& _mString) {
	/// ��������̏ꍇ�͋󕶎���Ԃ�
	if (_mString.empty()) {
		return std::wstring();
	}

	/// �ϊ���̃T�C�Y���擾
	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, _mString.data(), static_cast<int>(_mString.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}

	/// �ϊ�
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, _mString.data(), static_cast<int>(_mString.size()), result.data(), sizeNeeded);
	return result;
}
