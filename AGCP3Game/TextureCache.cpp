#include "TextureCache.h"

#include <DDSTextureLoader.h>
#include <filesystem>
#include "string"
#include "Util.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void TexCache::Release()
{
	for (auto& pair : mCache)
		ReleaseCOM(pair.second.pTex);
	mCache.clear();
}

ID3D12Resource* TexCache::LoadTexture(ID3D12Device* pDevice, const std::string& fileName, const std::string& texName,
	bool appendPath, const vector<RECTF>* frames)
{
	string name = texName;
	if (name.empty())
	{
		std::filesystem::path p(fileName);
		name = p.stem().string();
	}

	//search the cache
	MyMap::iterator it = mCache.find(name);
	if (it != mCache.end())
		return (*it).second.pTex;

	//prepare the path for loading
	const string* pPath = &fileName;
	string path;
	if (appendPath)
	{
		path = mAssetPath + fileName;
		pPath = &path;
	}
	std::wstring ws(pPath->begin(), pPath->end());
	//load it
	DDS_ALPHA_MODE alpha;
	ID3D12Resource* pT;
	if (CreateDDSTextureFromFile(pDevice, ws.c_str(), nullptr, &pT, 0, &alpha) != S_OK)
	{
		WDBOUT("Cannot load " << *pPath << "\n");
		assert(false);
	}
	//save it
	assert(pT);
	mCache.insert(MyMap::value_type(name, Data(fileName, pT, GetDimensions(pT), frames)));
	return pT;
}


//slowly find a texture by handle

const TexCache::Data& TexCache::Get(ID3D12Resource* pTex) {

	MyMap::iterator it = mCache.begin();
	Data* p = nullptr;
	while (it != mCache.end() && !p)
	{
		if ((*it).second.pTex == pTex)
			p = &(*it).second;
		++it;
	}
	assert(p);
	return *p;
}

//Vector2 TexCache::GetDimensions(ID3D12Resource* pTex)
//{
//	assert(pTex);
//	ID3D12Resource* res = nullptr;
//	pTex->GetResource(&res);
//	assert(res);
//	ID3D1Texture2D* texture2d = nullptr;
//	HRESULT hr = res->QueryInterface(&texture2d);
//	Vector2 dim(0, 0);
//	if (SUCCEEDED(hr))
//	{
//		D3D11_TEXTURE2D_DESC desc;
//		texture2d->GetDesc(&desc);
//		dim.x = static_cast<float>(desc.Width);
//		dim.y = static_cast<float>(desc.Height);
//	}
//	ReleaseCOM(texture2d);
//	ReleaseCOM(res);
//	return dim;
//}
//
