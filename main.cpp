
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

	assert(_argc >= NumArgument);

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	TextureConverter converter;
	converter.ConvertTextureWICToDDS(_argv[kFilePath]);

	CoUninitialize();

	system("pause");
	return 0;
}


