#include "TextureConverter.h"

/// std
#define WIN32_LEAN_AND_MEAN   // �� �����擪�ɓ����ƌy�ʉ��ł���i�����j
#include <Windows.h>



void TextureConverter::OutputUsage() {
	printf("�摜�t�@�C����WIC�`������DDS�`���ɕϊ����܂��B\n");
	printf("\n");
	printf("TextureConverter.exe [�h���C�u:][�p�X][�t�@�C����]\n");
	printf("\n");
	printf(" [�h���C�u:][�p�X][�t�@�C����]: �ϊ�������WIC�`���̉摜�t�@�C�����w�肵�܂��B \n");

	/// -ml �I�v�V�����̐���
	printf(" -ml [mip���x����]: ��������MipMap�̃��x�������w�肵�܂��B\n");

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

void TextureConverter::SaveDDSTextureToFile(int _numOptions, char* _options[]) {

	size_t mipLevel = 0;
	for (int i = 0; i < _numOptions; i++) {
		if(std::string(_options[i]) == "-ml") {
			/// mip level���w��
			mipLevel = std::stoi(_options[i + 1]);
			break;
		}
	}

	/// MipMap�𐶐�
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

	/// ���k�`���ɕϊ�
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


	/// SRGB�Ƃ��Ĉ���
	metadata_.format = DirectX::MakeSRGB(metadata_.format);

	/// �o�̓t�@�C����ݒ�ADDS�t�@�C���ɏ����o��
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
