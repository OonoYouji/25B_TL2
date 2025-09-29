#include "TextureConverter.h"

/// std
#define WIN32_LEAN_AND_MEAN   // �� �����擪�ɓ����ƌy�ʉ��ł���i�����j
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

	/// �t�H���_�p�X�ƃt�@�C�����𕪗�
	SeparateFilePath(filePathW);
}

void TextureConverter::SeparateFilePath(const std::wstring& _filepath) {
	size_t pos1;
	std::wstring exceptExt;

	/// �g���q�𕪗�
	pos1 = _filepath.rfind('.');
	if (pos1 != std::wstring::npos) {
		fileExtension_ = _filepath.substr(pos1 + 1, _filepath.size() - pos1 - 1);
		exceptExt = _filepath.substr(0, pos1);
	} else {
		fileExtension_ = L"";
		exceptExt = _filepath;
	}

	/// �f�B���N�g���p�X�ƃt�@�C�����𕪗� ('\\'�ŋ�؂�)
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	/// �f�B���N�g���p�X�ƃt�@�C�����𕪗� ('/'�ŋ�؂�)
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}


	/// ��؂蕶�����Ȃ��ꍇ
	directoryPath_ = L"";
	fileName_ = exceptExt;
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

void TextureConverter::SaveDDSTextureToFile() {
	/// SRGB�Ƃ��Ĉ���
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	/// �o�̓t�@�C����ݒ�ADDS�t�@�C���ɏ����o��
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
