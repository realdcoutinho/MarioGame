#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//TODO_W7(L"Implement SpriteFontLoader >> Parse .fnt file")
		//See BMFont Documentation for Binary Layout


	//Parse the Identification bytes (B,M,F)
	const auto id0 = pReader->Read<char>();
	const auto id1 = pReader->Read<char>();
	const auto id2 = pReader->Read<char>();


	//If Identification bytes doesn't match B|M|F,
	if ( id0 != 'B' || id1 != 'M' || id2 != 'F')
	{
		//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
		//return nullptr
		//...
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid.fnt font");
		return nullptr;
	}


	//Parse the version (version 3 required)
	char version = pReader->Read<byte>();
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...

	if (version < 3) 
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	char blockId = pReader->Read<byte>();
	int blockSize = pReader->Read<int>();
	//Retrieve the FontSize [fontDesc.fontSize]
	fontDesc.fontSize = static_cast<short>(pReader->Read<int>());  // so far we moved 11 bytes
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->SetBufferPosition(23);
	//Retrieve the FontName [fontDesc.fontName]
	fontDesc.fontName = pReader->ReadNullString();

	////////**********
	//////// BLOCK 1 *
	////////**********
	////////Retrieve the blockId and blockSize
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<int>();
	////////Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]


	pReader->MoveBufferPosition(4);


	fontDesc.textureWidth = pReader->Read<short>();
	fontDesc.textureHeight = pReader->Read<short>();
	////////Retrieve PageCount
	short pageCount = pReader->Read<short>();
	////////> if pagecount > 1
	////////	> Log Error (Only one texture per font is allowed!)
	if (pageCount > 1) 
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");
	}
	//Advance to Block2 (Move Reader)
	pReader->MoveBufferPosition(5);


	////////**********
	//////// BLOCK 2 *
	////////**********
	////////Retrieve the blockId and blockSize
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<int>();
	////////Retrieve the PageName (BinaryReader::ReadNullString)
	//////pReader->MoveBufferPosition(2);
	const std::wstring pageName = pReader->ReadNullString();

	if (pageName.empty()) 
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty]");
	}

	const auto texturePath = loadInfo.assetFullPath.parent_path().append(pageName);
	fontDesc.pTexture = ContentManager::Load<TextureData>(texturePath);


	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<int>();
	//Retrieve Character Count (see documentation)
	const UINT charCount = blockSize / 20;

	//Create loop for Character Count, and:
	for (UINT i = 0; i < charCount; ++i)
	{
		//> Create instance of FontMetric (struct)
		FontMetric instFontMetric{};

		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		wchar_t charId = static_cast<wchar_t>(pReader->Read<UINT>());

		//> Set Character (CharacterId) [FontMetric::character]
		instFontMetric.character = charId;

		//> Retrieve Xposition (store Local
		const short xPos = pReader->Read<short>();
		//> Retrieve Yposition (store Local)
		const short yPos = pReader->Read<short>();

		//> Retrieve & Set Width [FontMetric::Width]
		instFontMetric.width = pReader->Read<short>();
		//> Retrieve & Set Height [FontMetric::Height]
		instFontMetric.height = pReader->Read<short>();

		//> Retrieve & Set OffsetX [FontMetric::OffsetX]
		instFontMetric.offsetX = pReader->Read<short>();
		//> Retrieve & Set OffsetY [FontMetric::OffsetY]
		instFontMetric.offsetY = pReader->Read<short>();

		//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
		instFontMetric.advanceX = pReader->Read<short>();
		//> Retrieve & Set Page [FontMetric::Page]
		instFontMetric.page = pReader->Read<char>();


		//> Retrieve Channel (BITFIELD!!!) 		
		const char channel = pReader->Read<char>();
		//> See documentation for BitField meaning [FontMetrix::channel]
		switch (channel)
		{
		case 1:
			instFontMetric.channel = 2;
			break;
		case 2:
			instFontMetric.channel = 1;
			break;
		case 4:
			instFontMetric.channel = 0;
			break;
		case 8:
			instFontMetric.channel = 3;
			break;
		default:
			instFontMetric.channel = 0;
			break;
		}

		//> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		instFontMetric.texCoord.x = static_cast<float>(xPos) / static_cast<float>(fontDesc.textureWidth);
		instFontMetric.texCoord.y = static_cast<float>(yPos) / static_cast<float>(fontDesc.textureHeight);

		//> Insert new FontMetric to the metrics [font.metrics] map
		//> key = (wchar_t) charId
		//> value = new FontMetric
		//(loop restarts till all metrics are parsed)
		fontDesc.metrics.insert(std::pair(charId, instFontMetric));
	}

	





	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
