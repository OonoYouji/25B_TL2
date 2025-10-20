
/// std
#include <cstdio>
#include <cstdlib>
#include <cassert>

/// externals
#include "External/DirectXTex/DirectXTex.h"

/// engine
#include "Project/Converter/TextureConverter.h"

enum Argument {
	kApplicationPath,
	kFilePath,
	NumArgument
};

int main(int _argc, char* _argv[]) {

	if(_argc < NumArgument) {
		TextureConverter::OutputUsage();
		return 0;
	}

	assert(_argc >= NumArgument);

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));


	int numOptions = _argc - NumArgument;
	char** options = _argv + NumArgument;

	TextureConverter converter;
	converter.ConvertTextureWICToDDS(_argv[kFilePath], numOptions, options);

	CoUninitialize();

	//system("pause");
	return 0;
}


