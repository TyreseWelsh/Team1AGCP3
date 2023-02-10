#include "Map.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace std;

Map::Map()
{
	FILE* fp = fopen("data/test_level_jump1.json", "rb");		// opens json file 

	char readBuffer[10000];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document document;
	document.ParseStream(is);			// parses json file 

	fclose(fp);

	// Stores all the data from the json file in the respective variables from the class

	height = document["height"].GetInt();
	infinite = document["infinite"].GetBool();			

	//GenericArray layersA = document["layers"].GetArray();
	//for (size_t i = 0; i < layersA.Capacity(); i++)
	//{
	//	Layers layer(layersA[i]);
	//	layers.push_back(layer);
	//}

	nextlayerid = document["nextlayerid"].GetInt();
	nextobjectid = document["nextobjectid"].GetInt();
	orientation = document["orientation"].GetString();
	renderorder = document["renderorder"].GetString();
	tiledversion = document["tiledversion"].GetString();
	tileheight = document["tileheight"].GetInt();

	GenericArray layersArray = document["layers"].GetArray();									// Gets everything in the layers array in json and stores in new 
	GenericObject test = layersArray.begin()->GetObject();
	GenericArray data = test["data"].GetArray();
	int data1 = data[696].GetInt();
	string floor = test["name"].GetString();
	//layers.push_back(layer);

	rapidjson::Value::Array tileArray = document["tilesets"].GetArray();
	for (size_t i = 0; i < tileArray.Capacity(); i++)
	{
		TileSetMap tileset(tileArray[i]);
		tilesets.push_back(tileset);
	}

	tilewidth = document["tilewidth"].GetInt();
	type = document["type"].GetString();
	width = document["width"].GetInt();

	

	delete[](fp);
}

Layers::Layers(rapidjson::Value& value)			
{
	if (value.HasMember("data"))
	{
		for (auto& v : value.GetArray()) 
		{
			data.push_back(v.GetInt());
		}
	}
	
	if (value.HasMember("height"))
	{
		height = value["height"].GetInt();
	}

	id = value["id"].GetInt();

	if (value.HasMember("image")) 
	{
		image = value["image"].GetString();
	}

	name = value["name"].GetString();
	opacity = value["opacity"].GetInt();
	type = value["type"].GetString();
	visible = value["visible"].GetBool();

	if (value.HasMember("width")) 
	{
		width = value["width"].GetInt();
	}

	x = value["x"].GetInt();
	y = value["y"].GetInt();
}

TileSetMap::TileSetMap(rapidjson::Value& value) 
{
	firstgid = value["firstgid"].GetInt();
	source = value["source"].GetString();
}


TileSet::TileSet(int firstgid, const char* tileset)
{
	this->firstgid = firstgid;
	Document tilesetDoc;
	tilesetDoc.Parse(tileset);

	columns = tilesetDoc["columns"].GetInt();
	image = tilesetDoc["image"].GetString();
	imageHeight = tilesetDoc["imageheight"].GetInt();
	imageWidth = tilesetDoc["imagewidth"].GetInt();
	margin = tilesetDoc["margin"].GetInt();
	name = tilesetDoc["name"].GetString();
	spacing = tilesetDoc["spacing"].GetInt();
	tilecount = tilesetDoc["tilecount"].GetInt();
	tileheight = tilesetDoc["tileheight"].GetInt();
	tilewidth = tilesetDoc["tilewidth"].GetInt();
	type = tilesetDoc["type"].GetString();

	delete[](tileset);
}
